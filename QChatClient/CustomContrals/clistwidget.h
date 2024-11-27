#ifndef CLISTWIDGET_H
#define CLISTWIDGET_H


#include <QListWidget>
#include <QList>
#include <QHash>
#include <QMenu>

//#include "qqcell.h"
//#include "qqlistviewgroup.h"
//#include "qqlistviewchild.h"

class CListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CListWidget(QWidget *parent = 0);


};


#endif // CLISTWIDGET_H
