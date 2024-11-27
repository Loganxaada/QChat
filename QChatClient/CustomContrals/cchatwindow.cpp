#include "cchatwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QTableView>
#include <QSpacerItem>
#include<QFormLayout>
#include<QToolTip>
#include<QHeaderView>
#include"ComAPI/global.h"
#include"ComAPI/iteminfo.h"
#include<QDebug>
CChatWindow::CChatWindow(QWidget *parent) :
    CustomMoveWidget(parent)
{
    m_cell = new QQCell();
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Chat Window");
    setWindowIcon(QIcon(":/resource/chat.png"));
    setMinimumSize(800, 600);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(0);

    // title bar
    m_titleBar = new QWidget(this);
    m_titleBar->setStyleSheet("background-color: #00E5EE;");
    QHBoxLayout *titleLayout = new QHBoxLayout(m_titleBar);
    titleLayout->setContentsMargins(10, 0, 0, 0);

    QLabel *labelWinIcon = new QLabel(m_titleBar);
    labelWinIcon->setFixedSize(50, 50);
    labelWinIcon->setPixmap(QPixmap(":/resource/chat.png").scaled(30, 30));

    m_labelWinTitle = new QLabel("Chat with xxx", m_titleBar);
    m_labelWinTitle->setStyleSheet("color: #000000;");

    QPushButton *btnWinMin = new QPushButton(QIcon(":/resource/icon-mininize.png"), "", m_titleBar);
    btnWinMin->setFixedSize(50, 50);
    btnWinMin->setFocusPolicy(Qt::NoFocus);
    connect(btnWinMin, &QPushButton::clicked, this, &CChatWindow::showMinimized);

    QPushButton *btnWinMax = new QPushButton(QIcon(":/resource/maxed.png"), "", m_titleBar);
    btnWinMax->setFixedSize(50, 50);
    btnWinMax->setFocusPolicy(Qt::NoFocus);
    connect(btnWinMax, &QPushButton::clicked, this, &QWidget::showMaximized);

    QPushButton *btnWinClose = new QPushButton(QIcon(":/resource/icon-close.png"), "", m_titleBar);
    btnWinClose->setFixedSize(50, 50);
    btnWinClose->setFocusPolicy(Qt::NoFocus);
    connect(btnWinClose, &QPushButton::clicked, this, &CChatWindow::on_closeBtn_clicked);

    titleLayout->addWidget(labelWinIcon);
    titleLayout->addWidget(m_labelWinTitle);
    titleLayout->addStretch();
    titleLayout->addWidget(btnWinMin);
    titleLayout->addWidget(btnWinMax);
    titleLayout->addWidget(btnWinClose);
    m_titleBar->setLayout(titleLayout);
    m_mainLayout->addWidget(m_titleBar,1);

    // main chat lay
    QHBoxLayout *chatLayout = new QHBoxLayout;
    chatLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    m_widgetBubble = new BubbleList(this);
    m_widgetBubble->setMinimumSize(400, 400);
    m_widgetBubble->setStyleSheet("border: 1px solid #cccccc;");
    leftLayout->addWidget(m_widgetBubble,10);


    QWidget *widgetSetting = new QWidget(this);
    //widgetSetting->setStyleSheet("border: 1px solid #cccccc;");
    QHBoxLayout *settingLayout = new QHBoxLayout(widgetSetting);

    QToolButton *btnFont = new QToolButton(widgetSetting);
    btnFont->setToolTip("Modify chat font");
    btnFont->setIcon(QIcon(":/resource/font.png"));

    QToolButton *btnEmoji = new QToolButton(widgetSetting);
    btnEmoji->setToolTip("Insert emoticons");
    btnEmoji->setIcon(QIcon(":/resource/emoji.png"));

    QToolButton *btnColor = new QToolButton(widgetSetting);
    btnColor->setToolTip("Set the font color");
    btnColor->setIcon(QIcon(":/resource/color.png"));

    QToolButton *btnSendFile = new QToolButton(widgetSetting);
    btnSendFile->setToolTip("Send File");
    btnSendFile->setIcon(QIcon(":/resource/file.png"));

    QToolButton *btnSendImage = new QToolButton(widgetSetting);
    btnSendImage->setToolTip("Send picture");
    btnSendImage->setIcon(QIcon(":/resource/image.png"));

    QToolButton *btnScreenshot = new QToolButton(widgetSetting);
    btnScreenshot->setToolTip("screenshot");
    btnScreenshot->setIcon(QIcon(":/resource/screenshot.png"));

    QToolButton *btnChatHistory = new QToolButton(widgetSetting);
    btnChatHistory->setToolTip("Chat History");
    btnChatHistory->setIcon(QIcon(":/resource/chat_record.png"));

    settingLayout->addWidget(btnFont);
    settingLayout->addWidget(btnEmoji);
    settingLayout->addWidget(btnColor);
    settingLayout->addWidget(btnSendFile);
    settingLayout->addWidget(btnSendImage);
    settingLayout->addWidget(btnScreenshot);
    settingLayout->addSpacerItem(new QSpacerItem(396, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    settingLayout->addWidget(btnChatHistory);

    leftLayout->addWidget(widgetSetting,1);

    m_widgetMsg = new QWidget(this);
    m_widgetMsg->setMinimumHeight(100);
    m_widgetMsg->setStyleSheet("border: 1px solid #cccccc");
    QVBoxLayout *msgLayout = new QVBoxLayout(m_widgetMsg);
    msgLayout->setContentsMargins(0, 0, 0, 0);

    m_textEditMsg = new QTextEdit();
    m_textEditMsg->setMinimumHeight(100);
    msgLayout->addWidget(m_textEditMsg);

    QHBoxLayout *msgActionLayout = new QHBoxLayout;
    QLabel *labelCharCount = new QLabel("Character Limit: 0 / 255", m_widgetMsg);
    labelCharCount->setMaximumHeight(60);
    msgActionLayout->addWidget(labelCharCount);
    msgActionLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *btnClose = new QPushButton("Close(&C)", m_widgetMsg);
    btnClose->setToolTip("Close current window");
    connect(btnClose, &QPushButton::clicked, this, &QWidget::close);
    msgActionLayout->addWidget(btnClose);

    m_btnSendMsg = new QPushButton("Send(&S)", m_widgetMsg);
    m_btnSendMsg->setToolTip("Send chat message");
    msgActionLayout->addWidget(m_btnSendMsg);

    msgLayout->addLayout(msgActionLayout);
    msgLayout->setContentsMargins(5,5,5,5);
    leftLayout->addWidget(m_widgetMsg,5);

    chatLayout->addLayout(leftLayout);

    // 文件传输区域
    m_widgetFileBoard = new QWidget(this);
    m_widgetFileBoard->setStyleSheet("QLineEdit {border: none; background-color: #05CE26; color: #FFFFFF;}"
                                     "QProgressBar {border: 1px solid grey; color: #FFFFFF;}"
                                     "QProgressBar::chunk {background-color: #05B8CC;}");

    QVBoxLayout *fileBoardLayout = new QVBoxLayout(m_widgetFileBoard);

    m_widgetFileInfo = new QWidget(m_widgetFileBoard);
    QVBoxLayout *fileInfoLayout = new QVBoxLayout(m_widgetFileInfo);
    fileInfoLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *labelFileTransfer = new QLabel("File Transfer：", m_widgetFileInfo);
    QProgressBar *progressBar = new QProgressBar(m_widgetFileInfo);
    progressBar->setMaximumHeight(15);

    fileInfoLayout->addWidget(labelFileTransfer);
    fileInfoLayout->addWidget(progressBar);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("总大小:", m_widgetFileInfo), new QLineEdit("0 KB", m_widgetFileInfo));
    formLayout->addRow(new QLabel("已传输：", m_widgetFileInfo), new QLineEdit("0 KB", m_widgetFileInfo));
    formLayout->addRow(new QLabel("速率：", m_widgetFileInfo), new QLineEdit("10 M/S", m_widgetFileInfo));
    fileInfoLayout->addLayout(formLayout);

    fileBoardLayout->addWidget(m_widgetFileInfo,1);
    fileBoardLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    m_tableViewGroups = new QTableView(m_widgetFileBoard);
    fileBoardLayout->addWidget(m_tableViewGroups,8);

    chatLayout->addWidget(m_widgetFileBoard);
    m_mainLayout->addLayout(chatLayout,20);

    m_widgetFileBoard->setVisible(false);

    m_model = new QStandardItemModel(this);
    m_model->setRowCount(3);
    m_tableViewGroups->setModel(m_model);



    connect(m_btnSendMsg,&QPushButton::clicked,this,&CChatWindow::on_btnSendMsg_clicked);
}

CChatWindow::~CChatWindow()
{
}

void CChatWindow::setCell(QQCell *cell, const quint8 &type)
{
    m_cell = cell;
    m_labelWinTitle->setText(QString("Chat with %1").arg(cell->name));
    this->setWindowTitle(QString("Chat with %1").arg(cell->name));

    m_nChatType = type;

    if (0 == type) {
        // TODO
        // load history...
        //m_widgetBubble->addItems(DataBaseMagr::Instance()->QueryHistory(m_cell->id, 10));
        //        ui->tableViewGroups->setVisible(false);
        //        ui->widgetFileInfo->setVisible(true);
        //        ui->widgetFileBoard->setVisible(false);
        //        // 链接文件服务器,方便下载文件
        //        m_tcpFileSocket->ConnectToServer(MyApp::m_strHostAddr, MyApp::m_nFilePort, m_cell->id);
        m_tableViewGroups->setVisible(false);
        m_widgetFileInfo->setVisible(true);
        m_widgetFileBoard->setVisible(false);

    }
    else {
        m_tableViewGroups->setVisible(true);
        m_widgetFileInfo->setVisible(false);
        m_widgetFileBoard->setVisible(true);


        m_model->setColumnCount(2);
        m_model->setHorizontalHeaderLabels(QStringList() << "user" << "state"<<"role");

        m_tableViewGroups->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableViewGroups->verticalHeader()->setVisible(false);
        m_tableViewGroups->setMinimumWidth(300);
    }
}

void CChatWindow::setLabelWinTitle(QString title)
{
    m_labelWinTitle->setText(tr("Chatting with %1").arg(title));
}

int CChatWindow::getUserId() const
{
    return m_cell->id;
}

void CChatWindow::addMessage(json js)
{
    // 如果有头像，则用自己的头像(群组消息的时候会附带头像图片)
    //    strHead = GetHeadPixmap(strHead);

    //"{\"id\":3,\"msg\":\"adada\",\"msgid\":6,\"name\":\"xbw\",\"time\":\"2024-11-24 17:18:35\",\"toid\":1}"

    int msgid = js["msgid"].get<int>();


    QString fromname = QString::fromStdString(js["name"]);


    QString msg = QString::fromStdString(js["msg"]);
    QString time = QString::fromStdString(js["time"]);

    ItemInfo *itemInfo = new ItemInfo();
    itemInfo->setName(fromname);
    itemInfo->setDatetime(time);
    itemInfo->setHeadPixmap(":/resource/4.bmp");
    itemInfo->setMsgType(TEXT);
    itemInfo->setText(msg);

    // if recved files?

    m_widgetBubble->addItem(itemInfo);
    // 群组的聊天消息不保存
    if (0 != m_nChatType)
        return;

    // 添加聊天消息到历史记录
    //DataBaseMagr::Instance()->AddHistoryMsg(m_cell->id, itemInfo);

}

void CChatWindow::getGroupusers(vector<GroupUser> groupusers)
{
    m_groupusers = groupusers;
    for(GroupUser &groupuser: m_groupusers)
    {
        QString name = QString::fromStdString(groupuser.getName());
        QString state = QString::fromStdString(groupuser.getState());
        QString role = QString::fromStdString(groupuser.getRole());

        QStandardItem *item1 = new QStandardItem(name);
        QStandardItem *item2 = new QStandardItem(state);
        QStandardItem *item3 = new QStandardItem(role);

        m_model->setItem(m_rowCount,0,item1);
        m_model->setItem(m_rowCount,1,item2);
        m_model->setItem(m_rowCount,2,item3);
        m_rowCount++;
    }
}

void CChatWindow::on_btnSendMsg_clicked()
{
    QString text = m_textEditMsg->toPlainText();
    // delete last \n
    while (text.endsWith("\n")) {
        text.remove(text.length() - 2, 2);
    }

    if (text.isEmpty()) {
        QPoint point = m_widgetMsg->mapToGlobal(m_btnSendMsg->pos());

        QToolTip::showText(point, tr("The message content cannot be empty, please re-enter!"));
        return;
    }


    json js;

    if(0 == m_nChatType)
    {
        js["msgid"] = ONE_CHAT_MSG;
        js["id"] =Global::m_currentUser.getId();
        js["name"] = Global::m_currentUser.getName();
        js["toid"] = m_cell->id;
        js["msg"] = text.toStdString();
        js["time"] = Global::getCurrentTime();
    }else{
        js["msgid"] = GROUP_CHAT_MSG;
        js["id"] = Global::m_currentUser.getId();
        js["name"] = Global::m_currentUser.getName();
        js["groupid"] = m_cell->id;
        js["msg"] = text.toStdString();
        js["time"] = Global::getCurrentTime();
    }

    emit sig_sendMessage(js);

    // chat bubble item
    ItemInfo *itemInfo = new ItemInfo();
    itemInfo->setName(QString::fromStdString(Global::m_currentUser.getName()));
    itemInfo->setDatetime(QString::fromStdString(Global::getCurrentTime()));
    itemInfo->setHeadPixmap(":/resource/3.bmp");
    itemInfo->setText(text);
    itemInfo->setOrientation(Right);


    m_widgetBubble->addItem(itemInfo);    // add bubble
    m_textEditMsg->clear();

    // 群组消息不记录
    if (0 != m_nChatType)
        return;
    // 保存消息记录到数据库
    //    DataBaseMagr::Instance()->AddHistoryMsg(m_cell->id, itemInfo);
}

void CChatWindow::on_closeBtn_clicked()
{
    emit sig_close();
    QTimer::singleShot(100, this, &CChatWindow::close);
}
