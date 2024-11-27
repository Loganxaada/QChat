#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QStackedWidget>
#include<QToolButton>
#include<QLineEdit>
#include<QButtonGroup>
#include<QSystemTrayIcon>
#include"CustomContrals/cframelesswidget.h"
#include"CustomContrals/animationstackedwidget.h"
#include"CustomContrals/CustomList/qqlistwidget.h"
#include"CustomContrals/cchatwindow.h"
#include"Net/ctcpsocket.h"


class MainWindow : public CFramelessWidget
{
    Q_OBJECT

public:
    MainWindow(CTcpSocket *socket,QWidget *parent = nullptr);
    ~MainWindow();

public:
    void initStyleSheet();

    void initSysTray();


    void initTop();

    void initTalkWidget();
    void initContractsWidget();
    void initGroupWidget();
    void initAppWidget();

    void initListMenu();


    void initBottom();

    void initConnect();



protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void on_pageChanged(int index);

    void on_addFriendAction(QAction *action);

    void on_groupPopMenuSelected(QAction *action);
    void on_childPopMenuAction(QAction *action);

    void on_friendsClicked(QQCell *cell);

    void on_groupsClicked(QQCell *cell);

    void on_friendStateChange(int userid,QString state);

    void on_friendMsg(json js);
    void on_groupMsg(json js);

    void on_offlineMsg(json js);

    void on_addFriendMsg(json js);
    void on_ackAddFriendMsg(json js);

    void on_createGroupAck(json js);
    void on_ackAddGroup(json js);

    void on_addFriendToolBtn_clicked();



private slots:
    void on_minBtn_clicked();

    void on_closeBtn_clicked();

    // 托盘菜单处理
    void on_trayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void on_trayIconMenuClicked(QAction *action);

    void on_friendChatWindowClose();

    void on_groupChatWindowClose();

    void on_sendMsg(const json &js);

    void on_signature_editFinished();

    void on_calenderToolBtn_clicked();

    void on_serverClosed();
public:
    QLabel *m_nicknameLabel=nullptr;
    QLineEdit *m_signatureEdit=nullptr;

    QQListWidget *m_friendListWidget =nullptr;
    QQListWidget *m_groupListWidget =nullptr;

    QQCell *m_myFriend =nullptr;
    QQCell *m_groupCell=nullptr;
private:
    QLabel *m_logoLabel=nullptr;
    QPushButton *m_winBtn=nullptr;
    QPushButton *m_minBtn=nullptr;
    QPushButton *m_closeBtn=nullptr;

    QLabel *m_headLabel=nullptr;


    QLabel *m_weatherLabel=nullptr;

    QVBoxLayout *m_mainLay=nullptr;

    QWidget *m_topWidget=nullptr;
    QWidget *m_bottomWidget=nullptr;

    AnimationStackedWidget *m_stackWidget=nullptr;

    QToolButton *m_talkToolBtn=nullptr;
    QToolButton *m_friendToolBtn=nullptr;
    QToolButton *m_groupToolBtn=nullptr;
    QToolButton *m_appToolBtn=nullptr;

    QToolButton *m_menuToolBtn=nullptr;
    QToolButton *m_addFriendToolBtn=nullptr;
    QToolButton *m_weatherToolBtn=nullptr;
    QToolButton *m_calenderToolBtn=nullptr;

    QButtonGroup *m_btnGroup=nullptr;

    QWidget *m_talkWidget=nullptr;
    QWidget *m_contractsWidget=nullptr;
    QWidget *m_groupsWidget=nullptr;
    QWidget *m_appWidget=nullptr;


    QWidget *m_talkHeadWidget=nullptr;

    QLabel *m_talkLabel=nullptr;
    QLabel *m_talkTextLabel=nullptr;

    QLabel *m_contractsLabel=nullptr;
    QLabel *m_contractsTextLabel=nullptr;




    QLabel *m_groupsLabel=nullptr;
    QLabel *m_groupsTextLabel=nullptr;

    QLabel *m_appLabel=nullptr;
    QLabel *m_appTextLabel=nullptr;


    QSystemTrayIcon *m_sysTrayIcon=nullptr;
    QMenu *m_trayMenu=nullptr;

    bool m_bQuit = false;

    QMenu *m_addFriendMenu=nullptr;

    QMenu *m_childMenu=nullptr;

    QMenu *m_myGroupMenu=nullptr;

    CTcpSocket *m_tcpSocket=nullptr;

    // chat windows
    QList<CChatWindow *> m_chatFriendWindows;
    QList<CChatWindow *> m_chatGroupWindows;

};
#endif // MAINWINDOW_H
