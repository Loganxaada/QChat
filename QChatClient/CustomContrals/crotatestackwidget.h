#ifndef CROTATESTACKWIDGET_H
#define CROTATESTACKWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QStackedWidget>

class CRotateStackWidget :public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY( float rotateVal READ rotateVal WRITE setRotateVal)
public:
    explicit CRotateStackWidget(QWidget *parent=nullptr);
    ~CRotateStackWidget();

    void paintEvent(QPaintEvent *event) override;

    void rotate(int index);

    float rotateVal();

    void setRotateVal(float fl);

public slots:
    void nextPage();
private:

    float m_rotateVal; // rotate val

    bool m_bAnimating; //is rotating?

    int nextIndex; // stacked widget next index

private slots:
    void valChanged(QVariant);

    void animationDone();
};

#endif // CROTATESTACKWIDGET_H
