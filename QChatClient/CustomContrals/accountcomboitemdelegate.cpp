#include "accountcomboitemdelegate.h"
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>
#include<QAbstractItemView>
AccountComboItemDelegate::AccountComboItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void AccountComboItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    QString itemText = index.data(Qt::DisplayRole).toString();

    // 设置“×”按钮的绘制区域
    int padding = 5;
    QRect closeRect = QRect(rect.right() - 30, rect.top(), 16, rect.height());


    painter->drawText(rect.adjusted(padding, 0, -20, 0), Qt::AlignVCenter, itemText);

    QIcon closeIcon(":/resource/icon-close.png");
    closeIcon.paint(painter, closeRect, Qt::AlignCenter);
}

bool AccountComboItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        QRect closeRect = QRect(option.rect.right() - 30, option.rect.top(), 16, option.rect.height());
        if (closeRect.contains(mouseEvent->pos())) {
            qDebug()<<"closeRect.contains(mouseEvent->pos())";
            emit removeItemRequested(index);  // 发射删除信号
            qDebug()<<"closeRect.contains(mouseEvent->pos())"<<2;
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

ComboBoxWithRemoveButton::ComboBoxWithRemoveButton(QWidget *parent)
    : QComboBox(parent)
{
    AccountComboItemDelegate *delegate = new AccountComboItemDelegate(this);
    setItemDelegate(delegate);
    setModel(new QStandardItemModel(this));


    // 连接信号到槽函数以删除项
    connect(delegate, &AccountComboItemDelegate::removeItemRequested, this, &ComboBoxWithRemoveButton::removeItem);

//    // 使事件在下拉视图中生效
//    view()->viewport()->installEventFilter(delegate);
}

void ComboBoxWithRemoveButton::removeItem(const QModelIndex &index)
{
    qDebug()<<__FUNCTION__;
    model()->removeRow(index.row());  // 从模型中移除行
}


