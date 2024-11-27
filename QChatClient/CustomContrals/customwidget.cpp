#include "customwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include<QTimer>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_Hover);
}

void CustomWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

CustomMoveWidget::CustomMoveWidget(QWidget *parent)
    : CustomWidget(parent), m_mousePressed(false)
{
#ifdef Q_OS_WIN32
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
#else
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    setAttribute(Qt::WA_Hover);

    setMouseTracking(true);


}

CustomMoveWidget::~CustomMoveWidget() {}




void CustomMoveWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();

        if ("MainWindow" == this->objectName()) {
            QPoint pos = e->globalPos() - mousePoint;
//            MyApp::m_nWinX = pos.x();
//            MyApp::m_nWinY = pos.y();
        }
    }
}

void CustomMoveWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void CustomMoveWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;  
}


