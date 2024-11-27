#include "loginwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QApplication>
#include <QPropertyAnimation>
#include "ComAPI/json.hpp"
#include <string>
#include <QNetworkProxy>
#include <QMessageBox>
#include "ComAPI/global.h"

using namespace std;

using json = nlohmann::json;

LoginWidget::LoginWidget(QWidget *parent) : CustomMoveWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(QSize(800, 660));

    m_tcpSocket = new CTcpSocket(this);
    m_regWidget = new RegWidget(m_tcpSocket);

    m_bConnected = m_tcpSocket->connect("192.168.234.141", 60000);

    m_worker = new Worker();
    m_workerThread = new QThread();

    m_worker->moveToThread(m_workerThread);

    m_workerThread->start();

    initWidget();
    m_mainwindow = new MainWindow(m_tcpSocket);
    initConnect();
    setBubbleBackground();
}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::initWidget()
{
    QFont font = QFont("Consolas");
    font.setPointSize(14);
    font.setBold(true);

    m_stackedWidget = new RotatingStackedWidget(this); // login and set

    m_userWidget = new QWidget();

    m_setBtn = new QPushButton(QIcon(":/resource/setting.png"), "", m_userWidget);
    m_minBtn = new QPushButton(QIcon(":/resource/icon-mininize.png"), "", m_userWidget);
    m_closeBtn = new QPushButton(QIcon(":/resource/icon-close.png"), "", m_userWidget);

    m_logoLael = new QLabel(m_userWidget);
    m_logoLael->setPixmap(QPixmap(":/resource/chat.png").scaled(m_setBtn->width() + 10, m_setBtn->height() + 10, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    m_flowLabel = new FlowLable("           MyChat       ", m_userWidget);
    m_flowLabel->setAlignment(Qt::AlignCenter);
    m_flowLabel->setFixedSize(QSize(800, 100));

    m_headLabel = new QLabel(m_userWidget);
    m_headLabel->setPixmap(QPixmap(":/resource/head_boy.png"));
    m_headLabel->setScaledContents(true);
    m_headLabel->setFixedSize(250, 250);

    m_acountCombox = new ComboBoxWithRemoveButton(m_userWidget);
    m_acountCombox->setEditable(true);
    m_acountCombox->setFixedHeight(50);
    m_acountCombox->setFont(font);
    m_acountCombox->addItems(Global::m_userIDs);

    m_passwdEdit = new QLineEdit(m_userWidget);
    m_passwdEdit->setPlaceholderText("please input password");
    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setFixedHeight(50);
    m_passwdEdit->setFont(font);

    m_registerLabel = new QLabel("<a href=\"#\">register account</a>", m_userWidget);
    m_registerLabel->setOpenExternalLinks(false);
    m_registerLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    m_registerLabel->setStyleSheet("color: blue; text-decoration: underline;");
    m_registerLabel->setFont(font);

    m_forgetPwdLabel = new QLabel("<a href=\"#\">forget password?</a>", m_userWidget);
    m_forgetPwdLabel->setOpenExternalLinks(true);
    m_forgetPwdLabel->setStyleSheet("color: blue;");
    m_forgetPwdLabel->setFont(font);

    m_remberCheck = new QCheckBox("remeber password", m_userWidget);
    m_remberCheck->setFont(font);
    m_autoLoginCheck = new QCheckBox("auto login", m_userWidget);
    m_autoLoginCheck->setFont(font);

    m_loginBtn = new QPushButton("Login");
    m_loginBtn->setStyleSheet("background-color: #1E90FF; color: white; font-weight: bold; font-size: 50px;margin: 20");

    QVBoxLayout *mainlay = new QVBoxLayout(m_userWidget);

    QHBoxLayout *setlay = new QHBoxLayout();
    setlay->addWidget(m_logoLael);
    setlay->setSpacing(0);
    setlay->addStretch();
    setlay->addWidget(m_setBtn);
    setlay->addWidget(m_minBtn);
    setlay->addWidget(m_closeBtn);
    mainlay->addLayout(setlay);

    QHBoxLayout *toplay = new QHBoxLayout();
    toplay->addWidget(m_flowLabel);
    mainlay->addLayout(toplay);

    QVBoxLayout *inputlay = new QVBoxLayout();
    inputlay->addWidget(m_acountCombox);
    inputlay->addSpacing(20);
    inputlay->addWidget(m_passwdEdit);
    inputlay->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *midlay = new QHBoxLayout();
    midlay->addWidget(m_headLabel);
    // midlay->addStretch();
    midlay->addLayout(inputlay);
    mainlay->addLayout(midlay);

    QHBoxLayout *optionslay = new QHBoxLayout();
    optionslay->addSpacing(30);
    optionslay->addWidget(m_remberCheck);
    optionslay->addStretch();
    optionslay->addWidget(m_autoLoginCheck);
    optionslay->addSpacing(30);

    QHBoxLayout *linklay = new QHBoxLayout();
    linklay->addSpacing(30);
    linklay->addWidget(m_registerLabel);
    linklay->addStretch();
    linklay->addWidget(m_forgetPwdLabel);
    linklay->addSpacing(30);

    mainlay->addLayout(optionslay);
    mainlay->addStretch();
    mainlay->addLayout(linklay);
    mainlay->addStretch();
    mainlay->addWidget(m_loginBtn);

    mainlay->setContentsMargins(0, 0, 0, 0);

    m_userWidget->setLayout(mainlay);
    m_stackedWidget->addWidget(m_userWidget);

    m_setWidget = new QWidget(m_stackedWidget);
    m_setWidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    m_setLabel = new QLabel("Login setting", m_setWidget);
    m_setLabel->setFont(font);
    m_setminBtn = new QPushButton(QIcon(":/resource/icon-mininize.png"), "", m_setWidget);

    m_setcloseBtn = new QPushButton(QIcon(":/resource/icon-close.png"), "", m_setWidget);

    m_setGroup = new QGroupBox("Server Setting", m_setWidget);
    m_setGroup->setFont(font);

    QHBoxLayout *settopLay = new QHBoxLayout();
    settopLay->addWidget(m_setLabel);
    settopLay->addStretch();
    settopLay->setSpacing(0);
    settopLay->addWidget(m_setminBtn);
    settopLay->addWidget(m_setcloseBtn);

    QHBoxLayout *hserverIpLay = new QHBoxLayout();

    m_serveripLabel = new QLabel("Server IP  ", m_setWidget);
    m_serveripLabel->setFont(font);
    m_serveripEdit = new CIPLineEdit(m_setWidget);
    m_serveripEdit->setText(Global::getSettingFile("Server","HostAddr").toString());
    m_serveripEdit->setFont(font);

    hserverIpLay->addWidget(m_serveripLabel);
    hserverIpLay->addWidget(m_serveripEdit);

    QHBoxLayout *hserverPortLay = new QHBoxLayout();

    m_serverportLabel = new QLabel("Server Port", m_setWidget);
    m_serverportEdit = new QLineEdit(Global::getSettingFile("Server","MsgPort").toString(),m_setWidget);

    m_fileportLabel = new QLabel("File Port", m_setWidget);
    m_fileportEdit = new QLineEdit(Global::getSettingFile("Server","FilePort").toString(),m_setWidget);


    hserverPortLay->addWidget(m_serverportLabel);
    hserverPortLay->addWidget(m_serverportEdit);

    QHBoxLayout *hfileLay = new QHBoxLayout();
    hfileLay->addWidget(m_fileportLabel);
    hfileLay->addWidget(m_fileportEdit);

    QVBoxLayout *setgroupLay = new QVBoxLayout();
    setgroupLay->addLayout(hserverIpLay);
    setgroupLay->addLayout(hserverPortLay);
    setgroupLay->addLayout(hfileLay);

    m_setGroup->setLayout(setgroupLay);

    m_okBtn = new QPushButton("   Ok   ", m_setWidget);
    m_cancelBtn = new QPushButton(" Cancel ", m_setWidget);

    QHBoxLayout *opLay = new QHBoxLayout();
    opLay->addStretch();
    opLay->addWidget(m_okBtn);
    opLay->addWidget(m_cancelBtn);

    QVBoxLayout *setmainLay = new QVBoxLayout(m_setWidget);
    setmainLay->addLayout(settopLay);
    setmainLay->addSpacing(30);
    setmainLay->addWidget(m_setGroup);
    setmainLay->addLayout(opLay);

    setmainLay->setContentsMargins(0, 0, 0, 10);

    m_stackedWidget->addWidget(m_setWidget);
    // m_stackedWidget->setLayout(setmainLay);

    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->addWidget(m_stackedWidget);
    vlay->setContentsMargins(0, 0, 0, 0);

    setLayout(vlay);

    m_stackedWidget->setCurrentIndex(0);
}

void LoginWidget::initTcpSocket()
{
}

void LoginWidget::initConnect()
{
    connect(m_setBtn, &QPushButton::clicked, this, &LoginWidget::on_cancelsetBtn_clicked);
    connect(m_minBtn, &QPushButton::clicked, this, &LoginWidget::on_minBtn_clicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &LoginWidget::on_closeBtn_clicked);
    connect(m_setminBtn, &QPushButton::clicked, this, &LoginWidget::on_minBtn_clicked);

    connect(m_okBtn,&QPushButton::clicked,this,&LoginWidget::on_okBtn_clicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &LoginWidget::on_cancelsetBtn_clicked);

    connect(m_setcloseBtn, &QPushButton::clicked, this, &LoginWidget::on_closeBtn_clicked);
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWidget::on_loginBtn_clicked);

    connect(m_registerLabel, &QLabel::linkActivated, this, &LoginWidget::on_register);

    connect(m_tcpSocket, &CTcpSocket::readyRead, this, &LoginWidget::on_recvData);
    connect(this, &LoginWidget::sig_dataRecv, m_worker, &Worker::on_processData);

    connect(m_worker, &Worker::sig_regState, m_regWidget, &RegWidget::on_recvRegState);

    connect(m_worker, &Worker::sig_LoginState, this, &LoginWidget::on_ackMsg);

    connect(m_worker, &Worker::sig_friendStateChange, m_mainwindow, &MainWindow::on_friendStateChange);
    connect(m_worker, &Worker::sig_friendMsg, m_mainwindow, &MainWindow::on_friendMsg);
    connect(m_worker, &Worker::sig_groupMsg, m_mainwindow, &MainWindow::on_groupMsg);
    connect(m_worker, &Worker::sig_offlineMsg, m_mainwindow, &MainWindow::on_offlineMsg);
    connect(m_worker, &Worker::sig_addFriend, m_mainwindow, &MainWindow::on_addFriendMsg);
    connect(m_worker, &Worker::sig_ackAddFriend, m_mainwindow, &MainWindow::on_ackAddFriendMsg);
    connect(m_worker, &Worker::sig_createGroup, m_mainwindow, &MainWindow::on_createGroupAck);
    connect(m_worker,&Worker::sig_ackAddGroup,m_mainwindow,&MainWindow::on_ackAddGroup);
}

void LoginWidget::setBubbleBackground()
{
    // Create multiple random bubbles
    for (int i = 0; i < 20; ++i)
    {
        bubbles.append(generateRandomBubble());
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LoginWidget::updateBubbles);
    timer->start(30); // Update the foam position every 30 milliseconds
}

Bubble LoginWidget::generateRandomBubble()
{
    int x = QRandomGenerator::global()->bounded(width());
    int y = QRandomGenerator::global()->bounded(height());
    int radius = QRandomGenerator::global()->bounded(30, 80);                               // Bubbles of random size
    QColor color = QColor::fromHsv(QRandomGenerator::global()->bounded(360), 100, 255, 80); // Translucent bubbles
    return Bubble(x, y, radius, color);
}

void LoginWidget::drawBubble(QPainter &painter, const Bubble &bubble)
{
    QRadialGradient gradient(bubble.x, bubble.y, bubble.radius);
    gradient.setColorAt(0, QColor(255, 255, 255, 120)); // Bubble center highlight
    gradient.setColorAt(1, bubble.color);               // Bubble edge transparent

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.setCompositionMode(QPainter::CompositionMode_Screen); // Overlay mode softens the colors
    painter.drawEllipse(bubble.x - bubble.radius, bubble.y - bubble.radius,
                        bubble.radius * 2, bubble.radius * 2);
}

void LoginWidget::updateBubbles()
{
    for (Bubble &bubble : bubbles)
    {
        bubble.y -= 1; // Move up
        if (bubble.y + bubble.radius < 0)
        { // Beyond top boundary, regenerate
            bubble = generateRandomBubble();
            bubble.y = height() + bubble.radius; // started from the bottom
        }
    }
    update();
}

void LoginWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Use a dark background as a base
    painter.fillRect(rect(), QColor("#bfefff"));

    // Drawing bubbles
    for (const Bubble &bubble : bubbles)
    {
        drawBubble(painter, bubble);
    }
}

void LoginWidget::on_cancelsetBtn_clicked()
{
    m_stackedWidget->nextPage();
}

void LoginWidget::on_okBtn_clicked()
{
    Global::setSettingFile("Server","HostAddr",m_serveripEdit->text());
    Global::setSettingFile("Server","MsgPort",m_serverportEdit->text());
    Global::setSettingFile("Server","FilePort",m_fileportEdit->text());

    QMessageBox::information(m_setWidget,"Tips","setting modify successfully");

    m_stackedWidget->nextPage();
}

void LoginWidget::on_minBtn_clicked()
{
    showMinimized();
}

void LoginWidget::on_closeBtn_clicked()
{
    close();
}

void LoginWidget::on_loginBtn_clicked()
{

    m_userWidget->setVisible(false);

    QPropertyAnimation *animation = new QPropertyAnimation(m_headLabel, "pos");
    animation->setDuration(200);
    animation->setStartValue(QPoint(40, 115));
    animation->setEndValue(QPoint((this->width() - m_headLabel->width()) / 2 - 20, 100));
    connect(animation, &QPropertyAnimation::finished, this, &LoginWidget::on_loginAnimationFinished);

    animation->start();
}

void LoginWidget::on_loginAnimationFinished()
{
    if (!m_bConnected)
    {
        m_bConnected = m_tcpSocket->connect("192.168.234.141", 60000);
        QMessageBox::information(this, "logining...", "unconnect to server...");
        m_userWidget->setVisible(true);
        m_headLabel->move(40, 115);
        return;
    }

    int id = m_acountCombox->currentText().toInt();
    string pwd = m_passwdEdit->text().toStdString();

    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = id;
    js["password"] = pwd;
    string request = js.dump();

    m_tcpSocket->send(request.c_str(), strlen(request.c_str()) + 1);
}

void LoginWidget::on_register(const QString &)
{
    m_regWidget->show();
}

void LoginWidget::on_recvData()
{
    QByteArray data = m_tcpSocket->readAll();
    qDebug() << __FUNCTION__ << data;
    emit sig_dataRecv(data);
}

void LoginWidget::on_ackMsg(bool bSuccess, const QByteArray &data)
{
    if (!bSuccess)
    {
        QMessageBox::critical(this, "Tips", data.data());
        m_userWidget->setVisible(true);
    }
    else
    {
        QStringList idList = Global::getSettingFile("UserCfg", "UserIDs").toStringList();
        qDebug() << "ID List" << idList;

        // Avoid duplicate storage
        QString appendID = QString::number(Global::m_currentUser.getId());

        if (!idList.contains(appendID))
        {
            idList.append(appendID);
            if (idList.size() > 6)
                idList.removeFirst();
        }

        Global::setSettingFile("UserCfg", "UserIDs", idList);

        m_mainwindow->m_nicknameLabel->setText(QString::fromStdString(Global::m_currentUser.getName()));
        m_mainwindow->m_signatureEdit->setText(QString::fromStdString(Global::m_currentUser.getSignture()));

        for (User &user : Global::m_currentUserFriendList)
        {
            // Create a sub-item QQCell object
            QQCell *child1 = new QQCell();
            child1->type = QQCELLTYPE_CHILD;
            child1->groupName = "Friends";
            child1->name = QString::fromStdString(user.getName());
            child1->m_signature = QString::fromStdString(user.getSignture());

            child1->m_subTitle = QString::fromStdString(user.getState()) + " | " + child1->m_signature;
            child1->status = user.getState() == "offline" ? OFFLINE : ONLINE;
            child1->m_iconPath = ":/resource/1.bmp";
            child1->id = user.getId();

            m_mainwindow->m_myFriend->childs.append(child1);
        }

        for (Group &group : Global::m_currentUserGroupList)
        {
            // create group cell
            QQCell *cell = new QQCell;
            cell->groupName = QString(tr("Groups"));
            cell->m_iconPath = ":/resource/1.bmp";
            cell->type = QQCELLTYPE_CHILD;
            cell->name = QString::fromStdString(group.getName());
            cell->m_subTitle = QString::fromStdString(group.getDesc());
            cell->id = group.getId();
            cell->status = ONLINE;

            m_mainwindow->m_groupListWidget->insertQQCell(cell);
        }

        close();
        m_mainwindow->m_friendListWidget->upload();
        m_mainwindow->show();
    }
}
