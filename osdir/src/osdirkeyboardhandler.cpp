/*
 *  Copyright(C) 2006 Lime Software Group
 *               <www.limesg.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *
 *  This program is distributed in the hope that, in addition to its
 *  original purpose to support Neuros hardware, it will be useful
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 *
 * OSD IR Input Hanlder Source for Neuros OSD
 *
 * REVISION:
 *
 * 1) Initial creation. ----------------------------------- 2006-12-20 tom@limesg.com
 * 2) Imported To Neuros Firmware-------------------------- 2007-10-25 tom@limesg.com
 *
 */

#include "osdirkeyboardhandler.h"
#include <iostream>

static int  kbdFD = -1;
static int  tvpFD = -1;
#define TVP5150_POLL_DELAY  1000    //ms

OSDIrKeyboardHandler::OSDIrKeyboardHandler(const QString &device)
{
// 	std::cout << "OSDIrKeyboardHandler Created" << std::endl;

    // Opens the device file passed to it.
    kbdFD = ::open("/dev/neuros_ir", O_RDWR|O_NDELAY, 0);
    tvpFD = ::open("/dev/tvp5150", O_RDWR|O_NDELAY, 0);

    // If it is open setup a socket and connect to it to monitor changes.
    if (kbdFD >= 0)
    {
        QSocketNotifier *notifier;
        notifier = new QSocketNotifier(kbdFD, QSocketNotifier::Read, this);
        connect(notifier, SIGNAL(activated(int)),this,SLOT(readKeyboardData()));

    } else
    {
        qCritical("Cannot open keyboard: %s", strerror(errno)); 
    }

/*this should be use QSocketNotifier,but will slowly poll for tvp5150 status.
maybe a minute poll one times.so we use timer for now.*/

    tvp5150Timer.start(TVP5150_POLL_DELAY,this);

}

OSDIrKeyboardHandler::~OSDIrKeyboardHandler()
{
    ::close(kbdFD);
}

static int speckeycodemap[ ] =
{ 
	Qt::Key_unknown + 16,    // NVKEY_RELEASE_REMOTE  VKEY(  0x29  )
	Qt::Key_unknown,            //reserve VKEY(  0x2a  )
	Qt::Key_unknown,            //reserve VKEY(  0x2b  )
	Qt::Key_unknown,            //reserve VKEY(  0x2c  )
	Qt::Key_unknown,            //reserve VKEY(  0x2d  )
	Qt::Key_unknown,            //reserve VKEY(  0x2e  )
	Qt::Key_unknown,            //reserve VKEY(  0x2f  )
    Qt::Key_unknown + 1,    // NVKEY_VIDEOSIGNALNULL  VKEY(  0x30  )
    Qt::Key_unknown + 2,    // NVKEY_VIDEOSIGNALIN        VKEY(  0x31  )
	Qt::Key_unknown,     		//reserve VKEY(  0x32  )
	Qt::Key_unknown,     		//reserve VKEY(  0x33  )
    Qt::Key_unknown,     		//reserve VKEY(  0x34  )
    Qt::Key_unknown,     		//reserve VKEY(  0x35  )
    Qt::Key_unknown,     		//reserve VKEY(  0x36  )
    Qt::Key_unknown,     		//reserve VKEY(  0x37  )
    Qt::Key_unknown,     		//reserve VKEY(  0x38  )
    Qt::Key_unknown,     		//reserve VKEY(  0x39  )
    Qt::Key_unknown,     		//reserve VKEY(  0x3a  )
    Qt::Key_unknown,     		//reserve VKEY(  0x3b  )
    Qt::Key_unknown + 13,    // NVKEY_VIDEOMODENTSC     VKEY(  0x3c )
    Qt::Key_unknown + 14,    // NVKEY_VIDEOMODEPAL      VKEY(  0x3d  )
    Qt::Key_unknown + 15,    // NVKEY_LEARNING_COMPLETE VKEY(  0x3e  )
    Qt::Key_unknown + 17,   // VKEY_TEST          VKEY(  0x3f  )
};


#define SPEC_KEY_BASE       0x29
#define KEY(X)              speckeycodemap[ (X) -  SPEC_KEY_BASE ]  

void OSDIrKeyboardHandler::readKeyboardData()
{

    static int keycodemap[NUM_KEYS] = {
        0,                //0
        Qt::Key_1,          //1
        Qt::Key_2,          //2
        Qt::Key_3,          //3
        Qt::Key_4,          //4
        Qt::Key_5,          //5
        Qt::Key_6,          //6
        Qt::Key_7,          //7
        Qt::Key_8,          //8
        Qt::Key_9,          //9
        Qt::Key_0,          //10
        0,                //11
        0,                //12
        0,                //13
        0,                //14
        Qt::Key_Asterisk,     //15    "*"
        Qt::Key_Clear,       //16    "#"  //this was the only way.
        Qt::Key_PageUp,       //17    CHAN_UP
        Qt::Key_PageDown,     //18    CHAN_DN
        Qt::Key_Escape,    //19    BACK
        Qt::Key_HomePage,       //20    HOMEPAGE 
        Qt::Key_Up,           //21    UP
        Qt::Key_Down,         //22    DOWN
        Qt::Key_Left,         //23    LEFT
        Qt::Key_Right,    //24    RIGHT
        Qt::Key_Enter,    //25    ENTER
        Qt::Key_Help,       //26    HELP  // no better idea than H
        Qt::Key_Menu,       //27    XIM   // TAB seems just better than X, no other reason
        Qt::Key_F12,        //28    UNLABELED_1  // tiny unlabeled key on remote. used to engage passthrough mode. also called "DISP"
        Qt::Key_Back,      //29    REW
        Qt::Key_Forward,       //30    FFW
        Qt::Key_MediaPlay,        //31    PLAY  //Most media apps use space for quick pause/play
        Qt::Key_MediaPrevious,         //32    PREV
        Qt::Key_MediaNext,          //33    NEXT
        Qt::Key_MediaStop,       //34    STOP
        Qt::Key_MediaRecord,        //35    RECORD
		Qt::Key_Mode_switch,		// 36     NTSC_PAL 
    };

	int key = -1;
	int status = read(kbdFD, &key, sizeof(int));

	if (status==4) // 4 means that the key was read successfully.
	{
        if ( key == 0 )
		{
			// This is a key release notification. Stop any auto-repeat and do nothing else (we already sent the
			// press and release notification if this was a short keypress)
			endAutoRepeat();
		}
        else if (key < SPEC_KEY_BASE)
		{
			// For each key press we send out a press+release pair immediately, then we start autorepeat for that key.
			// This will do the same effect as a shork keypress if the key is released withing 400ms.
			// Otherwise autorepeat is triggered every 80ms until release of key.
			processKeyEvent(0xFFFF,keycodemap[key],0,true,false);
			processKeyEvent(0xFFFF,keycodemap[key],0,false,false);
			beginAutoRepeat(0xFFFF,keycodemap[key],0);
		}
		else 
		{
			// These are "fake" IR keys that are emitted to notify the system of a series of events (see above for the list).
			// They don't need to have any autorepeat handling or even a press-release logic. They are just sent out once.
			processKeyEvent(0xFFFF,KEY(key),0,true,false);
		}
	}
	else std::cout << "[QT-IR] WARNING: IR key reading failure (status: " << status << "\n";
}

void OSDIrKeyboardHandler::processKeyEvent(int unicode, int keycode, Qt::KeyboardModifiers modifiers,
                                           bool isPress, bool autoRepeat)
{
    QWSKeyboardHandler::processKeyEvent(unicode,keycode,modifiers,isPress,autoRepeat);

}

void OSDIrKeyboardHandler::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == tvp5150Timer.timerId() )
    {
        fd_set rfds;
        struct timeval t;
        //poll
        t.tv_sec = 0;
        t.tv_usec = 0;

        if (tvpFD >= 0 )
        {

            FD_ZERO(&rfds);
            FD_SET(tvpFD, &rfds);

            if (select(tvpFD+1,&rfds,NULL,NULL,&t) >= 0)
            {
                if (FD_ISSET(tvpFD, &rfds))
                {
                    int key = -1;
                    int r = read(tvpFD, &key, sizeof(int));
                    std::cout << "TVP5150 INPUT Read : " << r << " - Key: " << key << std::endl;
                    if (r == sizeof(int))
                    {
                        processKeyEvent(0xFFFF,KEY(key),0,true,false);

                    }
                }

            }
        }

    }
}



