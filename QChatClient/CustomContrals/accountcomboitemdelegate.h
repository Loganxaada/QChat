#ifndef ACCOUNTCOMBOITEMDELEGATE_H
#define ACCOUNTCOMBOITEMDELEGATE_H

#include<QStyledItemDelegate>
#include<QComboBox>
#include<QStandardItemModel>


class AccountComboItemDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AccountComboItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    // check mouse is or not in × rect
    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void removeItemRequested(const QModelIndex &index) const;
};

class ComboBoxWithRemoveButton : public QComboBox {
    Q_OBJECT

public:
    ComboBoxWithRemoveButton(QWidget *parent = nullptr);
public slots:
    void removeItem(const QModelIndex &index);
};

#endif // ACCOUNTCOMBOITEMDELEGATE_H
