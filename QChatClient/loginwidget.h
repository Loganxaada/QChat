#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QCheckBox>
#include<QPushButton>
#include<QComboBox>
#include<QRandomGenerator>
#include"CustomContrals/flowlable.h"
#include"CustomContrals/accountcomboitemdelegate.h"
#include<QStackedWidget>
#include<QGroupBox>
#include"CustomContrals/ciplineedit.h"
#include"CustomContrals/rotatingstackedwidget.h"
#include"CustomContrals/customwidget.h"
#include"Net/ctcpsocket.h"
#include<QThread>
#include"worker.h"
#include"regwidget.h"
#include"mainwindow.h"

class Bubble
{
public:
    Bubble(int x, int y, int radius, QColor color)
        : x(x), y(y), radius(radius), color(color) {}

    int x, y;          // 气泡中心坐标
    int radius;        // 气泡半径
    QColor color;      // 气泡颜色
};

class LoginWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private:
    void initWidget();

    void initTcpSocket();

    void initConnect();

    // bool connectToServer(const QString ip,const int port);

    void setBubbleBackground();


    // 随机生成气泡
    Bubble generateRandomBubble();

    // 绘制一个气泡
    void drawBubble(QPainter &painter, const Bubble &bubble);

    // 更新气泡位置，实现流动效果
    void updateBubbles();

protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void sig_dataRecv(QByteArray data);
public slots:
    void on_cancelsetBtn_clicked();
    void on_okBtn_clicked();

    void on_minBtn_clicked();

    void on_closeBtn_clicked();

    void on_loginBtn_clicked();

    void on_loginAnimationFinished();

    void on_register(const QString&);

    void on_recvData();

    void on_ackMsg(bool bSuccess,const QByteArray &data);
private:
    CTcpSocket *m_tcpSocket=nullptr;

    RotatingStackedWidget *m_stackedWidget=nullptr;
    QWidget *m_userWidget=nullptr;
    QWidget *m_setWidget=nullptr;

    RegWidget *m_regWidget=nullptr;
    MainWindow *m_mainwindow=nullptr;

    QLabel *m_logoLael=nullptr;
    FlowLable *m_flowLabel=nullptr;
    QLabel *m_headLabel=nullptr;


    ComboBoxWithRemoveButton *m_acountCombox=nullptr;
    QLineEdit *m_passwdEdit=nullptr;


    QCheckBox *m_remberCheck=nullptr;
    QCheckBox* m_autoLoginCheck=nullptr;

    QLabel *m_registerLabel=nullptr;
    QLabel *m_forgetPwdLabel=nullptr;

    QPushButton *m_loginBtn=nullptr;

    QPushButton *m_setBtn=nullptr;
    QPushButton *m_minBtn=nullptr;
    QPushButton *m_closeBtn=nullptr;


    QLabel *m_setLabel=nullptr;

    QPushButton *m_setminBtn=nullptr;
    QPushButton *m_setcloseBtn=nullptr;

    QGroupBox *m_setGroup=nullptr;

    QLabel *m_serveripLabel=nullptr;
    CIPLineEdit *m_serveripEdit=nullptr;
    QLabel *m_serverportLabel=nullptr;
    QLineEdit *m_serverportEdit=nullptr;
    QLabel *m_fileportLabel=nullptr;
    QLineEdit *m_fileportEdit=nullptr;

    QPushButton *m_okBtn=nullptr;
    QPushButton *m_cancelBtn=nullptr;

    QList<Bubble> bubbles; // 存储所有气泡

    bool m_bConnected=false;

    QThread *m_workerThread=nullptr;
    Worker *m_worker=nullptr;

};

#endif // LOGINWIDGET_H
