#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QGroupBox>
#include<QTextBrowser>
#include"custom/animationstackedwidget.h"
#include"server/chatserver.hpp"
#include"server/chatservice.hpp"
#include"custom/cframelesswidget.h"
#include"model/managermodel.h"
#include<QTreeView>
#include<QStandardItemModel>
#include <signal.h>

class Widget : public CFramelessWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public:
    void initTitle();
    void initLoginWidget();

    void initStackedWidget();
    void initWidget();

    void initNavWidget();
    void initRightWidget();

    void initFuncStackWidget();

    void initClientMgrPage();
    void loadClientMgrData(int page);

    void appendUserToModel(const User &user);


    void initClientInfoPage();
    void initServerSetPage();
    void initMgrInfoPage();
    void initBackupPage();

    void initStatusWidget();


    void initMainWidget();

    void initBtnGroup();

    void initConnect();

    void initChatServer();

    //stop server
    void stopChatServer();



public slots:
    void on_close();

    void on_minBtn_clicked();

    void on_loginBtn_clicked();

    void on_changePage(int index);

    void on_exitSystem();

    void on_clientPrevPage();
    void on_clientNextPage();
    void on_changeClientPage();

    void on_updateClientMgr();
public:

    AnimationStackedWidget *m_stackWidget=nullptr;

    QWidget *m_titleWidget=nullptr;


    QLabel *m_logoLabel=nullptr;
    QLabel *m_titleLabel=nullptr;
    QPushButton *m_minBtn=nullptr;
    QPushButton *m_closeBtn=nullptr;

    QWidget *m_loginWidget=nullptr;

    QWidget *m_userWidget=nullptr;

    QLabel *m_userLabel=nullptr;
    QLineEdit *m_userEdit=nullptr;
    QLabel *m_pwdLabel=nullptr;
    QLineEdit *m_pwdEdit=nullptr;

    QPushButton *m_loginBtn=nullptr;
    QPushButton *m_exitBtn=nullptr;

    QWidget *m_mainWidget=nullptr;

    QWidget *m_bodyWidget=nullptr;

    QWidget *m_navWidegt=nullptr;
    QPushButton *m_clientMgrBtn=nullptr;
    QPushButton *m_clientInfoBtn=nullptr;
    QPushButton *m_serverSetBtn=nullptr;
    QPushButton *m_mgrInfoBtn=nullptr;
    QPushButton *m_backupBtn=nullptr;
    QPushButton *m_exitSystemBtn=nullptr;

    QWidget *m_rightWidget=nullptr;

    AnimationStackedWidget *m_funcStackWidget=nullptr;
    QWidget *m_clientMgrPage=nullptr;
    QGroupBox *m_clientMgrGroupBox=nullptr;
    QStandardItemModel *m_clientMgrModel=nullptr;
    QTreeView *m_clientMgrTreeView=nullptr;

    QPushButton *m_prevPageBtn=nullptr;
    QPushButton *m_nextPageBtn=nullptr;
    QLabel *m_pageLabel=nullptr;
    QLineEdit *m_inputEdit=nullptr;
    QPushButton *m_updateClientMgrBtn=nullptr;
    int m_currentPage=1;
    int m_totalPages;
    int m_itemsPerPage=30;
    int m_lastId=0;

    QWidget *m_clientInfoPage=nullptr;
    QTextBrowser *m_textBrowser=nullptr;

    QWidget *m_serverSetPage=nullptr;
    QWidget *m_mgrInfoPage=nullptr;
    QWidget *m_backupPage=nullptr;

    QWidget *m_statuWidget=nullptr;
    QLabel *m_serverIpLabel=nullptr;
    QLabel *m_systemTime=nullptr;

    QButtonGroup *m_btnGroup=nullptr;

private:
    QThread *m_serverThread=nullptr;

    EventLoop *m_loop=nullptr;

    ChatServer *m_server=nullptr;

    ManagerModel _managerModel;

};
#endif // WIDGET_H
