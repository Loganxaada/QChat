#ifndef CFRAMELESSWIDGET_H
#define CFRAMELESSWIDGET_H

#include <QWidget>
#include<QMouseEvent>
#include<QDebug>

enum WidgetRegion
{
    UNKNOWN = 0,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    INNER
};

class CFramelessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CFramelessWidget(QWidget *parent = nullptr);
    ~CFramelessWidget();
protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    WidgetRegion hitTest(const QPoint &pos);

    void updateCursorShape(WidgetRegion region);

private:
    bool m_mousePressed;
    QPoint m_startPos;
    QRect m_startGeometry;
    WidgetRegion m_regionPressed;

    QTimer *m_timer;
    QPoint m_lastDelta;
};

#endif // CFRAMELESSWIDGET_H
