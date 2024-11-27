#include "mainwindow.h"
#include<QFile>
#include<QApplication>
#include<QMenu>
#include<QInputDialog>
#include"ComAPI/global.h"
#include "ComAPI/json.hpp"
#include<QMessageBox>
#include<QCalendarWidget>
#include"ComAPI/CHelper.h"
using json = nlohmann::json;
Q_DECLARE_METATYPE(json);

MainWindow::MainWindow(CTcpSocket *socket,QWidget *parent)
    :CFramelessWidget(parent)
    ,m_tcpSocket(socket)
{
    initStyleSheet();
    qRegisterMetaType<json>("json");

    setMinimumSize(600,1200);
    setMaximumSize(1200,1800);

    m_stackWidget = new AnimationStackedWidget(this);

    initTop();
    initBottom();

    initTalkWidget();
    initContractsWidget();
    initGroupWidget();
    initAppWidget();

    initListMenu();
    initSysTray();


    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(m_talkToolBtn, 0);
    m_btnGroup->addButton(m_friendToolBtn, 1);
    m_btnGroup->addButton(m_groupToolBtn, 2);
    m_btnGroup->addButton(m_appToolBtn, 3);

    initConnect();

    m_stackWidget->setCurrentIndex(0);

    m_mainLay = new QVBoxLayout(this);
    m_mainLay->addSpacing(5);
    m_mainLay->addWidget(m_topWidget);
    m_mainLay->addSpacing(20);
    m_mainLay->addWidget(m_stackWidget);
    m_mainLay->addWidget(m_bottomWidget);
    m_mainLay->addSpacing(5);

    m_mainLay->setContentsMargins(10, 10, 10, 10);
    //m_mainLay->setSpacing(10);

    setLayout(m_mainLay);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initStyleSheet()
{
    QFile file(":/qss/default.qss");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();
}

void MainWindow::initSysTray()
{
    m_sysTrayIcon = new QSystemTrayIcon(this);
    m_sysTrayIcon->setIcon(QIcon(":/resource/app.png"));

    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction(tr("I'm online"));
    m_trayMenu->addAction("Offline");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Main");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Quit");

    m_sysTrayIcon->setContextMenu(m_trayMenu);

}

void MainWindow::initTop()
{
    m_topWidget = new QWidget(this);

    m_logoLabel = new QLabel(this);

    m_minBtn = new QPushButton(QIcon(":/resource/icon-mininize.png"),"",this);
    m_closeBtn = new QPushButton(QIcon(":/resource/icon-close.png"),"",this);

    m_logoLabel->setPixmap(QPixmap(":/resource/chat.png").scaled(m_minBtn->width()+10, m_minBtn->height()+10,Qt::KeepAspectRatio, Qt::SmoothTransformation));


    QHBoxLayout *topLay = new QHBoxLayout();
    topLay->addWidget(m_logoLabel);
    topLay->addStretch();
    topLay->addWidget(m_minBtn);
    topLay->setSpacing(0);
    topLay->addWidget(m_closeBtn);

    m_headLabel = new QLabel(this);
    m_headLabel->setPixmap(QPixmap(":/resource/head_boy.png").scaled(160,160));

    m_nicknameLabel=new QLabel("zhang san",this);
    QFont font  = m_nicknameLabel->font();
    font.setPointSize(18);
    font.setBold(true);
    m_nicknameLabel->setFont(font);

    m_signatureEdit = new QLineEdit("hello world",this);

    m_weatherLabel = new QLabel(this);
    m_weatherLabel->setPixmap(QPixmap(":/resource/weather.png").scaled(100,100));


    QHBoxLayout *headLay = new QHBoxLayout();

    headLay->addWidget(m_headLabel);

    QVBoxLayout *nicksignLay = new QVBoxLayout();
    nicksignLay->addWidget(m_nicknameLabel);
    nicksignLay->addSpacing(5);
    nicksignLay->addWidget(m_signatureEdit);

    headLay->addSpacing(10);
    headLay->addLayout(nicksignLay);
    headLay->addStretch();
    headLay->addWidget(m_weatherLabel);
    headLay->addSpacing(5);

    QVBoxLayout* topmainLay = new QVBoxLayout(this);
    topmainLay->addLayout(topLay);
    topmainLay->setContentsMargins(5,0,5,0);
    topmainLay->addSpacing(20);
    topmainLay->addLayout(headLay);

    m_topWidget->setLayout(topmainLay);
}

void MainWindow::initTalkWidget()
{
    m_talkWidget = new QWidget(m_stackWidget);

    m_talkLabel = new QLabel(m_talkWidget);


    m_talkTextLabel=new QLabel("Talks",m_talkWidget);

    m_talkLabel->setPixmap(QPixmap(":/resource/talk.png").scaled(60,60));

    QHBoxLayout *talkHeadLay = new QHBoxLayout();
    //talkHeadLay->setSpacing(10);
    talkHeadLay->addWidget(m_talkLabel);
    talkHeadLay->addSpacing(10);
    talkHeadLay->addWidget(m_talkTextLabel);
    talkHeadLay->addStretch();

    m_talkWidget->setLayout(talkHeadLay);

    m_stackWidget->addWidget(m_talkWidget);
    m_stackWidget->setCurrentIndex(0);
}

void MainWindow::initContractsWidget()
{
    m_contractsWidget = new QWidget(m_stackWidget);
    m_friendListWidget = new QQListWidget();

    m_contractsLabel = new QLabel(m_contractsWidget);


    m_contractsTextLabel=new QLabel("Contracts",m_contractsWidget);

    m_contractsLabel->setPixmap(QPixmap(":/resource/friend.png").scaled(60,60));

    QHBoxLayout *contractsHeadLay = new QHBoxLayout();
    //talkHeadLay->setSpacing(10);
    contractsHeadLay->addWidget(m_contractsLabel);
    contractsHeadLay->addSpacing(10);
    contractsHeadLay->addWidget(m_contractsTextLabel);
    contractsHeadLay->addStretch();

    QVBoxLayout *friendLay=new QVBoxLayout();
    friendLay->addLayout(contractsHeadLay);
    friendLay->addWidget(m_friendListWidget);

    m_contractsWidget->setLayout(friendLay);

    m_stackWidget->addWidget(m_contractsWidget);

}

void MainWindow::initGroupWidget()
{
    m_groupListWidget = new QQListWidget();

    m_groupsWidget = new QWidget(m_stackWidget);

    m_groupsLabel = new QLabel(m_groupsWidget);


    m_groupsTextLabel=new QLabel("Groups",m_groupsWidget);

    m_groupsLabel->setPixmap(QPixmap(":/resource/group.png").scaled(60,60));

    QHBoxLayout *groupsHeadLay = new QHBoxLayout();
    groupsHeadLay->addWidget(m_groupsLabel);
    groupsHeadLay->addSpacing(10);
    groupsHeadLay->addWidget(m_groupsTextLabel);
    groupsHeadLay->addStretch();

    QVBoxLayout *groupLay=new QVBoxLayout();
    groupLay->addLayout(groupsHeadLay);
    groupLay->addWidget(m_groupListWidget);

    m_groupsWidget->setLayout(groupLay);

    m_stackWidget->addWidget(m_groupsWidget);
}

void MainWindow::initAppWidget()
{
    m_appWidget = new QWidget(m_stackWidget);

    m_appLabel = new QLabel(m_appWidget);


    m_appTextLabel=new QLabel("APP",m_appWidget);

    m_appLabel->setPixmap(QPixmap(":/resource/applation.png").scaled(60,60));

    QHBoxLayout *appHeadLay = new QHBoxLayout();
    //talkHeadLay->setSpacing(10);
    appHeadLay->addWidget(m_appLabel);
    appHeadLay->addSpacing(10);
    appHeadLay->addWidget(m_appTextLabel);
    appHeadLay->addStretch();

    m_appWidget->setLayout(appHeadLay);

    m_stackWidget->addWidget(m_appWidget);
}

void MainWindow::initListMenu()
{
    //设置菜单
    //设置组菜单
    m_addFriendMenu = new QMenu(this);
    m_addFriendMenu->addAction(tr("添加好友"));
    m_addFriendMenu->addAction(tr("刷新"));
    m_addFriendMenu->addSeparator();
    m_addFriendMenu->addAction(tr("Delete this group"));

    m_friendListWidget->setGroupPopMenu(m_addFriendMenu);

    // 设置子菜单
    m_childMenu = new QMenu(this);
    m_childMenu->addAction(tr("发送即时消息"));
    m_childMenu->addSeparator();
    m_childMenu->addAction("移动至黑名单");
    m_childMenu->addAction("删除联系人");

    QMenu *groupListMenu = new QMenu(tr("移动联系人至"));
    m_childMenu->addMenu(groupListMenu);

    m_childMenu->addAction(tr("Create a discussion group"));

    m_friendListWidget->setChildPopMenu(m_childMenu);

    //添加默认项
    //好友列表
    m_myFriend = new QQCell;
    m_myFriend->groupName = QString(tr("Friends"));
    m_myFriend->m_bOpen = false;
    m_myFriend->type = QQCELLTYPE_GROUP;
    m_myFriend->name = QString(tr("我的好友"));
    m_myFriend->m_subTitle = QString("(0/0)");
    m_friendListWidget->insertQQCell(m_myFriend);

    QQCell *blacklist = new QQCell;
    blacklist->groupName = QString(tr("黑名单"));
    blacklist->m_bOpen = false;
    blacklist->type = QQCELLTYPE_GROUP;
    blacklist->name = QString(tr("黑名单"));
    blacklist->m_subTitle = QString("(0/0)");
    m_friendListWidget->insertQQCell(blacklist);

    //组列表
    m_myGroupMenu = new QMenu(this);
    m_myGroupMenu->addAction(tr("Create a discussion group"));
    m_myGroupMenu->addAction(tr("Join the discussion group"));
    m_myGroupMenu->addAction(tr("Delete this group"));
    m_myGroupMenu->addSeparator();

    m_groupListWidget->setGroupPopMenu(m_myGroupMenu);
    //设置子菜单
    QMenu *groupChildMen = new QMenu(this);
    groupChildMen->addAction(tr("发送即时消息"));
    m_groupListWidget->setChildPopMenu(groupChildMen);

    //添加默认项
    m_groupCell = new QQCell;
    m_groupCell->groupName = QString(tr("Groups"));
    m_groupCell->m_bOpen = false;
    m_groupCell->type = QQCELLTYPE_GROUPEX;
    m_groupCell->name = QString(tr("讨论组"));
    m_groupCell->m_subTitle = QString("(0/0)");
    m_groupListWidget->insertQQCell(m_groupCell);


}

void MainWindow::initBottom()
{
    m_bottomWidget = new QWidget();

    QHBoxLayout *indexLay = new QHBoxLayout();

    indexLay->setSpacing(0);
    indexLay->setContentsMargins(0, 0, 0, 0); // 去掉布局的边距

    m_talkToolBtn = new QToolButton();
    m_talkToolBtn->setIcon(QIcon(":/resource/talk.png"));
    m_talkToolBtn->setText("Talks");
    m_talkToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_talkToolBtn->setToolTip("Chat");
    m_talkToolBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_friendToolBtn = new QToolButton();
    m_friendToolBtn->setIcon(QIcon(":/resource/friend.png"));
    m_friendToolBtn->setText("Friends");
    m_friendToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_friendToolBtn->setToolTip("Friends");
    m_friendToolBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    m_groupToolBtn = new QToolButton();
    m_groupToolBtn->setIcon(QIcon(":/resource/group.png"));
    m_groupToolBtn->setText("Groups");
    m_groupToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_groupToolBtn->setToolTip("Groups");
    m_groupToolBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_appToolBtn = new QToolButton();
    m_appToolBtn->setIcon(QIcon(":/resource/applation.png"));
    m_appToolBtn->setText("Apps");
    m_appToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_appToolBtn->setToolTip("Apps");
    m_appToolBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    indexLay->addWidget(m_talkToolBtn,1);
    indexLay->addWidget(m_friendToolBtn,1);
    indexLay->addWidget(m_groupToolBtn,1);
    indexLay->addWidget(m_appToolBtn,1);


    m_menuToolBtn = new QToolButton();
    m_menuToolBtn->setIcon(QIcon(":/resource/menu.png"));
    m_menuToolBtn->setToolTip("Menu");

    m_addFriendToolBtn = new QToolButton();
    m_addFriendToolBtn->setIcon(QIcon(":/resource/add_friend.png"));
    m_addFriendToolBtn->setToolTip("Add friend");

    m_weatherToolBtn = new QToolButton();
    m_weatherToolBtn->setIcon(QIcon(":/resource/weather.png"));
    m_weatherToolBtn->setToolTip("Weather");

    m_calenderToolBtn = new QToolButton();
    m_calenderToolBtn->setIcon(QIcon(":/resource/calendar.png"));
    m_calenderToolBtn->setToolTip("Calender");

    QHBoxLayout *bLay = new QHBoxLayout();
    bLay->setSpacing(0);
    bLay->setContentsMargins(0,0,0,0);

    bLay->addWidget(m_menuToolBtn);
    bLay->addWidget(m_addFriendToolBtn);
    bLay->addStretch();
    bLay->addWidget(m_weatherToolBtn);
    bLay->addWidget(m_calenderToolBtn);


    QVBoxLayout *bvLay=new QVBoxLayout();
    bvLay->addLayout(indexLay);
    bvLay->addLayout(bLay);
    bvLay->setContentsMargins(0,0,0,0);
    bvLay->setSpacing(0);

    m_bottomWidget->setLayout(bvLay);
}

void MainWindow::initConnect()
{
    connect(m_minBtn,&QPushButton::clicked,this,&MainWindow::on_minBtn_clicked);
    connect(m_closeBtn,&QPushButton::clicked,this,&MainWindow::on_closeBtn_clicked);


    connect(m_btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),this, &MainWindow::on_pageChanged);

    connect(m_sysTrayIcon, &QSystemTrayIcon::activated,this, &MainWindow::on_trayIcoClicked);
    connect(m_trayMenu, &QMenu::triggered, this, &MainWindow::on_trayIconMenuClicked);

    connect(m_addFriendMenu, &QMenu::triggered, this, &MainWindow::on_addFriendAction);
    connect(m_childMenu, &QMenu::triggered,this, &MainWindow::on_childPopMenuAction);
    connect(m_friendListWidget, &QQListWidget::sig_childDoubleClicked, this, &MainWindow::on_friendsClicked);

    connect(m_myGroupMenu, &QMenu::triggered, this, &MainWindow::on_groupPopMenuSelected);
    connect(m_groupListWidget, &QQListWidget::sig_childDoubleClicked, this, &MainWindow::on_groupsClicked);


    connect(m_addFriendToolBtn,&QToolButton::clicked,this,&MainWindow::on_addFriendToolBtn_clicked);

    connect(m_calenderToolBtn,&QToolButton::clicked,this,&MainWindow::on_calenderToolBtn_clicked);

    connect(m_signatureEdit,&QLineEdit::editingFinished,this,&MainWindow::on_signature_editFinished);

    connect(m_tcpSocket,&CTcpSocket::disconnected,this,&MainWindow::on_serverClosed);
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_bQuit) {
        m_sysTrayIcon->show();
        hide();
        event->ignore();
    }
}

void MainWindow::on_minBtn_clicked()
{
    showMinimized();
}

void MainWindow::on_closeBtn_clicked()
{
    close();
}

void MainWindow::on_trayIcoClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    {
    }
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        if (!this->isVisible())
            this->show();
    }
        break;
    default:
        break;
    }
}

void MainWindow::on_trayIconMenuClicked(QAction *action)
{
    if ("Quit" == action->text()) {
        m_bQuit = true;
        close();
    }
    else if ("Main" == action->text()) {
        this->show();
    }
    else if (!QString::compare("I'm online", action->text()))
    {
        if (m_tcpSocket->state() != QTcpSocket::ConnectedState)
        {
            m_tcpSocket->connect(Global::m_serverIP, Global::m_msgPort);
        }

    }
    else if (!QString::compare("Offline", action->text()))
    {
        if (m_tcpSocket->isOpen())
            m_tcpSocket->abort();

    }
}

void MainWindow::on_friendChatWindowClose()
{
    CChatWindow *chatWindow = (CChatWindow *)sender();

    // Disconnect a signal-slot connection
    disconnect(chatWindow, &CChatWindow::sig_sendMessage, this, &MainWindow::on_sendMsg);
    disconnect(chatWindow, &CChatWindow::sig_close, this, &MainWindow::on_groupChatWindowClose);

    if (!this->isVisible() && m_chatFriendWindows.size() == 1)
    {
        this->show();
    }

    // Delete chat window
    m_chatFriendWindows.removeOne(chatWindow);
}

void MainWindow::on_groupChatWindowClose()
{
    CChatWindow *chatWindow = (CChatWindow*)sender();
    disconnect(chatWindow, &CChatWindow::sig_sendMessage, this, &MainWindow::on_sendMsg);
    disconnect(chatWindow, &CChatWindow::sig_close, this, &MainWindow::on_groupChatWindowClose);
    if (!this->isVisible() && m_chatGroupWindows.size() == 1)
    {
        this->show();
    }
    // Delete chat window
    m_chatGroupWindows.removeOne(chatWindow);
}

void MainWindow::on_sendMsg(const json &js)
{
    if (!m_tcpSocket->isOpen()) {
        m_tcpSocket->connect("192.168.234.141",60000);
        m_tcpSocket->waitForConnected(1000);
    }

    if (!m_tcpSocket->isOpen())
        return;

    m_tcpSocket->send(js.dump().c_str(),strlen(js.dump().c_str())+1);
}

void MainWindow::on_signature_editFinished()
{
    json js;
    js["msgid"]=UPDATE_SIGNATURE;
    js["userid"]=Global::m_currentUser.getId();
    js["signature"]=m_signatureEdit->text().toStdString();
    string buf = js.dump();

    m_tcpSocket->send(buf.c_str(),strlen(buf.c_str())+1);
}

void MainWindow::on_calenderToolBtn_clicked()
{
    QDialog *calendarDialog = new QDialog(this);
    QCalendarWidget *calendar = new QCalendarWidget(calendarDialog);
    calendar->setGridVisible(true);

    QVBoxLayout *layout = new QVBoxLayout(calendarDialog);
    layout->addWidget(calendar);

    calendarDialog->setWindowTitle("Select a Date");

    connect(calendar, &QCalendarWidget::clicked, this, [this, calendarDialog](const QDate &date) {
        QString selectedDate = date.toString("yyyy-MM-dd");
        QMessageBox::information(this, "Selected Date", "You selected: " + selectedDate);
        calendarDialog->accept();
    });

    calendarDialog->exec();

}

void MainWindow::on_serverClosed()
{
    if (m_bQuit)
        return;
    QPixmap pixmap  = QPixmap(":/resource/chat.png");
    pixmap = CHelper::ChangeGrayPixmap(pixmap.toImage());
    m_sysTrayIcon->setIcon(QIcon(pixmap));
    m_headLabel->setPixmap(CHelper::ChangeGrayPixmap(m_headLabel->pixmap()->toImage()));

    if (!this->isVisible()) this->show();
    QMessageBox::question(this, tr("Error"),tr("diconnect to server!!!"));

}

void MainWindow::on_friendMsg(json js)
{
    int fromid = js["id"].get<int>();
    foreach (CChatWindow *window, m_chatFriendWindows) {
        if (window->getUserId() == fromid) {
            window->addMessage(js);
            window->show();
            return;
        }
    }

    QList<QQCell *> groups = m_friendListWidget->getCells();
    foreach (QQCell *cell, groups.at(0)->childs) {
        if (cell->id == fromid) {
            CChatWindow *chatWindow = new CChatWindow();
            connect(chatWindow, &CChatWindow::sig_sendMessage,this,&MainWindow::on_sendMsg);
            connect(chatWindow, &CChatWindow::sig_close, this,&MainWindow::on_friendChatWindowClose);

            chatWindow->setCell(cell);
            chatWindow->addMessage(js);
            chatWindow->show();
            // 添加到当前聊天框
            m_chatFriendWindows.append(chatWindow);
            return;
        }
    }
}

void MainWindow::on_groupMsg(json js)
{
    qDebug()<<__FUNCTION__;
    int fromid = js["groupid"].get<int>();
    foreach (CChatWindow *window, m_chatGroupWindows) {
        if (window->getUserId() == fromid) {
            window->addMessage(js);
            window->show();
            return;
        }
    }



    // 没有检索到聊天窗口，直接弹出新窗口
    QList<QQCell *> groups = m_groupListWidget->getCells();
    foreach (QQCell *cell, groups.at(0)->childs) {
        if (cell->id == fromid) {
            CChatWindow *chatWindow = new CChatWindow();
            connect(chatWindow, &CChatWindow::sig_sendMessage,this,&MainWindow::on_sendMsg);
            connect(chatWindow, &CChatWindow::sig_close, this,&MainWindow::on_groupChatWindowClose);

            auto it = std::find_if(Global::m_currentUserGroupList.begin(), Global::m_currentUserGroupList.end(), [=](Group& obj) {
                return obj.getId() == cell->id;
            });

            vector<GroupUser> groupusers;
            if(it!=Global::m_currentUserGroupList.end()){
                groupusers = it->getUsers();
            }

            chatWindow->setCell(cell,1);
            chatWindow->getGroupusers(groupusers);
            chatWindow->addMessage(js);
            chatWindow->show();
            // 添加到当前聊天框
            m_chatGroupWindows.append(chatWindow);
            return;
        }
    }
}

void MainWindow::on_offlineMsg(json js)
{
    qDebug()<<__FUNCTION__<<js["msgid"].get<int>();
    if(ONE_CHAT_MSG==js["msgid"].get<int>())
    {
        on_friendMsg(js);
    }else if(GROUP_CHAT_MSG ==js["msgid"].get<int>()) {
        on_groupMsg(js);
    }else if(ADD_FRIEND_MSG ==js["msgid"].get<int>()){
        on_addFriendMsg(js);
    }
}

void MainWindow::on_addFriendMsg(json js)
{
    QString fromname = QString::fromStdString(js["name"]);
    int friendid = js["friendid"].get<int>();
    int id = js["id"].get<int>();
    QMessageBox::question(this,tr("Tips"), tr("[%1] Request to add you as a friend, do you accept?？").arg(fromname));

    json ackjson;
    ackjson["msgid"]=ADD_FRIEND_ACK;
    ackjson["id"] = id;
    ackjson["friendid"] = friendid;

    m_tcpSocket->send(ackjson.dump().c_str(),strlen(ackjson.dump().c_str())+1);
}

void MainWindow::on_ackAddFriendMsg(json js)
{
    try {
        // 从 JSON 中提取 friendid 和 id
        int friendid = js.at("friendid").get<int>();
        int id = js.at("id").get<int>();

        int othrerId = (friendid == Global::m_currentUser.getId()) ? id : friendid;

        QMessageBox::information(this, "Tips",
                                 tr("Have become friends with [%1]").arg(othrerId));

        if (!js.contains("user") || !js.contains("friend")) {
            qDebug() << "JSON structure invalid!";
            return;
        }

        json userjson = js.at("user");
        json fjson = js.at("friend");

        if (!userjson.is_object() || !fjson.is_object()) {
            qDebug() << "User or Friend data invalid!";
            return;
        }

        json resultjson;
        if (fjson.at("id").get<int>() == Global::m_currentUser.getId()) {
            resultjson = userjson;
        } else {
            resultjson = fjson;
        }


        User user;
        user.setId(resultjson.at("id").get<int>());
        user.setName(resultjson.at("name").get<std::string>());
        user.setState(resultjson.at("state").get<std::string>());
        user.setSignature(resultjson.at("signature").get<std::string>());

        Global::m_currentUserFriendList.push_back(user);

        QQCell *child1 = new QQCell();
        child1->type = QQCELLTYPE_CHILD;
        child1->groupName = "Friends";
        child1->name = QString::fromStdString(user.getName());
        child1->m_signature = QString::fromStdString(user.getSignture());

        child1->m_subTitle =
                QString::fromStdString(user.getState()) + " | " + child1->m_signature;
        child1->status = (user.getState() == "offline") ? OFFLINE : ONLINE;
        child1->m_iconPath = ":/resource/1.bmp";
        child1->id = user.getId();

        m_myFriend->childs.append(child1);
        m_friendListWidget->upload();
    } catch (const std::exception &e) {
        qDebug() << "Exception in on_ackAddFriendMsg:" << e.what();
    }
}

void MainWindow::on_createGroupAck(json js)
{
    int groupid =js["groupid"].get<int>();
    QString groupname = QString::fromStdString(js["groupname"]);
    QString groupdesc = QString::fromStdString(js["groupdesc"]);

    Group group;
    group.setId(groupid);
    group.setName(js["groupname"]);
    group.setDesc(js["groupdesc"]);
    GroupUser groupuser;
    groupuser.setId(Global::m_currentUser.getId());
    groupuser.setName(Global::m_currentUser.getName());
    groupuser.setState(Global::m_currentUser.getState());
    groupuser.setRole("creator");
    group.addGroupUser(groupuser);

    Global::m_currentUserGroupList.push_back(group);


    // create group cell
    QQCell *cell = new QQCell;
    cell->groupName = QString(tr("Groups"));
    cell->m_iconPath  = ":/resource/1.bmp";
    cell->type      = QQCELLTYPE_CHILD;
    cell->name      = groupname;
    cell->m_subTitle  = groupdesc;
    cell->id        = groupid;
    cell->status    = ONLINE;

    qDebug() << cell->name;
    qDebug() << cell->groupName;

    m_groupListWidget->insertQQCell(cell);
}

void MainWindow::on_ackAddGroup(json js)
{
    qDebug() << __FUNCTION__;

    // get group info
    int groupid = js["groupid"].get<int>();
    string groupname = js["groupname"];
    string groupdesc = js["groupdesc"];

    Group group;
    group.setId(groupid);
    group.setName(groupname);
    group.setDesc(groupdesc);

    // get users
    for (const auto& userJs : js["users"]) {
        GroupUser user;
        user.setId(userJs["id"].get<int>());
        user.setName(userJs["name"]);
        user.setState(userJs["state"]);
        user.setRole(userJs["role"]);

        group.getUsers().push_back(user);
    }

    //
    Global::m_currentUserGroupList.push_back(group);

    // 创建群组项并插入
    QQCell* cell = new QQCell;
    cell->groupName = QString(tr("Groups"));
    cell->m_iconPath = ":/resource/1.bmp";
    cell->type = QQCELLTYPE_CHILD;
    cell->name = QString::fromStdString(group.getName());
    cell->m_subTitle = QString::fromStdString(group.getDesc());
    cell->id = group.getId();
    cell->status = ONLINE;

    m_groupListWidget->insertQQCell(cell);

}

void MainWindow::on_addFriendToolBtn_clicked()
{
    bool ok;
    QString friendid = QInputDialog::getText(
                nullptr,
                "Add Friend",
                "please input friend id：",
                QLineEdit::Normal,
                "",
                &ok
                );

    if (!ok || friendid.isEmpty()) {
        QMessageBox::information(nullptr, "Tips", "The operation was canceled or the input is empty！");
        return;
    }

    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = Global::m_currentUser.getId();
    js["name"]=Global::m_currentUser.getName();
    js["friendid"] = friendid.toInt();
    string buffer = js.dump();

    m_tcpSocket->send(js.dump().c_str(),strlen(js.dump().c_str())+1);
}

void MainWindow::on_friendStateChange(int userid, QString state)
{
    m_friendListWidget->updateCellStatus(userid,state);
}

void MainWindow::on_pageChanged(int index)
{
    static int s_nIndex = 0;
    if (index == s_nIndex)
        return;

    m_stackWidget->setLength(m_stackWidget->width(),index > s_nIndex? AnimationStackedWidget::LeftToRight:AnimationStackedWidget::RightToLeft);

    m_stackWidget->start(index);
    s_nIndex = index;
}

void MainWindow::on_addFriendAction(QAction *action)
{
    qDebug()<<__FUNCTION__;
    on_addFriendToolBtn_clicked();
}

void MainWindow::on_groupPopMenuSelected(QAction *action)
{
    qDebug()<<__FUNCTION__;
    if (!action->text().compare(tr("Create a discussion group")))
    {
        bool ok;
        QString groupName = QInputDialog::getText(this, tr("Create a discussion group"),
                                                  tr("Please enter a discussion group name:"), QLineEdit::Normal,
                                                  "", &ok);
        if (ok && !groupName.isEmpty())
        {
            QString groupDesc = QInputDialog::getText(this, tr("Discussion Group Description"),
                                                      tr("Please enter a discussion group description:"), QLineEdit::Normal,
                                                      "", &ok);
            if (ok && !groupDesc.isEmpty())
            {

                json js;
                js["msgid"] = CREATE_GROUP_MSG;
                js["id"] = Global::m_currentUser.getId();
                js["groupname"] = groupName.toStdString();
                js["groupdesc"] = groupDesc.toStdString();
                string buffer = js.dump();

                if(!m_tcpSocket->send(buffer.c_str(),strlen(buffer.c_str())+1))
                {
                    QMessageBox::question(this, tr("Error"), tr("Failed to establish connection with server！"));
                }else{
                    QMessageBox::information(this, tr("Tips"), tr("Discussion group %1 created successfully").arg(groupName));
                }

            }
        }
    }
    else if (!action->text().compare(tr("Join the discussion group")))
    {
        bool ok;
        int groupId = QInputDialog::getInt(this, tr("Join a group"),
                                           tr("Please enter the group ID:"), 0, 0, 100000, 1, &ok);
        if (!ok || groupId <= 0) {
            QMessageBox::warning(this, tr("warning"), tr("Invalid group ID!"));
            return;
        }

        // Construct a JSON request to join a group
        json js;
        js["msgid"] = ADD_GROUP_MSG;
        js["id"] = Global::m_currentUser.getId();
        js["groupid"] = groupId;
        string buffer = js.dump();

        // Send a request to the server
        m_tcpSocket->send(buffer.c_str(),strlen(buffer.c_str())+1);

        QMessageBox::information(this, tr("提示"), tr("请求已发送，等待服务器处理。"));
    }
    else if (!action->text().compare(tr("Delete this group")))
    {

    }
}

void MainWindow::on_childPopMenuAction(QAction *action)
{
    QQCell *cell = m_friendListWidget->getRightClickedCell();
    if (nullptr == cell)
        return;

    if (!action->text().compare(tr("发送即时消息")))
    {
        on_friendsClicked(cell);
    }
    else if (!action->text().compare(tr("移动至黑名单")))
    {
        qDebug() << "balck friend list" << cell->name;
    }
    else if (!action->text().compare(tr("删除联系人")))
    {
        QString delname = cell->name;
        json js;
        js["msgid"]=DEL_FRIEND_MSG;
        js["userid"]=Global::m_currentUser.getId();
        js["friendid"]=cell->id;
        std::string buffer = js.dump();
        qDebug()<<"delFriend"<<QString::fromStdString(buffer);

        if(!m_tcpSocket->send(buffer.c_str(),strlen(buffer.c_str())+1))
        {
            qDebug()<<"delete friend "<<cell->name<<" "<<m_tcpSocket->errorString();
        }

        m_friendListWidget->removeQQCell(cell);

        QMessageBox::information(this,tr("Tips"),tr("delete friend %1 successfully!").arg(delname));
    }
}

void MainWindow::on_friendsClicked(QQCell *cell)
{
    // Determine whether there is a chat window with the user, if there is a pop-up window
    foreach (CChatWindow *window, m_chatFriendWindows) {
        if (!window)
            continue;

        if (window->getUserId() == cell->id) {
            window->show();
            return;
        }
    }
    // No chat window was found, a new window popped up directly
    CChatWindow *chatWindow = new CChatWindow();
    connect(chatWindow, &CChatWindow::sig_sendMessage,this,&MainWindow::on_sendMsg);
    connect(chatWindow, &CChatWindow::sig_close, this,&MainWindow::on_friendChatWindowClose);

    chatWindow->setCell(cell);
    chatWindow->show();

    m_chatFriendWindows.append(chatWindow);
}

void MainWindow::on_groupsClicked(QQCell *cell)
{
    qDebug()<<__FUNCTION__;


    auto it = std::find_if(Global::m_currentUserGroupList.begin(), Global::m_currentUserGroupList.end(), [=](Group& obj) {
        return obj.getId() == cell->id;
    });

    vector<GroupUser> groupusers;
    if(it!=Global::m_currentUserGroupList.end()){
        groupusers = it->getUsers();
    }



    // Determine whether there is a chat window with the user, if there is a pop-up window
    foreach (CChatWindow *window, m_chatGroupWindows) {
        if (window->getUserId() == cell->id) {
            window->show();
            return;
        }
    }

    // No chat window was found, a new window popped up directly
    CChatWindow *chatWindow = new CChatWindow();
    connect(chatWindow, &CChatWindow::sig_sendMessage, this, &MainWindow::on_sendMsg);
    connect(chatWindow, &CChatWindow::sig_close, this, &MainWindow::on_groupChatWindowClose);


    chatWindow->setCell(cell, 1);

    chatWindow->getGroupusers(groupusers);

    chatWindow->show();

    // Add to current chat box
    m_chatGroupWindows.append(chatWindow);
}
