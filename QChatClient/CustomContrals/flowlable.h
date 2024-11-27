#ifndef FLOWLABLE_H
#define FLOWLABLE_H

#include <QLabel>
#include<QPainter>

class FlowLable : public QLabel
{
    Q_OBJECT
public:
    explicit FlowLable(const QString text,QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
signals:

private slots:
    void updateTextEffect();
private:
    QString m_text; //show text
    int m_offset; //color offset


};

#endif // FLOWLABLE_H
