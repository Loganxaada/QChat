#include "cframelesswidget.h"
#include<QTimer>

CFramelessWidget::CFramelessWidget(QWidget *parent) :
    QWidget(parent), m_mousePressed(false)
{
    // 设置无边框
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setMouseTracking(true);

    setMinimumSize(800,600);
}

CFramelessWidget::~CFramelessWidget()
{
}

void CFramelessWidget::mousePressEvent(QMouseEvent *event)
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

void CFramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_mousePressed = false;
        m_regionPressed = UNKNOWN;
    }
    QWidget::mouseReleaseEvent(event);
}

void CFramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!childAt(event->pos()) && hitTest(event->pos()) == INNER)
    {
        setCursor(Qt::ArrowCursor);
    }

    if (childAt(event->pos()))
    {
        setCursor(Qt::ArrowCursor);
        QWidget::mouseMoveEvent(event);
        return;
    }


    if (!m_mousePressed)
    {
        WidgetRegion region = hitTest(event->pos());
        updateCursorShape(region);

        // 当进入Inner区域时，恢复默认的箭头形状
        if (region == INNER|| UNKNOWN)
        {
            setCursor(Qt::ArrowCursor);
        }
    }
    else
    {
        QPoint delta = event->globalPos() - m_startPos;
        QRect newGeometry = m_startGeometry;

        switch (m_regionPressed)
        {
        case TOP:
            newGeometry.setTop(m_startGeometry.top() + delta.y());
            break;
        case BOTTOM:
            newGeometry.setBottom(m_startGeometry.bottom() + delta.y());
            break;
        case LEFT:
            newGeometry.setLeft(m_startGeometry.left() + delta.x());
            break;
        case RIGHT:
            newGeometry.setRight(m_startGeometry.right() + delta.x());
            break;
        case TOP_LEFT:
            newGeometry.setTopLeft(m_startGeometry.topLeft() + delta);
            break;
        case TOP_RIGHT:
            newGeometry.setTopRight(m_startGeometry.topRight() + delta);
            break;
        case BOTTOM_LEFT:
            newGeometry.setBottomLeft(m_startGeometry.bottomLeft() + delta);
            break;
        case BOTTOM_RIGHT:
            newGeometry.setBottomRight(m_startGeometry.bottomRight() + delta);
            break;
        case INNER:
        {
            move(m_startGeometry.topLeft() + delta);

        }
            break;
        default:
            break;
        }

        if (m_regionPressed != INNER)
        {
            setGeometry(newGeometry);
        }
    }
    QWidget::mouseMoveEvent(event);
}

WidgetRegion CFramelessWidget::hitTest(const QPoint &pos)
{
    const int borderWidth = 5;
    QRect rect = this->rect();

    if (pos.y() <= borderWidth)
    {
        if (pos.x() <= borderWidth) return TOP_LEFT;
        if (pos.x() >= rect.width() - borderWidth) return TOP_RIGHT;
        return TOP;
    }
    if (pos.y() >= rect.height() - borderWidth)
    {
        if (pos.x() <= borderWidth) return BOTTOM_LEFT;
        if (pos.x() >= rect.width() - borderWidth) return BOTTOM_RIGHT;
        return BOTTOM;
    }
    if (pos.x() <= borderWidth) return LEFT;
    if (pos.x() >= rect.width() - borderWidth) return RIGHT;

    return INNER;
}

void CFramelessWidget::updateCursorShape(WidgetRegion region)
{
    switch (region)
    {
    case TOP:
    case BOTTOM:
        setCursor(Qt::SizeVerCursor);
        break;
    case LEFT:
    case RIGHT:
        setCursor(Qt::SizeHorCursor);
        break;
    case TOP_LEFT:
    case BOTTOM_RIGHT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOP_RIGHT:
    case BOTTOM_LEFT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}
