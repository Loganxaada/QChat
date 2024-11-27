#include "qqlistviewgroup.h"
#include<QHBoxLayout>
#include<QLabel>

QQListViewGroup::QQListViewGroup(QQPopMenuWidget *parent) :
    QQPopMenuWidget(parent)
{
    // 设置窗口的初始几何属性
    setGeometry(0, 0, 270, 80);
    setWindowTitle("Form");

    // 创建并设置 groupIconLabel
    m_groupIconLabel = new QLabel(this);
    m_groupIconLabel->setText(""); // 设置图标标签的文本为空
    m_groupIconLabel->setFixedSize(20, 20); // 固定图标的大小

    // 创建并设置 groupTitleLabel
    m_groupTitleLabel = new QLabel(this);
    m_groupTitleLabel->setText("Title"); // 设置标题文本

    // 创建并设置 groupSubTitleLabel
    m_groupSubTitleLabel = new QLabel(this);
    m_groupSubTitleLabel->setText("SubTitle"); // 设置副标题文本

    // 创建水平布局并将控件添加到布局中
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5); // 设置布局边距
    layout->setSpacing(10); // 设置控件之间的间距

    layout->addWidget(m_groupIconLabel); // 添加图标标签
    layout->addWidget(m_groupTitleLabel); // 添加标题标签
    layout->addStretch(); // 添加弹性空间，将副标题推到右边
    layout->addWidget(m_groupSubTitleLabel); // 添加副标题标签

    // 将布局设置为当前窗口部件的布局
    setLayout(layout);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    this->setMinimumHeight(50); // 设置窗口部件的最小高度

    m_groupSubTitleLabel->setAlignment(Qt::AlignRight);
}

QQListViewGroup::~QQListViewGroup()
{
}

void QQListViewGroup::setQQCell(QQCell *c)
{
    m_cell = c;
    if (m_cell->m_bOpen)
    {
        m_groupIconLabel->setPixmap(QPixmap(":/resource/angle-down.png").scaled(20,20));
    }
    else
    {
        m_groupIconLabel->setPixmap(QPixmap(":/resource/angle-right.png").scaled(20,20));
    }
}

void QQListViewGroup::mousePressEvent(QMouseEvent *ev)
{
    QQPopMenuWidget::mousePressEvent(ev);

    if (ev->button() != Qt::LeftButton)
    {
        return ;
    }

    if (m_cell == nullptr)
        return;

    m_cell->m_bOpen = !m_cell->m_bOpen;
    if (m_cell->m_bOpen)
    {
        m_groupIconLabel->setPixmap(QPixmap(":/resource/angle-down.png"));
    }
    else
    {
        m_groupIconLabel->setPixmap(QPixmap(":/resource/angle-right.png"));
    }

    emit sig_listExpansion(this);
}
