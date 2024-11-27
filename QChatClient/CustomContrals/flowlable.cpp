#include "flowlable.h"
#include<QTimer>

FlowLable::FlowLable(const QString text,QWidget *parent)
    :QLabel(parent)
    ,m_text(text)
    ,m_offset(0)
{
    // set font
    QFont font("Consolas", 20, QFont::Bold);
    setFont(font);

    // start timer
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FlowLable::updateTextEffect);
    timer->start(100); // update per 100ms

    // set default size and pure background
//    setFixedSize(400, 100);
    setAttribute(Qt::WA_TranslucentBackground);
}

void FlowLable::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    QFont font("Consolas", 20, QFont::Bold);
    painter.setFont(font);

    QFontMetrics metrics(font);
    int x = 0;
    int y = (height() + metrics.ascent() - metrics.descent()) / 2;

    QColor color;
    for (int i = 0; i < m_text.size(); i++)
    {
        // set (H)、 (S)、 (V)
        int nIndex = (m_offset + i) % 16;
        color.setHsv((15 - nIndex) * 16, 255, 191);
        painter.setPen(color);

        // Drawing a single character
        painter.drawText(x, y, QString(m_text[i]));

        // Calculate the x coordinate starting point of the next character
        x += metrics.width(m_text[i]);
    }
}

void FlowLable::updateTextEffect()
{
    m_offset++; // Controlling the color shift
    update();
}
