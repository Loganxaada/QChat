#include "regwidget.h"
#include<QHBoxLayout>
#include<QIcon>
#include<QDebug>
#include<QMessageBox>
#include"ComAPI/MyUnit.h"
#include"ComAPI/json.hpp"
using json = nlohmann::json;

RegWidget::RegWidget(CTcpSocket *socket,QWidget *parent)
    :CustomMoveWidget(parent)
    ,m_tcpSocket(socket)
{
    setMinimumSize(QSize(800,660));
    initTitle();
    initBody();
    QVBoxLayout *mainlay = new QVBoxLayout();
    mainlay->addWidget(m_titleWidget,1);
    mainlay->addWidget(m_bodyWidget,10);
    mainlay->setContentsMargins(0,0,0,0);
    mainlay->setSpacing(0);
    setLayout(mainlay);

    initConnect();
}

RegWidget::~RegWidget()
{
}

void RegWidget::initTitle()
{
    m_titleWidget = new QWidget();

    QHBoxLayout *hlay = new QHBoxLayout();
    m_logoLabel = new QLabel();
    m_logoLabel->setPixmap(QPixmap(":/resource/chat.png").scaled(40,40));
    m_regLabel = new QLabel("Register Account");
    m_minBtn = new QPushButton(QIcon(":/resource/icon-mininize.png"),"");
    m_closeBtn = new QPushButton(QIcon(":/resource/icon-close.png"),"");

    //    m_titleWidget->setStyleSheet(
    //            "QWidget {"
    //            "  border: 2px solid #1E90FF;"      // 边框颜色
    //            "  border-radius: 5px;"            // 边框圆角
    //            "  background-color: #F0F8FF;"     // 背景颜色
    //            "}");

    hlay->addWidget(m_logoLabel);
    hlay->addWidget(m_regLabel);
    hlay->addStretch();
    hlay->addWidget(m_minBtn);
    hlay->addWidget(m_closeBtn);


    hlay->setContentsMargins(0, 0, 0, 0);

    m_titleWidget->setLayout(hlay);
}

void RegWidget::initBody()
{
    m_bodyWidget = new QWidget(this);

    QFont font("Consolas", 12);

    QLabel *usernameLabel = new QLabel("Username:");
    usernameLabel->setFont(font);
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Enter your username");
    m_usernameEdit->setFont(font);

    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setFont(font);
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Enter your password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFont(font);

    QLabel *confirmPasswordLabel = new QLabel("Confirm Password:");
    confirmPasswordLabel->setFont(font);
    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setPlaceholderText("Confirm your password");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setFont(font);

    m_agreeCheckBox = new QCheckBox("I agree to the Terms and Conditions");
    m_agreeCheckBox->setFont(font);

    m_registerButton = new QPushButton("Register");
    m_registerButton->setFont(font);
    m_registerButton->setStyleSheet("background-color: #1E90FF; color: white; font-weight: bold;");

    QVBoxLayout *mainLayout = new QVBoxLayout(m_bodyWidget);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(m_usernameEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(confirmPasswordLabel);
    mainLayout->addWidget(m_confirmPasswordEdit);
    mainLayout->addWidget(m_agreeCheckBox);
    mainLayout->addWidget(m_registerButton);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
}

void RegWidget::initConnect()
{
    connect(m_minBtn,&QPushButton::clicked,this,&RegWidget::on_minBtn_clicked);
    connect(m_closeBtn,&QPushButton::clicked,this,&RegWidget::on_closeBtn_clicked);

    connect(m_registerButton,&QPushButton::clicked,this,&RegWidget::on_regBtn_clicked)
            ;}

void RegWidget::on_regBtn_clicked()
{
    std::string name = m_usernameEdit->text().toStdString();
    std::string pwd = m_passwordEdit->text().toStdString();
    std::string confirmPwd = m_confirmPasswordEdit->text().toStdString();

    if(name.empty() || pwd.empty() || confirmPwd.empty())
    {
        qDebug()<<"please input conrrect username or password";
        return;
    }


    if(!m_agreeCheckBox ->isChecked())
    {
        qDebug()<<"please agree to the Terms and Conditions";
        return;
    }


    if(pwd == confirmPwd)
    {
        json js;
        js["msgid"] = REG_MSG;
        js["name"] = name;
        js["password"] = pwd;
        std::string request = js.dump();

        m_tcpSocket->send(request.c_str(),strlen(request.c_str())+1);
    }else{
        qDebug()<<"confrim twice password same";
    }
}

void RegWidget::on_minBtn_clicked()
{
    showMinimized();
}

void RegWidget::on_closeBtn_clicked()
{
    close();
}

void RegWidget::on_recvRegState(int id)
{
    if(-1!=id)
    {
        QMessageBox::information(this,"Register Infomation",tr("register success, userid is  %1, do not forget it!").arg(id));

        close();
    }else{
        QMessageBox::warning(this,"Register Infomation",tr("username is already exist, register error"));

    }
}
