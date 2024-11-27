#ifndef ROTATINGSTACKEDWIDGET_H
#define ROTATINGSTACKEDWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QStackedWidget>

class RotatingStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY( float rotateVal READ rotateVal WRITE setRotateVal)
public:

    explicit RotatingStackedWidget(QWidget *parent = 0);
    ~RotatingStackedWidget();

    void paintEvent(QPaintEvent *event);

public slots:
    void nextPage();

public:


    void rotate(int index);


    float rotateVal();


    void setRotateVal(float fl);

private slots:

    void valChanged(QVariant);


    void animDone();

private:  
    float m_rotateVal;

    bool m_bAnimating;

    int nextIndex;
};

#endif // ROTATINGSTACKEDWIDGET_H
