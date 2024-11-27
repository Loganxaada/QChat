#include "qqlistviewchild.h"
#include "ComAPI/MyUnit.h"
#include "ComAPI/CHelper.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>

// 构造函数，初始化成员变量
QQListViewChild::QQListViewChild(QQPopMenuWidget *parent) :
    QQPopMenuWidget(parent)
{
    m_bEntered = false;  // 鼠标是否进入控件区域
    setMinimumSize(90,90);
}

// 析构函数
QQListViewChild::~QQListViewChild()
{
}

// 处理鼠标按下事件
void QQListViewChild::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)  // 如果按下的是左键
    {
        emit onChildDidSelected(this);  // 发送子项被选择的信号
    }
    else if (Qt::RightButton == ev->button()) {  // 如果按下的是右键
        if (NULL == this->cell) return;  // 如果没有cell对象，直接返回
        Q_EMIT signalRightClicked(this->cell);  // 发送右键点击的信号

        QQPopMenuWidget::mousePressEvent(ev);  // 调用父类的右键按下事件
    }
}

// 处理鼠标双击事件
void QQListViewChild::mouseDoubleClickEvent(QMouseEvent * /*event*/)
{
    emit sig_childDoubleClicked(this->cell);  // 发送子项双击的信号
}

// 处理鼠标进入事件
void QQListViewChild::enterEvent(QEvent *event)
{
    m_bEntered = true;  // 标记鼠标已进入控件区域
    update();  // 更新控件的显示
    QWidget::enterEvent(event);  // 调用父类的进入事件
}

// 处理鼠标离开事件
void QQListViewChild::leaveEvent(QEvent *event)
{
    m_bEntered = false;  // 标记鼠标已离开控件区域
    update();  // 更新控件的显示
    QWidget::leaveEvent(event);  // 调用父类的离开事件
}

// 绘制控件的显示内容
void QQListViewChild::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 开启抗锯齿

    // 控件背景色，根据鼠标是否进入控件区域，设置背景色
    painter.fillRect(this->rect(), m_bEntered ? QColor("#D9D8D7") : QColor("transparent"));

    QPixmap pixmap(cell->m_iconPath);  // 加载头像
    if (pixmap.isNull()) pixmap = QPixmap(":/resource/head/1.bmp");  // 如果头像路径为空，使用默认头像

    // 如果当前是离线状态，绘制灰度图
    if (OFFLINE == cell->status) {
        pixmap = CHelper::ChangeGrayPixmap(pixmap.toImage());
    }

    // 增大头像的绘制区域
    int iconSize = 80;  // 增大头像的大小
    painter.drawPixmap(10, 10, iconSize, iconSize, pixmap);  // 绘制头像

    int w = this->rect().width();
    int h = this->rect().height();

    QFont font("微软雅黑", 14);
    QPen pen;

    // 增大名字文本的字体和绘制区域
    pen.setColor(QColor("#000000"));
    font.setPixelSize(30);  // 增大字体大小
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(100, 0, w - 100, h / 2 - 2, Qt::AlignBottom, cell->name);

    // 增大副标题文本的字体和绘制区域
    pen.setColor(QColor("#666666"));
    font.setPixelSize(25);  // 增大字体大小
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(100, h / 2 + 2, w - 100, h / 2 - 2, Qt::AlignTop, cell->m_subTitle);
}
