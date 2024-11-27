#include "dragframeless.h"

DragFramelessWidget::DragFrameless(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setMouseTracking(true);

    setMinimumSize(800,600);
}

DragFrameless::~DragFrameless()
{
}

void DragFrameless::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        m_mousePressed = true;
        m_startPos = event->globalPos();
        m_startGeometry = geometry();
        m_regionPressed = hitTest(event->pos());
    }
    QWidget::mousePressEvent(event);

}
