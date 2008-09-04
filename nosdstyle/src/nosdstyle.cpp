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
 * OSD Qt4 Style Theme source.
 *
 * REVISION:
 *
 * 3) Import to OSD20. ----------------------------------- 2008-07-21 WX
 * 2) Restyle buttons. ------------------------------------ 2008-01-21 nerochiaro
 * 1) Initial creation. ----------------------------------- 2007-10-26 Tom Bruno
 *
 */

//#define QT_NO_DEBUG_OUTPUT
#include <QDebug>
#include <QProgressBar>
#include "nosdstyle.h"

/*
 * Initializes the appearance of the given widget.
 * This function is called for every widget at some point
 * after it has been fully created but just before it is shown for the very first time.
 */
void NOSDStyle::polish(QWidget *widget)
{
    return QWindowsStyle::polish(widget);
}

/*
 * Changes the palette according to style specific requirements for color palettes (if any).
 */
void NOSDStyle::polish(QPalette &palette)
{
    palette.setBrush(QPalette::Text, Qt::white);
    palette.setBrush(QPalette::WindowText, Qt::white);
    palette.setBrush(QPalette::Base, Qt::black);
    palette.setBrush(QPalette::Window, Qt::black);
    palette.setBrush(QPalette::Mid, QColor(0x80, 0x80, 0x80));
    palette.setBrush(QPalette::Button, QColor(0x00, 0x72, 0x7D));
    palette.setBrush(QPalette::ButtonText, QColor(0xB2, 0xB2, 0xB2));
}

/*
 * Draws the given element with the provided painter with the style options specified by option.
 */
void NOSDStyle::drawControl(ControlElement element, const QStyleOption *option, 
                            QPainter *painter, const QWidget *widget) const 
{
    switch (element)
    {
    case CE_PushButtonLabel:
        //For NPushButton, we want the default text to be white when it's focused, instead of QPalette::ButtonText
        if ((option->state & QStyle::State_HasFocus) == QStyle::State_HasFocus )
        {
            const QStyleOptionButton *buttonOpt = qstyleoption_cast<const QStyleOptionButton *>(option);
            if (NULL == buttonOpt) return;
            QStyleOptionButton customOpt(*buttonOpt);
            customOpt.palette.setBrush(QPalette::ButtonText, Qt::white);
            QWindowsStyle::drawControl(element, &customOpt, painter, widget);
            return;
        }
        break;
    case CE_ProgressBarGroove:
        {
            if (widget->metaObject()->className() != QString("NProgressBar"))
                break;
            const QProgressBar *bar = qobject_cast<const QProgressBar *>(widget);
            if (NULL != bar)
            {
                painter->save();
                drawSlopePanel(painter, option->rect,
                               option->palette.color(QPalette::Window).lighter(), 
                               option->palette.color(QPalette::Window).darker(), 
                               bar->orientation() == Qt::Horizontal ? 2 : 0, true);
                drawRect(painter, option->rect, 5, option->palette.color(QPalette::Highlight).darker(150), true);
                painter->restore();
                return;
            }
        }
        break;
    case CE_ProgressBarContents:
        {
            if (widget->metaObject()->className() != QString("NProgressBar"))
                break;

            if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(option))
            {
                QRect rect = pb->rect;
                bool vertical = false;
                bool inverted = false;
                qint64 minimum = qint64(pb->minimum);
                qint64 maximum = qint64(pb->maximum);
                qint64 progress = qint64(pb->progress);

                if (minimum == 0 && maximum == 0)
                    return; // nothing to paint

                if (const QStyleOptionProgressBarV2 *pb2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option))
                {
                    vertical = (pb2->orientation == Qt::Vertical);
                    inverted = pb2->invertedAppearance;
                }

                bool reverse = ((!vertical && (pb->direction == Qt::RightToLeft)) || vertical);
                if (inverted)
                    reverse = !reverse;

                int percent = int(((qreal(progress) - qreal(minimum))*100.0) / (maximum - minimum));

                QRect lighterRect;
                QRect darkerRect;
                if (!vertical)
                {
                    int w = rect.width()*percent/100;
                    lighterRect =QRect(reverse ? rect.right() - w: rect.x(), rect.y(), w, rect.height()/2);
                    darkerRect =QRect(reverse ? rect.right() - w: rect.x(), rect.y() + rect.height()/2, w, rect.height()/2);
                }
                else
                {
                    int h = rect.height()*percent/100;
                    lighterRect =QRect(rect.x(), reverse ? rect.bottom() - h : rect.y(), rect.width()/2, h);
                    darkerRect =QRect(rect.x() + rect.width()/2, reverse ? rect.bottom() - h : rect.y(), rect.width()/2, h);
                }

                QPainterPath path = roundRectPath(pb->rect.adjusted(2, 3, -2, -2));
                QPainterPath lighterPath, darkerPath;
                lighterPath.addRect(lighterRect);
                darkerPath.addRect(darkerRect);
                painter->fillPath(path.intersected(lighterPath), pb->palette.color(QPalette::Highlight).darker(200));
                painter->fillPath(path.intersected(darkerPath), pb->palette.color(QPalette::Highlight).darker(250));
                return;
            }
        }
        break;
    default:
        break;
    }

    return QWindowsStyle::drawControl(element, option, painter, widget);
}

/*
 * Draws the given primitive element with the provided painter using the style options specified by option.
 */
void NOSDStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                              QPainter *painter, const QWidget *widget) const
{
    switch (element)
    {
    case PE_PanelButtonCommand:
        {
            painter->save();
            bool focused = ((option->state & QStyle::State_HasFocus) == QStyle::State_HasFocus);
            QPainterPath roundbutton = roundRectPath(option->rect);
            painter->save();
            painter->setClipPath(roundbutton);
            if (focused)
            {
                if ((option->state & QStyle::State_Sunken) == QStyle::State_Sunken)
                {
                    QBrush btnbrush(QColor(0xe4, 0x5b, 0x00));
                    painter->fillRect(option->rect, btnbrush);
                }
                else
                {
                    QBrush btnbrush(option->palette.button());
                    painter->fillRect(0, 0, option->rect.width(), option->rect.height() / 2, btnbrush);
                    btnbrush.setColor(btnbrush.color().darker(buttonDarkening));
                    painter->fillRect(0, option->rect.height() / 2, option->rect.width(), option->rect.height() / 2, btnbrush);
                }
            }
            else
            {
                QBrush btnbrush(QColor(0x4d, 0x4d, 0x4d));
                painter->fillRect(option->rect, btnbrush);
            }

            QPen pen(option->palette.mid(), 3, Qt::SolidLine);
            painter->restore();
            painter->setPen(pen);
            painter->drawPath(roundbutton);
            painter->restore();
            return;
        }
        break;
    case PE_FrameFocusRect:
        if (NULL != qobject_cast<const QPushButton*>(widget))
            return;
        break;
    case PE_IndicatorArrowUp:
    case PE_IndicatorArrowDown:
    case PE_IndicatorArrowLeft:
    case PE_IndicatorArrowRight:
        {
            int x = option->rect.x();
            int y = option->rect.y();
            int w = option->rect.width();
            int h = option->rect.height();
            QPolygon points;
            switch (option->type)
            {
            case QStyleOption::SO_Slider:
            case QStyleOption::SO_MenuItem:
                switch (element)
                {
                case PE_IndicatorArrowUp:
                    points.setPoints(6, x, y + h - 2, x + w/2, y, x + w, y + h - 2,
                                     x + w, y + h, x + w/2, y + 2, x, y + h);
                    break;
                case PE_IndicatorArrowDown:
                    points.setPoints(6, x, y, x, y + 2, x + w/2, y + h,
                                     x + w, y + 2, x + w, y, x + w/2, y + h - 2);
                    break;
                case PE_IndicatorArrowLeft:
                    points.setPoints(6, x + w - 2, y, x, y + h/2, x + w - 2, y + h,
                                     x + w, y + h, x + 2, y + h/2, x + w, y);
                    break;
                case PE_IndicatorArrowRight:
                    points.setPoints(6, x, y, x + w - 2, y + h/2, x, y + h,
                                     x + 2, y + h, x + w, y + h/2, x + 2, y);
                    break;
                default:
                    break;
                }
                break;
            default:
                switch (element)
                {
                case PE_IndicatorArrowUp:
                    points.setPoints(3, x, y + h, x + w, y + h, x + w/2, y);
                    break;
                case PE_IndicatorArrowDown:
                    points.setPoints(3, x, y, x + w, y, x + w/2, y + h);
                    break;
                case PE_IndicatorArrowLeft:
                    points.setPoints(3, x, y + h/2, x + w, y, x + w, y + h);
                    break;
                case PE_IndicatorArrowRight:
                    points.setPoints(3, x, y, x + w, y + h/2, x, y + h);
                    break;
                default:
                    break;
                }
            }

            painter->save();
            painter->setPen(option->palette.buttonText().color());
            painter->setBrush(option->palette.buttonText());
            painter->drawPolygon(points);
            painter->restore();
            return;
        }
        break;
    default:
        break;
    }

    return QWindowsStyle::drawPrimitive(element, option, painter, widget);
}

int NOSDStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric)
    {
    case PM_ProgressBarChunkWidth:
        return 5;
    case QStyle::PM_MenuHMargin:
        if (widget->metaObject()->className() == QString("NXim"))
            return 10;
    case QStyle::PM_MenuVMargin:
        if (widget->metaObject()->className() == QString("NXim"))
            return 0;
    default:
        return QWindowsStyle::pixelMetric(metric, option, widget);
    }
}

/*
 * Returns the size of the element described by the specified option and type,
 * based on the provided contentsSize.
 */
QSize NOSDStyle::sizeFromContents(ContentsType ct, const QStyleOption *opt,
                                  const QSize &csz, const QWidget *widget) const
{
    QSize sz(csz);
    switch (ct)
    {
    case CT_ComboBox:
        if (widget->metaObject()->className() != QString("NHorizontalComboBox"))
            break;
        sz = QWindowsStyle::sizeFromContents(ct, opt, csz, widget) + QSize(2*pixelMetric(PM_LayoutHorizontalSpacing, opt, widget), 0);
        return sz;
    case CT_MenuItem:
        if (widget->metaObject()->className() != QString("NXim"))
            break;
        sz = QWindowsStyle::sizeFromContents(ct, opt, csz, widget);
        if (sz.height( ) < 30)
            sz.setHeight(30);
        return sz;
    default:
        break;
    }

    sz = QWindowsStyle::sizeFromContents(ct, opt, csz, widget);
    return sz;
}

/*
 * Returns the sub-area for the given element as described in the provided style option.
 * The returned rectangle is defined in screen coordinates.
 */
QRect NOSDStyle::subElementRect(SubElement sr, const QStyleOption *opt,
                                const QWidget *widget) const
{
    QRect r;
    switch (sr)
    {
    case SE_ProgressBarGroove:
    case SE_ProgressBarContents:
    case SE_ProgressBarLabel:
        if (widget->metaObject()->className() != QString("NProgressBar"))
            break;

        if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(opt))
        {
            if ((sr == SE_ProgressBarGroove) || (sr == SE_ProgressBarContents))
                r = pb->rect;
            else
                r = itemTextRect(pb->fontMetrics,
                                 pb->rect.adjusted(pb->fontMetrics.height(), pb->fontMetrics.height(),
                                                   -pb->fontMetrics.height(), -pb->fontMetrics.height()),
                                 pb->textAlignment, false, pb->text);

            r = visualRect(pb->direction, pb->rect, r);
            return r;
        }
        break;
    default:
        break;
    }

    return QWindowsStyle::subElementRect(sr, opt, widget);
}

/*
 * Returns the rectangle containing the specified subControl of the given complex control (with the style specified by option).
 * The rectangle is defined in screen coordinates.
 */
QRect  NOSDStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option,
                                 SubControl subControl, const QWidget *widget) const
{
    switch (control)
    {
    case CC_ComboBox:
        if (widget->metaObject()->className() != QString("NHorizontalComboBox"))
            break;
        if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option))
        {
            QRect ret;
            int fw = cb->frame ? pixelMetric(PM_ComboBoxFrameWidth, option, widget) : 0;
            int hs = pixelMetric(PM_LayoutHorizontalSpacing, option, widget);
            int ah = qMin(QFontMetrics(widget->font()).height(), cb->rect.height());
            int aw = qMin(ah*2/5, cb->rect.width()/2);

            switch (subControl)
            {
            case SC_ComboBoxFrame:
            case SC_ComboBoxListBoxPopup:
                ret = cb->rect;
                break;
            case SC_ComboBoxArrow:
                ret.setRect(cb->rect.x() + fw, cb->rect.y() + (cb->rect.height() - ah)/2, aw, ah);
                break;
            case SC_ComboBoxEditField:
                ret.setRect(cb->rect.x() + fw + hs + aw, cb->rect.y() + fw,
                            cb->rect.width() - 2*fw - 2*hs - 2*aw, cb->rect.height() - 2*fw);
                break;
            default:
                break;
            }
            ret = visualRect(cb->direction, cb->rect, ret);
            return ret;
        }
    default:
        break;
    }

    return QWindowsStyle::subControlRect(control, option, subControl, widget);
}

/*
 * Draws a slope panel in the given rectangle of painter using the lighter, darker and direction.
 * direction: 0 LeftToRight, 1 RightToLeft, 2 TopToBottom,3 BottomToTop
 * round: draw round rectangle if true
 */
void NOSDStyle::drawSlopePanel(QPainter *p, const QRect &r, const QColor &lighter, 
                               const QColor &darker, int direction, bool round) const
{
    QRect lighterRect;
    QRect darkerRect;
    switch (direction)
    {
    case 0: // LeftToRight
        lighterRect = r.adjusted(0, 0, - r.width()/2, 0);
        darkerRect = r.adjusted(r.width()/2, 0, 0, 0);
        break;
    case 1: // RightToLeft
        lighterRect = r.adjusted(r.width()/2, 0, 0, 0);
        darkerRect = r.adjusted(0, 0, - r.width()/2, 0);
        break;
    case 2: // TopToBottom
        lighterRect = r.adjusted(0, 0, 0, - r.height()/2);
        darkerRect = r.adjusted(0, r.height()/2, 0, 0);
        break;
    case 3: // BottomToTop
        lighterRect = r.adjusted(0, r.height()/2, 0, 0);
        darkerRect = r.adjusted(0, 0, 0, - r.height()/2);
        break;
    default:
        break;
    }

    if (round)
    {
        QPainterPath path = roundRectPath(r);
        QPainterPath lighterPath, darkerPath;
        lighterPath.addRect(lighterRect);
        darkerPath.addRect(darkerRect);
        p->fillPath(path.intersected(lighterPath), lighter);
        p->fillPath(path.intersected(darkerPath), darker);
    }
    else
    {
        p->fillRect(lighterRect, lighter);
        p->fillRect(darkerRect, darker);
    }
}

void NOSDStyle::drawRect(QPainter *p, const QRect &r, int width, const QColor &c, bool round) const
{
    QPen pen(c);
    pen.setWidth(width);
    p->setPen(pen);
    QPainterPath path;
    if (round)
        path.addPath(roundRectPath(r));
    else
        path.addRect(r);

    p->drawPath(path);
}

QPainterPath NOSDStyle::roundRectPath(const QRect &rect) const
{
    int radius = rect.height() / 5;
    int diam = 2 * radius;
    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}

