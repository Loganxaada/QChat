#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include<QStackedWidget>
#include<QPropertyAnimation>

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimationStackedWidget(QWidget *parent=nullptr);
    virtual ~AnimationStackedWidget();

    enum AnimationType
    {
        TopToBottom,    ///Animation from top to bottom
        BottomToTop,    ///Animation from bottom to top
        LeftToRight,    ///Animation from left to right
        RightToLeft     ///Animation from right to left
    };


    static QString getClassName();


    void start(int index);

    void setIndex(int previous, int current);

    void setLength(int length, AnimationType type);


    void setDuration(int duration);

    int getDuration() const;


private Q_SLOTS:
    void on_valueChanged(const QVariant &value);

    void on_animationFinished();


protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void renderPreviousWidget(QPainter &painter, QTransform &transform);

    void renderCurrentWidget(QPainter &painter, QTransform &transform);


    bool m_isAnimating;
    float m_currentValue;
    int m_currentIndex, m_previousIndex;
    AnimationType m_type;
    QPropertyAnimation *m_animation;

};

#endif // ANIMATIONSTACKEDWIDGET_H
