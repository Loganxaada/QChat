#include "rotatingstackedwidget.h"

#include <QVariant>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QTransform>

RotatingStackedWidget::RotatingStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    m_rotateVal = 0;
    m_bAnimating = false;
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

RotatingStackedWidget::~RotatingStackedWidget()
{

}

void RotatingStackedWidget::paintEvent(QPaintEvent *event)
{
    if (m_bAnimating)
    {
        if (m_rotateVal > 90)
        {
            QPixmap pixmap(widget(nextIndex)->size());
            widget(nextIndex)->render(&pixmap);
            QPainter painter(this);

            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(m_rotateVal + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, pixmap);
        }
        else
        {
            QPixmap pixmap(currentWidget()->size());
            currentWidget()->render(&pixmap);
            QPainter painter(this);

            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(m_rotateVal, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, pixmap);
        }
    }
    else {
        QWidget::paintEvent(event);
    }
}


void RotatingStackedWidget::nextPage()
{
    rotate((currentIndex() + 1) >= count() ? 0 : (currentIndex() + 1));
}

void RotatingStackedWidget::rotate(int index)
{
    if(m_bAnimating) return;
    nextIndex = index;
    int offsetx=frameRect().width();
    int offsety=frameRect().height();
    widget(index)->setGeometry ( 0,  0, offsetx, offsety );

    QPropertyAnimation *animnow = new QPropertyAnimation(this,"rotateVal");
    animnow->setDuration(500);
    animnow->setEasingCurve(QEasingCurve::Linear);
    animnow->setStartValue(0);
    animnow->setEndValue(180);
    connect(animnow, SIGNAL(valueChanged(QVariant)), this, SLOT(valChanged(QVariant)));
    connect(animnow, SIGNAL(finished()), this, SLOT(animDone()));
    currentWidget()->hide();
    m_bAnimating = true;
    animnow->start();
}

float RotatingStackedWidget::rotateVal()
{
    return m_rotateVal;
}

void RotatingStackedWidget::setRotateVal(float fl)
{
    m_rotateVal = fl;
}

void RotatingStackedWidget::valChanged(QVariant)
{
    repaint();
}

void RotatingStackedWidget::animDone()
{
    m_rotateVal = 0;
    m_bAnimating = false;
    widget(nextIndex)->show();
    widget(nextIndex)->raise();;
    setCurrentWidget(widget(nextIndex));
    repaint();
}
