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

#ifndef OSDIRKEYBOARDHANDLER_H
#define OSDIRKEYBOARDHANDLER_H

#include <QStringList>
#include <QString>
#include <QWSKeyboardHandler>
#include <QSocketNotifier>
#include <QBasicTimer>

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h> 
#include <errno.h>

#define NUM_KEYS 37



class OSDIrKeyboardHandler :  public QObject,public QWSKeyboardHandler
{
Q_OBJECT

public:
	explicit OSDIrKeyboardHandler(const QString&);
	~OSDIrKeyboardHandler();


	void processKeyEvent(int unicode, int keycode, Qt::KeyboardModifiers modifiers,bool isPress, bool autoRepeat);
    

private slots:
	void readKeyboardData();

private:
    void timerEvent(QTimerEvent *e);
    QBasicTimer tvp5150Timer;
    QBasicTimer irholdTimer;
    QBasicTimer irrepeatTimer;

};


#endif

