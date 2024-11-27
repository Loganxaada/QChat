#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include<QCheckBox>
#include<QPushButton>
#include<QLabel>
#include<string>
#include"Net/ctcpsocket.h"
#include"CustomContrals/customwidget.h"


class RegWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit RegWidget(CTcpSocket *socket,QWidget *parent = nullptr);
    ~RegWidget();
private:
    void initTitle();
    void initBody();
    void initConnect();
signals:
    void sig_regsiter(std::string username,std::string password);
public slots:
    void on_regBtn_clicked();

    void on_minBtn_clicked();

    void on_closeBtn_clicked();

    void on_recvRegState(int id);

private:
    QWidget *m_titleWidget=nullptr;
    QLabel *m_logoLabel=nullptr;
    QLabel *m_regLabel=nullptr;
    QPushButton *m_minBtn=nullptr;
    QPushButton *m_closeBtn=nullptr;


    QWidget *m_bodyWidget=nullptr;
    QLineEdit *m_usernameEdit=nullptr;
    QLineEdit *m_passwordEdit=nullptr;
    QLineEdit *m_confirmPasswordEdit=nullptr;
    QCheckBox *m_agreeCheckBox=nullptr;
    QPushButton *m_registerButton=nullptr;

    CTcpSocket *m_tcpSocket=nullptr;




};

#endif // REGWIDGET_H
