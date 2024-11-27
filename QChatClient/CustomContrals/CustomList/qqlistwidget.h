#ifndef QQLISTWIDGET_H
#define QQLISTWIDGET_H

#include <QListWidget>
#include <QList>
#include <QHash>
#include <QMenu>

#include "qqcell.h"
#include "qqlistviewgroup.h"
#include "qqlistviewchild.h"

class QQListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QQListWidget(QWidget *parent = 0);

    QQCell *getRightClickedCell();
signals:
    void on_popmenu_will_show(QQListWidget *listWidget,QQPopMenuWidget *widget, QMenu *menu);
    void sig_childDoubleClicked(QQCell *child);

public slots:
    void on_listExpansion(QQListViewGroup *group);

    void onChildDidSelected(QQListViewChild *child);

    void on_popmenuWillShow(QQPopMenuWidget *widget, QMenu *menu);

    void on_cellRightCicked(QQCell *cell);

private:
    QList<QQCell *> cells;                      //所有的数据
    QMenu *mGroupMenu;
    QMenu *mChildMenu;

    QQCell *m_nRightClickCell;
public:
    void insertQQCell(QQCell * cell);
    void removeQQCell(QQCell * cell);

    void updateCellStatus(int userId, QString state);

    void upload();
    QQCell *getGroupForName(QString *groupName);

    void setGroupPopMenu(QMenu *menu);
    void setChildPopMenu(QMenu *menu);

    QList<QQCell *> getCells() const;

private:
    void setBackGroundColor(QWidget *widget,int index);

    void AddChildItem(QQCell *cell);
};

#endif // QQLISTWIDGET_H
