#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include<QMetaType>
#include"ComAPI/json.hpp"
using json = nlohmann::json;

    // 注册 json 类型

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    // 处理登录的响应逻辑
    void doLoginResponse(json &responsejs);

    // 处理注册的响应逻辑
    void doRegResponse(json &responsejs);

    void doDelFriendResponse(json &responsejs);

    void doFriendStateChange(json &responsejs);

    void doRecvFriendMsg(json &responsejs);

    void doGroupChatMsg(json &responsejs);

    void doAddFriend(json &responsejs);

    void doAckAddFriend(json &responsejs);
    void doCreateGroup(json &responsejs);

    void doAckAddGroup(json &responsejs);

signals:
    void sig_LoginState(bool bSuccess,const QByteArray &data);

    void sig_regState(int id =-1);

    void sig_friendStateChange(int id,QString state);

    void sig_friendMsg(json responsejs);
    void sig_groupMsg(json responsejs);

    void sig_offlineMsg(json js);

    void sig_addFriend(json js);

    void sig_ackAddFriend(json js);

     void sig_createGroup(json js);

     void sig_ackAddGroup(json js);

public slots:
    void on_processData(const QByteArray &data);
};

#endif // WORKER_H
