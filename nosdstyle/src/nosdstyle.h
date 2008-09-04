#ifndef _NOSDSTYLE_H_
#define _NOSDSTYLE_H_

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
 * OSD Qt4 Style Theme support header.
 *
 * REVISION:
 *
 * 3) Import to OSD20. ----------------------------------- 2008-07-21 WX
 * 2) Restyle buttons. ------------------------------------ 2008-01-21 nerochiaro
 * 1) Initial creation. ----------------------------------- 2007-10-26 Tom Bruno
 *
 */

#include <QStyle>
#include <QStyleOption>
#include <QWindowsStyle>
#include <QWidget>
#include <QPainter>
#include <QPalette>
#include <QPushButton>

class NOSDStyle : public QWindowsStyle
{
    Q_OBJECT

public:
    NOSDStyle() {}

    virtual void polish(QWidget *widget);
    virtual void polish(QPalette &palette);

    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = NULL) const;
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = NULL) const;
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option = NULL, const QWidget *widget = NULL) const;
    virtual QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *widget = NULL) const;
    virtual QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget = NULL) const;
    virtual QRect subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget = NULL) const;

private:
    QPainterPath roundRectPath(const QRect &rect) const;
    void drawSlopePanel(QPainter *p, const QRect &r, const QColor &lighter, const QColor &darker, int direction, bool round = false) const;
    void drawRect(QPainter *p, const QRect &r, int width, const QColor &c, bool round = false) const;

private:
    static const unsigned int buttonDarkening = 120;

};

#endif // _NOSDSTYLE_H_

