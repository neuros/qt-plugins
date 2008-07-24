
/*
 *  Copyright(C) 2007 Neuros Technology International LLC.
 *               <www.neurostechnology.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *
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
 * OSD Qt4 Style plugin source.
 *
 * REVISION:
 *
 * 2) Import to OSD20. ----------------------------------- 2008-07-18 WX
 * 1) Initial creation. ----------------------------------- 2007-10-26 Tom Bruno
 *
 */

//#define QT_NO_DEBUG_OUTPUT
#include <QDebug>
#include "nosdstyleplugin.h"
#include "nosdstyle.h"

/*
 * Constructs a style plugin with the given parent.
 * Note that this constructor is invoked automatically by the Q_EXPORT_PLUGIN2() macro, 
 * so there is no need for calling it explicitly.
 */
NOSDStylePlugin::NOSDStylePlugin(QObject *parent)
: QStylePlugin(parent)
{
    qDebug( ) << "NOSDStylePlugin::NOSDStylePlugin" << parent;
}

/*
 * Returns the list of style keys this plugin supports.
 * These keys are usually the class names of the custom styles that are implemented in the plugin.
 */
QStyle * NOSDStylePlugin::create(const QString &key)
{
    qDebug( ) << "NOSDStylePlugin::create" << key;
    if (key.toLower( ) == "nosdstyle")
        return new NOSDStyle;

    return NULL;
}

/*
 * Creates and returns a QStyle object for the given style key.
 * The style key is usually the class name of the required style.
 * Note that the keys are case insensitive.
 */
QStringList NOSDStylePlugin::keys( ) const
{
    qDebug( ) << "NOSDStylePlugin::keys" << "NOSDStyle";

    return QStringList( ) << "NOSDStyle";
}

/*
 * Q_EXPORT_PLUGIN2(PluginName, ClassName)
 * This macro exports the plugin class ClassName for the plugin specified by PluginName.
 * The value of PluginName should correspond to the TARGET specified in the plugin's project file.
 */
Q_EXPORT_PLUGIN2(nosdstyle, NOSDStylePlugin)

