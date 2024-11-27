#include "qqlistwidget.h"
#include "ComAPI/MyUnit.h"

#include <QDebug>
#include<QLabel>

QQListWidget::QQListWidget(QWidget *parent) :
    QListWidget(parent)
{
    m_nRightClickCell = NULL;
}

QQCell *QQListWidget::getRightClickedCell()
{
    return m_nRightClickCell;
}

void QQListWidget::setGroupPopMenu(QMenu *menu)
{
    mGroupMenu = menu;
}

void QQListWidget::setChildPopMenu(QMenu *menu)
{
    mChildMenu = menu;
}

void QQListWidget::insertQQCell(QQCell * cell)
{
    if (cell->type == QQCELLTYPE_GROUP || cell->type == QQCELLTYPE_GROUPEX)
    {
        cells.append(cell);
    }
    else
    {
        foreach (QQCell *group, cells)
        {
            if (!group->groupName.compare(cell->groupName))
            {
                group->childs.append(cell);

                break;
            }
        }
    }

    upload();
}

void QQListWidget::removeQQCell(QQCell * cell)
{

    // 如果是分组类型，直接从cells中移除
    if (cell->type == QQCELLTYPE_GROUP || cell->type == QQCELLTYPE_GROUPEX)
    {
        // 删除所有子项
        for (QQCell *child : cell->childs) {
            removeQQCell(child);  // 递归删除子项
        }
        cells.removeOne(cell);  // 删除分组
    }
    else
    {
        // 如果是联系人，直接从其所在的分组中移除
        for (QQCell *group : cells)
        {
            if (!group->groupName.compare(cell->groupName))
            {
                group->childs.removeOne(cell);  // 从分组中删除联系人
                break;
            }
        }
    }

    upload();  // 更新列表
}

void QQListWidget::updateCellStatus(int userId, QString state)
{
    qDebug()<<__FUNCTION__;
    for (QQCell *group : cells)  // 遍历分组
    {
        for (QQCell *child : group->childs)  // 遍历分组中的好友
        {
            if (child->id == userId)  // 找到目标好友
            {
                child->status = state == "offline"?OFFLINE:ONLINE; // 更新好友状态

                child->m_subTitle = state +" | "+child->m_signature;
                upload();  // 刷新列表显示
                return;
            }
        }
    }
}

void QQListWidget::upload()
{
    this->clear();
    // update list
    for (QQCell * cell:cells)
    {
        if (cell->type == QQCELLTYPE_GROUP || cell->type == QQCELLTYPE_GROUPEX )
        {
            QQListViewGroup *group = new QQListViewGroup();
            group->setGeometry(0, 0, 300, 30);
            if (cell->type == QQCELLTYPE_GROUP)
            {
                // 先获取在线好友
                int nOnlineCnt = 0;
                for (QQCell * child: cell->childs)
                {
                    if (ONLINE == child->status) {
                        nOnlineCnt ++;
                    }
                }

                group->m_groupTitleLabel->setText(cell->groupName);
                group->m_groupSubTitleLabel->setText(QString("[%1/%2]").arg(nOnlineCnt).arg(cell->childs.size()));
            }
            else
            {
                group->m_groupTitleLabel->setText(QString("%1[%2/%2]").arg(cell->groupName).arg(cell->childs.size()));
                group->m_groupSubTitleLabel->setText(QString(""));
            }
            group->setQQCell(cell);
            group->setPopMenu(mGroupMenu);


            connect(group,&QQListViewGroup::sig_listExpansion,this,&QQListWidget::on_listExpansion);

            connect(group, &QQListViewGroup::onpopmenuwillshow,this, &QQListWidget::on_popmenuWillShow);

            QListWidgetItem * item = new QListWidgetItem("");
            this->addItem(item);
            this->setItemWidget(item, group);
            item->setSizeHint(group->geometry().size());

            if (cell->m_bOpen)
            {
                for (QQCell * ccell:cell->childs)
                {
                    if (ONLINE == ccell->status)
                        AddChildItem(ccell);
                }

                for (QQCell * ccell:cell->childs)
                {
                    if (OFFLINE == ccell->status)
                        AddChildItem(ccell);
                }
            }
        }
    }
}

void QQListWidget::on_listExpansion(QQListViewGroup *)
{
    upload();
}

void QQListWidget::onChildDidSelected(QQListViewChild *child)
{
    //    qDebug() << "clicked" << child->cell->name;
    Q_UNUSED(child);
}

//void QQListWidget::sig_childDoubleClicked(QQCell *cell)
//{

//}

void QQListWidget::setBackGroundColor(QWidget *widget,int index)
{
    widget->setStyleSheet(index % 2 ? "QWidget{background:rgb(100, 100, 100, 100)}":"QWidget{background:rgb(255, 255, 255)}");
}

void QQListWidget::AddChildItem(QQCell *cell)
{
    QQListViewChild *child = new QQListViewChild;
    child->setGeometry(0, 0, 300, 60);
    child->cell = cell;
    child->setPopMenu(mChildMenu);

    connect(child, SIGNAL(sig_childDoubleClicked(QQCell*)), this, SIGNAL(sig_childDoubleClicked(QQCell*)));
    connect(child, SIGNAL(signalRightClicked(QQCell*)),this, SLOT(on_cellRightCicked(QQCell*)));

    QListWidgetItem * item = new QListWidgetItem("");
    this->addItem(item);
    this->setItemWidget(item, child);
    item->setSizeHint(child->geometry().size());
}

QQCell *QQListWidget::getGroupForName(QString *groupName)
{
    for (QQCell *group : cells)
    {
        if (!group->groupName.compare(groupName))
        {
            return group;
        }
    }

    return nullptr;
}

void QQListWidget::on_popmenuWillShow(QQPopMenuWidget *widget, QMenu *menu)
{
    emit on_popmenu_will_show(this, widget, menu);;
}

void QQListWidget::on_cellRightCicked(QQCell *cell)
{
    m_nRightClickCell = cell;
}

QList<QQCell *> QQListWidget::getCells() const
{
    return cells;
}
