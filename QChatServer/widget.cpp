#include "widget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QThread>
#include<QButtonGroup>
#include<QHeaderView>

Widget::Widget(QWidget *parent)
    : CFramelessWidget(parent)
{
    setMinimumSize(QSize(1200,900));
    setWindowFlags(Qt::FramelessWindowHint);

    initStackedWidget();
    initTitle();
    initLoginWidget();

    initWidget();

    initNavWidget();
    initFuncStackWidget();
    initClientMgrPage();
    initClientInfoPage();
    initServerSetPage();
    initMgrInfoPage();
    initBackupPage();
    initStatusWidget();
    initRightWidget();

    initMainWidget();

    initBtnGroup();

    initConnect();

    initChatServer();
}

Widget::~Widget()
{
}

void Widget::initTitle()
{
    QHBoxLayout *hTitleLay = new QHBoxLayout();

    m_logoLabel = new QLabel();
    m_logoLabel->setPixmap(QPixmap(":/resource/app.png").scaled(60,60));

    m_titleLabel = new QLabel(tr("Chat Server Management System"));

    m_minBtn = new QPushButton(QIcon(":/resource/icon-mininize.png"),nullptr);
    m_minBtn->setFixedSize(QSize(40,40));
    m_closeBtn = new QPushButton(QIcon(":/resource/icon-close.png"),nullptr);
    m_closeBtn->setFixedSize(QSize(40,40));

    hTitleLay->addWidget(m_logoLabel);
    hTitleLay->addWidget(m_titleLabel);
    hTitleLay->addStretch();
    hTitleLay->addWidget(m_minBtn);
    hTitleLay->addWidget(m_closeBtn);

    m_titleWidget = new QWidget(m_stackWidget);
    m_titleWidget->setLayout(hTitleLay);
}

void Widget::initLoginWidget()
{
    m_loginWidget = new QWidget();
    m_loginWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置为填充剩余空间

    QHBoxLayout *hUserLay = new QHBoxLayout();
    m_userLabel = new QLabel("Username: ");
    m_userEdit = new QLineEdit("admin");
    hUserLay->addStretch();
    hUserLay->addWidget(m_userLabel);
    hUserLay->addWidget(m_userEdit);
    hUserLay->addStretch();

    QHBoxLayout *hPwdLay = new QHBoxLayout();
    m_pwdLabel = new QLabel("Password: ");
    m_pwdEdit = new QLineEdit("admin123");
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    hPwdLay->addStretch();
    hPwdLay->addWidget(m_pwdLabel);
    hPwdLay->addWidget(m_pwdEdit);
    hPwdLay->addStretch();

    QHBoxLayout *hBtnLay = new QHBoxLayout();
    m_loginBtn = new QPushButton("Login");
    m_exitBtn = new QPushButton("Exit");
    hBtnLay->addStretch();
    hBtnLay->addWidget(m_loginBtn);
    hBtnLay->addSpacing(40);
    hBtnLay->addWidget(m_exitBtn);
    hBtnLay->addStretch();


    QVBoxLayout *vInputLay = new QVBoxLayout();

    vInputLay->addLayout(hUserLay);
    vInputLay->addSpacing(20);        // 输入框之间增加间距
    vInputLay->addLayout(hPwdLay);
    vInputLay->addSpacing(20);        // 输入框和按钮之间增加间距
    vInputLay->addLayout(hBtnLay);
    //    vInputLay->addStretch();          // 底部留白

    m_loginWidget->setLayout(vInputLay);

    m_stackWidget->addWidget(m_loginWidget);
}

void Widget::initStackedWidget()
{
    m_bodyWidget = new QWidget(this);
    m_stackWidget = new AnimationStackedWidget(m_bodyWidget);

    // 设置父窗口的布局
    QVBoxLayout *layout = new QVBoxLayout(m_bodyWidget);
    layout->setContentsMargins(10,10,10,10); // 去除边距

    layout->addWidget(m_stackWidget);

    m_stackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置为填充剩余空间
    m_stackWidget->setCurrentIndex(0);

    layout->setContentsMargins(10,10,10,10);
}

void Widget::initWidget()
{
    QVBoxLayout *vBodyLay = new QVBoxLayout();

    m_titleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed); // 固定高度

    vBodyLay->addWidget(m_titleWidget, 1);

    m_bodyWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding); // 自动填充剩余空间

    vBodyLay->addWidget(m_bodyWidget, 3);

    //vBodyLay->addStretch(1);


    vBodyLay->setContentsMargins(10,10,10,10); //
    //vBodyLay->setSpacing(10); // 控件之间的间距为10像素

    setLayout(vBodyLay);
}

void Widget::initNavWidget()
{
    m_clientMgrBtn = new QPushButton(QIcon(":/resource/clientMgr.png"),"Client Manage");
    m_clientInfoBtn = new QPushButton(QIcon(":/resource/icons_goods.png"),"Client Info");
    m_serverSetBtn = new QPushButton(QIcon(":/resource/config.png"),"Server Setting");
    m_mgrInfoBtn = new QPushButton(QIcon(":/resource/manager.png"),"Manager Info");
    m_backupBtn = new QPushButton(QIcon(":/resource/backup.png"),"Data Backup");
    m_exitSystemBtn = new QPushButton(QIcon(":/resource/exit.png"),"Exit System");

    QVBoxLayout *vlay = new QVBoxLayout();

    vlay->addWidget(m_clientMgrBtn);
    vlay->addWidget(m_clientInfoBtn);
    vlay->addWidget(m_serverSetBtn);
    vlay->addWidget(m_mgrInfoBtn);
    vlay->addWidget(m_backupBtn);
    vlay->addStretch();
    vlay->addWidget(m_exitSystemBtn);

    m_navWidegt = new QWidget(m_mainWidget);
    m_navWidegt->setLayout(vlay);
}

void Widget::initRightWidget()
{
    m_rightWidget = new QWidget();

    QVBoxLayout *vlay = new QVBoxLayout(m_rightWidget);


    vlay->addWidget(m_funcStackWidget);
    vlay->addWidget(m_statuWidget);

}

void Widget::initFuncStackWidget()
{
    m_funcStackWidget = new AnimationStackedWidget(m_rightWidget);
    m_funcStackWidget->setCurrentIndex(0);
}

void Widget::initClientMgrPage()
{
    m_clientMgrPage = new QWidget();

    m_clientMgrTreeView =  new QTreeView();
    m_clientMgrModel = new QStandardItemModel();
    m_prevPageBtn = new QPushButton("Prev");
    m_nextPageBtn = new QPushButton("Next");
    m_inputEdit = new QLineEdit();
    m_pageLabel = new QLabel("  /  "+QString::number(m_totalPages));
    m_updateClientMgrBtn = new QPushButton("Update");

    // set model head
    m_clientMgrModel->setHorizontalHeaderLabels({"ID", "Name", "Password", "State","Signarure"});

    m_clientMgrTreeView->setModel(m_clientMgrModel);
    m_clientMgrTreeView->setHeaderHidden(false); // show header

    m_clientMgrTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *hpageLay = new QHBoxLayout();
    hpageLay->addWidget(m_prevPageBtn);
    hpageLay->addStretch();
    hpageLay->addWidget(m_inputEdit);
    hpageLay->addWidget(m_pageLabel);
    hpageLay->addWidget(m_updateClientMgrBtn);
    hpageLay->addStretch();
    hpageLay->addWidget(m_nextPageBtn);

    QVBoxLayout *vlay = new QVBoxLayout();

    vlay->addWidget(m_clientMgrTreeView);
    vlay->addLayout(hpageLay);

    m_clientMgrPage->setLayout(vlay);

    loadClientMgrData(m_currentPage);

    m_funcStackWidget->addWidget(m_clientMgrPage);

}

void Widget::loadClientMgrData(int page)
{
    UserModel userModel;

    int totalItems = userModel.count(); // 获取总记录数
    m_totalPages = (totalItems + m_itemsPerPage - 1) / m_itemsPerPage;

    if (page < 1 || page > m_totalPages)
    {
        qDebug() << "Invalid page number";
        return;
    }

    // set page label
    m_pageLabel->setText(QString("Page %1 / %2").arg(page).arg(m_totalPages));
    m_inputEdit->setText(QString::number(page));

    // get cur page data
    int offset = (page - 1) * m_itemsPerPage;
    vector<User> userList = userModel.queryPage(offset, m_itemsPerPage);

    // clear old data
    m_clientMgrModel->removeRows(0, m_clientMgrModel->rowCount());

    // add new data
    for (const auto &user : userList)
    {
        QList<QStandardItem *> items;

        items.append(new QStandardItem(QString::number(user.getId())));
        items.append(new QStandardItem(QString::fromStdString(user.getName())));
        items.append(new QStandardItem(QString::fromStdString(user.getPwd())));
        items.append(new QStandardItem(QString::fromStdString(user.getState())));
        items.append(new QStandardItem(QString::fromStdString(user.getSignture())));

        m_clientMgrModel->appendRow(items);
    }
}

void Widget::appendUserToModel(const User &user)
{
    QList<QStandardItem *> items;

    items.append(new QStandardItem(QString::number(user.getId())));
    items.append(new QStandardItem(QString::fromStdString(user.getName())));
    items.append(new QStandardItem(QString::fromStdString(user.getPwd())));
    items.append(new QStandardItem(QString::fromStdString(user.getState())));

    m_clientMgrModel->appendRow(items);
}

void Widget::on_clientPrevPage()
{
    if (m_currentPage > 1)
    {
        m_currentPage--;
        loadClientMgrData(m_currentPage);
    }
}

void Widget::on_clientNextPage()
{
    if (m_currentPage < m_totalPages)
    {
        m_currentPage++;
        loadClientMgrData(m_currentPage);
    }
}

void Widget::on_changeClientPage()
{
    int page = m_inputEdit->text().toInt();
    if (page >= 1 && page <= m_totalPages)
    {
        m_currentPage = page;
        loadClientMgrData(page);
    }
    else
    {
        qDebug() << "Invalid page number entered";
    }
}

void Widget::on_updateClientMgr()
{
    UserModel userModel;

    // Query all records newer than the current maximum ID
    std::vector<User> newUsers = userModel.queryNewRecords(m_lastId);

    for (const auto &user : newUsers)
    {
        appendUserToModel(user);
        m_lastId = std::max(m_lastId, user.getId()); // Update the most recently inserted ID
    }
}

void Widget::initClientInfoPage()
{
    m_clientInfoPage = new QWidget();

    m_textBrowser = new QTextBrowser(m_clientInfoPage);

    // 设置父窗口的布局
    QVBoxLayout *layout = new QVBoxLayout(m_clientInfoPage);
    layout->setContentsMargins(0, 0, 0, 0); // 去除边距
    layout->setSpacing(0);                  // 去除控件间的间隔
    layout->addWidget(m_textBrowser);

    m_funcStackWidget->addWidget(m_clientInfoPage);

}

void Widget::initServerSetPage()
{
    m_serverSetPage = new QWidget();

    m_funcStackWidget->addWidget(m_serverSetPage);

}

void Widget::initMgrInfoPage()
{
    m_mgrInfoPage = new QWidget();

    m_funcStackWidget->addWidget(m_mgrInfoPage);
}

void Widget::initBackupPage()
{
    m_backupPage = new QWidget();

    m_funcStackWidget->addWidget(m_backupPage);
}

void Widget::initStatusWidget()
{
    m_statuWidget = new QWidget(m_rightWidget);

    QHBoxLayout * hlay = new QHBoxLayout();

    m_serverIpLabel = new QLabel("Server IP: 192.168.1.1");
    m_systemTime = new QLabel("System Time: 1970/01/01 11:59:12:123");


    hlay->addStretch();
    hlay->addWidget(m_serverIpLabel);
    hlay->addWidget(m_systemTime);

    m_statuWidget->setLayout(hlay);

}

void Widget::initMainWidget()
{
    m_mainWidget = new QWidget();

    m_stackWidget->addWidget(m_mainWidget);

    QHBoxLayout *hlay = new QHBoxLayout();

    hlay->addWidget(m_navWidegt,1);
    hlay->addWidget(m_rightWidget,4);

    m_mainWidget->setLayout(hlay);
}

void Widget::initBtnGroup()
{
    m_btnGroup = new QButtonGroup(this);

    m_btnGroup->addButton(m_clientMgrBtn, 0);
    m_btnGroup->addButton(m_clientInfoBtn, 1);
    m_btnGroup->addButton(m_serverSetBtn, 2);
    m_btnGroup->addButton(m_mgrInfoBtn, 3);
    m_btnGroup->addButton(m_backupBtn, 4);

}

void Widget::initConnect()
{
    connect(m_closeBtn,&QPushButton::clicked,this,&Widget::on_close);
    connect(m_exitBtn,&QPushButton::clicked,this,&Widget::on_close);
    connect(m_exitSystemBtn,&QPushButton::clicked,this,&Widget::on_exitSystem);

    connect(m_minBtn,&QPushButton::clicked,this,&Widget::on_minBtn_clicked);


    connect(m_loginBtn,&QPushButton::clicked,this,&Widget::on_loginBtn_clicked);

    connect(m_btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &Widget::on_changePage);

    connect(m_prevPageBtn,&QPushButton::clicked,this,&Widget::on_clientPrevPage);
    connect(m_nextPageBtn,&QPushButton::clicked,this,&Widget::on_clientNextPage);
    connect(m_inputEdit,&QLineEdit::editingFinished,this,&Widget::on_changeClientPage);

    connect(m_updateClientMgrBtn,&QPushButton::clicked,this,&Widget::on_updateClientMgr);

}

void Widget::initChatServer()
{
    m_serverThread = new QThread(this);

    // move to thread to prevent blocking
    connect(m_serverThread, &QThread::started, [this]() {

//        signal(SIGINT, resetHandler);

        m_loop = new EventLoop();
        InetAddress addr("192.168.234.141", 60000);
        m_server = new ChatServer(m_loop, addr, "ChatServer");

        m_server->start();

        m_loop->loop(); // start EventLoop


    });

    // free resource after thread finished
    connect(m_serverThread, &QThread::finished, [this]() {
        delete m_server;
        delete m_loop;
        m_server = nullptr;
        m_loop = nullptr;
    });

    m_serverThread->start();

}

void Widget::stopChatServer()
{
    if (m_loop != nullptr)
    {
        ChatService::instance()->reset();
        m_loop->quit(); // loop exit
        m_serverThread->quit(); // QThread exit
        m_serverThread->wait(); // wait thread finished
    }

}

void Widget::on_close()
{
    stopChatServer();
    close();
}

void Widget::on_minBtn_clicked()
{
    showMinimized();
}

void Widget::on_loginBtn_clicked()
{
    string username = m_userEdit->text().toStdString();
    string password = m_pwdEdit->text().toStdString();
    if(!username.empty() && !password.empty())
    {
        Manager manager = _managerModel.query(username);
        if(manager.getName() == username && manager.getPwd() == password)
        {
            qDebug()<<__FUNCTION__;
            m_stackWidget->setCurrentIndex(1);
        }else{
            qDebug()<<__FUNCTION__<<"error";
        }
    }
}

void Widget::on_changePage(int index)
{
    static int s_nIndex = 0;
    if (index == s_nIndex) return;

    m_funcStackWidget->setLength(m_funcStackWidget->width(),
                                 index > s_nIndex
                                 ? AnimationStackedWidget::LeftToRight
                                 : AnimationStackedWidget::RightToLeft);

    m_funcStackWidget->start(index);
    s_nIndex = index;
}

void Widget::on_exitSystem()
{
    m_stackWidget->setCurrentIndex(0);
}
