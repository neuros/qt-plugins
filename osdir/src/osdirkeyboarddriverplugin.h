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
 * OSD IR Input Plugin Header for Neuros OSD
 *
 * REVISION:
 *
 * 1) Initial creation. ----------------------------------- 2006-12-20 tom@limesg.com
 * 2) Imported To Neuros Firmware-------------------------- 2007-10-25 tom@limesg.com
 *
 */


#ifndef OSDIRKEYBOARDDRIVERPLUGIN_H
#define OSDIRKEYBOARDDRIVERPLUGIN_H

#include <QtGui>

#include <QStringList>
#include <QString>

#include <QKbdDriverPlugin>
#include <QWSKeyboardHandler>

#include "osdirkeyboardhandler.h"
#include <QtPlugin>
#include <QtGui>
#include <iostream>
class OSDIrKeyboardDriverPlugin : public QKbdDriverPlugin
{

public:
	OSDIrKeyboardDriverPlugin();

	virtual QStringList keys() const;
	QWSKeyboardHandler *create(const QString& name);
	QWSKeyboardHandler *create(const QString& key,const QString &device);
};

#endif

