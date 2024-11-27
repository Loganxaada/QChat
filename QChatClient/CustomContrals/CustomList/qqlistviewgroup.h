#ifndef QQLISTVIEWGROUP_H
#define QQLISTVIEWGROUP_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>

#include "qqcell.h"
#include "qqpopmenuwidget.h"

class QQListViewGroup : public QQPopMenuWidget
{
    Q_OBJECT

public:
    explicit QQListViewGroup(QQPopMenuWidget *parent = 0);
    ~QQListViewGroup();

public:
    void setQQCell(QQCell *c);

signals:
    void sig_listExpansion(QQListViewGroup *group);

protected:
    void mousePressEvent(QMouseEvent *ev);

public:
    QLabel *m_groupIconLabel=nullptr;
    QLabel *m_groupTitleLabel=nullptr;
    QLabel *m_groupSubTitleLabel=nullptr;
private:
    QQCell *m_cell=nullptr;
};

#endif // QQLISTVIEWGROUP_H
