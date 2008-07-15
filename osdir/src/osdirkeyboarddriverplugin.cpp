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
 * OSD IR Input Plugin Source for Neuros OSD
 *
 * REVISION:
 *
 * 1) Initial creation. ----------------------------------- 2006-12-20 tom@limesg.com
 * 2) Imported To Neuros Firmware-------------------------- 2007-10-25 tom@limesg.com
 *
 */


#include "osdirkeyboarddriverplugin.h"

QStringList OSDIrKeyboardDriverPlugin::keys() const
{
	return QStringList() << "osdir";
}

QWSKeyboardHandler* OSDIrKeyboardDriverPlugin::create(const QString& key,const QString &device)
{
	std::cout << "::create(QString,QString) Ran" << std::endl;
	if(key.toLower() == "osdir")
		return new OSDIrKeyboardHandler(device);
	return 0;
}

QWSKeyboardHandler* OSDIrKeyboardDriverPlugin::create(const QString &name)
{
	std::cout << "::create(QString) Ran" << std::endl;
	return create(name,QString());

}

OSDIrKeyboardDriverPlugin::OSDIrKeyboardDriverPlugin()
: QKbdDriverPlugin()
{
	std::cout << "OSDIrKeyboardDriverPlugin Created" << std::endl;

}

Q_EXPORT_PLUGIN2(osdir,OSDIrKeyboardDriverPlugin)
