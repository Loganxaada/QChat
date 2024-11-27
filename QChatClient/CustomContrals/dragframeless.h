#ifndef DRAGFRAMELESS_H
#define DRAGFRAMELESS_H

#include <QWidget>
#include<QMouseEvent>

enum WidgetRegion
{
    Unknown = 0,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Inner
};

class DragFrameless : public QWidget
{
    Q_OBJECT

public:
    explicit DragFrameless(QWidget *parent = nullptr);
    ~DragFrameless();
protected:
    void mousePressEvent(QMouseEvent *event) override;


    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            m_mousePressed = false;
            m_regionPressed = Unknown;
        }
        QWidget::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (!m_mousePressed)
        {
            WidgetRegion region = hitTest(event->pos());
            updateCursorShape(region);
        }
        else
        {
            QPoint delta = event->globalPos() - m_startPos;
            QRect newGeometry = m_startGeometry;

            switch (m_regionPressed)
            {
            case Top:
                newGeometry.setTop(m_startGeometry.top() + delta.y());
                break;
            case Bottom:
                newGeometry.setBottom(m_startGeometry.bottom() + delta.y());
                break;
            case Left:
                newGeometry.setLeft(m_startGeometry.left() + delta.x());
                break;
            case Right:
                newGeometry.setRight(m_startGeometry.right() + delta.x());
                break;
            case TopLeft:
                newGeometry.setTopLeft(m_startGeometry.topLeft() + delta);
                break;
            case TopRight:
                newGeometry.setTopRight(m_startGeometry.topRight() + delta);
                break;
            case BottomLeft:
                newGeometry.setBottomLeft(m_startGeometry.bottomLeft() + delta);
                break;
            case BottomRight:
                newGeometry.setBottomRight(m_startGeometry.bottomRight() + delta);
                break;
            case Inner:
                // 修复移动逻辑
                move(m_startGeometry.topLeft() + delta);
                break;
            default:
                break;
            }

            if (m_regionPressed != Inner)
            {
                setGeometry(newGeometry);
            }
        }
        QWidget::mouseMoveEvent(event);
    }

private:


    WidgetRegion hitTest(const QPoint &pos)
    {
        const int borderWidth = 5;
        QRect rect = this->rect();

        if (pos.y() <= borderWidth)
        {
            if (pos.x() <= borderWidth) return TopLeft;
            if (pos.x() >= rect.width() - borderWidth) return TopRight;
            return Top;
        }
        if (pos.y() >= rect.height() - borderWidth)
        {
            if (pos.x() <= borderWidth) return BottomLeft;
            if (pos.x() >= rect.width() - borderWidth) return BottomRight;
            return Bottom;
        }
        if (pos.x() <= borderWidth) return Left;
        if (pos.x() >= rect.width() - borderWidth) return Right;

        return Inner;
    }

    void updateCursorShape(WidgetRegion region)
    {
        switch (region)
        {
        case Top:
        case Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case Left:
        case Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case TopLeft:
        case BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case TopRight:
        case BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    }

private:
    bool m_mousePressed=false;
    QPoint m_startPos;
    QRect m_startGeometry;
    WidgetRegion m_regionPressed;
};

#endif // DRAGFRAMELESS_H
