#include <QtCore/QDebug>
#include <QtGui/QCommonStyle>

#include "ZLQtTreeModel.h"
#include "ZLQtTreeView.h"
#include "../dialogs/ZLQtProgressDialog.h"

ZLQtTreeView::ZLQtTreeView(QWidget *parent) :  QListView(parent) {

}

void ZLQtTreeView::paint() {
    qDebug() << Q_FUNC_INFO;
    update();
}

SubtitleDelegate::SubtitleDelegate(QObject *parent) : SeparatorDelegate(parent) {

}

void SubtitleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    drawHighlight(painter,option,index);
    drawIcon(painter,option,index);
    drawTitle(painter,option,index);
    drawSubtitle(painter,option,index);
    drawSeparator(painter,option,index);
}

void SubtitleDelegate::drawTitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString title = index.data(Qt::DisplayRole).toString();
    QString subtitle = index.data(ZLQtTreeModel::SubTitleRole).toString();
    if (title.isEmpty()) {
        return;
    }
    painter->save();
    painter->setClipRect(option.rect);
    painter->setPen( option.state & QStyle::State_HasFocus ?
                     option.palette.highlightedText().color() :
                     option.palette.text().color());

    QRect textRect;
    int alignFlags = Qt::AlignCenter;
    if (subtitle.isEmpty()) {
        textRect = option.rect;
        alignFlags = Qt::AlignVCenter;
    } else {
        int centerY = option.rect.bottom() - option.rect.size().height()/2;
        textRect = QRect(option.rect.topLeft(), QPoint(option.rect.right(), centerY));
        alignFlags = Qt::AlignBottom;
    }
    textRect.adjust(getTextLeftMargin(option, index), 0,0,0);

    painter->setFont(MenuItemParameters::getFont());
    QString elidedSubtitle = painter->fontMetrics().elidedText(title,Qt::ElideRight, textRect.width());
    painter->drawText(textRect, alignFlags,  elidedSubtitle);
    painter->restore();
}

void SubtitleDelegate::drawSubtitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString subtitle = index.data(ZLQtTreeModel::SubTitleRole).toString();
    if (subtitle.isEmpty()) {
        return;
    }
    painter->save();
    painter->setClipRect(option.rect);
    painter->setPen( option.state & QStyle::State_HasFocus ?
                     option.palette.highlightedText().color() :
                     option.palette.text().color());
    //QRect textRect = option.rect;//.adjusted(textMargin+iconMargin, 0, -(textMargin+iconMargin), 0);
    int centerY = option.rect.top() + option.rect.size().height()/2;
    QRect textRect(QPoint(option.rect.left(), centerY), option.rect.bottomRight());

    textRect.adjust(getTextLeftMargin(option, index), 0,0,0);

    painter->setFont(MenuItemParameters::getSubtitleFont());
    QString elidedSubtitle = painter->fontMetrics().elidedText(subtitle,Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignVCenter,  elidedSubtitle);
    painter->restore();
}

void SubtitleDelegate::drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    if (pixmap.isNull()) {
        return;
    }
    //painter->drawPixmap(QRect(option.rect.topLeft(), pixmap.size()) , pixmap);
    QPoint topLeft = QPoint( option.rect.left() + getLeftMargin(), option.rect.top());
    painter->drawPixmap( getCenteredRectangle( QRect(topLeft, MenuItemParameters::getImageZoneSize()),
                                               pixmap.size()) , pixmap);
    //painter->drawPixmap(QRect(iconRect.topLeft(), pixmap.size()), pixmap);

}

void SubtitleDelegate::drawHighlight(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option);
    QStyleOptionViewItemV4 voptAdj = *vopt;
    const QWidget* widget = voptAdj.widget;
    const QAbstractItemView *itemView = qobject_cast<const QAbstractItemView *>(widget);

    QStyle *style = widget ? widget->style() : QApplication::style();

    //painter->setClipRect(voptAdj.rect);
    const bool isSelected = (vopt->state & QStyle::State_Selected);
    const bool hasFocus = (vopt->state & QStyle::State_HasFocus);

    const bool singleSelection = itemView &&
        ((itemView->selectionMode() == QAbstractItemView::SingleSelection ||
         itemView->selectionMode() == QAbstractItemView::NoSelection));
    const bool selectItems = itemView && (itemView->selectionBehavior() == QAbstractItemView::SelectItems);

    //TODO this code has been brought from QS60Style and may be should be refactored
    // draw themed background for itemview unless background brush has been defined.
    if (vopt->backgroundBrush == Qt::NoBrush) {
        if (itemView) {
            //With single item selection, use highlight focus as selection indicator.
            if (singleSelection && isSelected){
                voptAdj.state = voptAdj.state | QStyle::State_HasFocus;
                if (!hasFocus && selectItems) {
                    painter->save();
                    painter->setOpacity(0.5);
                }
            }
            style->drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);
            if (singleSelection && isSelected && !hasFocus && selectItems) {
                painter->restore();
            }
        }
        //in original QS60Style, there was a line
        //} else { QCommonStyle::drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);}
    } else { style->drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);}


}

QSize SubtitleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    //qDebug() << Q_FUNC_INFO << index << QStyledItemDelegate::sizeHint(option,index) << MenuItemParameters::getItemSize();
    return QStyledItemDelegate::sizeHint(option,index);
}

QRect SubtitleDelegate::getCenteredRectangle(QRect zoneRect, QSize imageSize) {
    if (zoneRect.width() < zoneRect.width() || zoneRect.height() < zoneRect.height() ) {
        qDebug() << "SubtitleDelegate::getCenteredRectangle -- imageSize is more then zoneRect" << zoneRect << imageSize;
        return zoneRect;
    }
    return QRect( QPoint( zoneRect.left() + (zoneRect.width() - imageSize.width())  / 2,
                          zoneRect.top()  + (zoneRect.height()- imageSize.height()) / 2 ),
                          imageSize);
}

int SubtitleDelegate::getTextLeftMargin(const QStyleOptionViewItem &option, const QModelIndex &index) {
    Q_UNUSED(option);
//    qDebug() << "GET_TEXT_LEFT_MARGIN";
//    qDebug() << QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin, &option); // 3
//    qDebug() << QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option); // 6
//    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin, &option) + 2;
//    const int iconMargin = MenuItemParameters::getImageZoneSize().width() +
//                           QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option);
    int textMargin = 0;
    int iconMargin = 0;

    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    if (!pixmap.isNull()) {
        iconMargin = MenuItemParameters::getImageZoneSize().width(); //+
                     //QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option);
    }
    return getLeftMargin() + textMargin + iconMargin;
}

int SubtitleDelegate::getLeftMargin() {
    static const int LEFT_MARGIN = 6;
    return LEFT_MARGIN;
}
