#include "worker.h"
#include"ComAPI/MyUnit.h"
#include<QDebug>
#include"ComAPI/global.h"
#include<iostream>

Q_DECLARE_METATYPE(json)

Worker::Worker(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<json>("json");
}

void Worker::doLoginResponse(json &responsejs)
{
    if (0 != responsejs["errno"].get<int>()) // 登录失败
    {
        QString errmsg = QString::fromStdString(responsejs["errmsg"]);
        qDebug()<< errmsg;
        emit sig_LoginState(false,errmsg.toLatin1());
    }
    else // 登录成功
    {
        std::string name = responsejs["name"];
        int id = responsejs["id"];
        std::string signature = responsejs["signature"];
        std::string state = responsejs["state"];
        QString loginMsg = QString::number(id)+" "+ QString::fromStdString(name)+" login success!";
        qDebug()<<  loginMsg;
        emit sig_LoginState(true,loginMsg.toLatin1());


        Global::m_currentUser.setId(id);
        Global::m_currentUser.setName(name);
        Global::m_currentUser.setState(state);
        Global::m_currentUser.setSignature(signature);

        // 记录当前用户的好友列表信息
        if (responsejs.contains("friends"))
        {
            // 初始化
            Global::m_currentUserFriendList.clear();

            vector<string> vec = responsejs["friends"];
            for (string &str : vec)
            {
                json js = json::parse(str);
                User user;
                user.setId(js["id"].get<int>());
                user.setName(js["name"]);
                user.setState(js["state"]);
                user.setSignature(js["signature"]);
                std::string sign =js["signature"];

                Global::m_currentUserFriendList.push_back(user);
            }
        }

        // 记录当前用户的群组列表信息
        if (responsejs.contains("groups"))
        {
            // 初始化
            Global::m_currentUserGroupList.clear();

            vector<string> vec1 = responsejs["groups"];
            for (string &groupstr : vec1)
            {
                json grpjs = json::parse(groupstr);
                Group group;
                group.setId(grpjs["id"].get<int>());
                group.setName(grpjs["groupname"]);
                group.setDesc(grpjs["groupdesc"]);

                vector<string> vec2 = grpjs["users"];
                for (string &userstr : vec2)
                {
                    GroupUser user;
                    json js = json::parse(userstr);
                    user.setId(js["id"].get<int>());
                    user.setName(js["name"]);
                    user.setState(js["state"]);
                    user.setRole(js["role"]);
                    group.getUsers().push_back(user);
                }

                Global::m_currentUserGroupList.push_back(group);


            }
        }

        // 显示当前用户的离线消息  个人聊天信息或者群组消息
        if (responsejs.contains("offlinemsg"))
        {
            vector<string> vec = responsejs["offlinemsg"];
            for (string &str : vec)
            {
                json js = json::parse(str);
                // time + [id] + name + " said: " + xxx
                if (ONE_CHAT_MSG == js["msgid"].get<int>())
                {
                    //                    std::cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                    //                                                     << " said: " << js["msg"].get<string>() << endl;
                    emit sig_offlineMsg(js);
                    qDebug()<<"emit sig_offlineMsg(js)";
                }
                else
                {
                    //                    cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                    //                         << " said: " << js["msg"].get<string>() << endl;
                    emit sig_offlineMsg(js);

                }
            }
        }

    }
}

void Worker::doRegResponse(json &responsejs)
{
    if (0 != responsejs["errno"].get<int>()) // 注册失败
    {
        qDebug() << "name is already exist, register error!" ;
        emit sig_regState();
    }
    else // 注册成功
    {
        int id = responsejs["id"];
        qDebug() << "Register success, userid is " << id << ", do not forget it!" ;
        emit sig_regState(id);
    }

}

void Worker::doDelFriendResponse(json &responsejs)
{
    bool bDel = responsejs["delstate"].get<bool>();
    int id = responsejs["delid"].get<int>();
    if(bDel)
    {
        for (auto it = Global::m_currentUserFriendList.begin(); it != Global::m_currentUserFriendList.end(); ++it) {
            if (it->getId() == id) {
                Global::m_currentUserFriendList.erase(it); // 按迭代器位置删除
                break;           // 删除后迭代器失效，需要退出循环
            }
        }

    }

}

void Worker::doFriendStateChange(json &responsejs)
{
    //int msgid = responsejs["msgid"].get<int>();
    int userid =responsejs["userid"].get<int>();
    std::string state = responsejs["state"];

    for (auto it = Global::m_currentUserFriendList.begin(); it != Global::m_currentUserFriendList.end(); ++it) {
        if (it->getId() == userid) {
            it->setState(state);
            emit sig_friendStateChange(userid,QString::fromStdString(state));
            break;
        }
    }
}

void Worker::doRecvFriendMsg(json &responsejs)
{
    //{"msgid":5,"from":2,"name":"li si","to":1,"msg":"hello, zhang san"}
    emit sig_friendMsg(responsejs);
}

void Worker::doGroupChatMsg(json &responsejs)
{
    emit sig_groupMsg(responsejs);
}

void Worker::doAddFriend(json &responsejs)
{
    emit sig_addFriend(responsejs);
}

void Worker::doAckAddFriend(json &responsejs)
{
    emit sig_ackAddFriend(responsejs);
}

void Worker::doCreateGroup(json &responsejs)
{
    emit sig_createGroup(responsejs);
}

void Worker::doAckAddGroup(json &responsejs)
{
    emit sig_ackAddGroup(responsejs);
}

void Worker::on_processData(const QByteArray &data)
{
    QByteArray processedData = data.toUpper();
    //emit sig_LoginState(processedData);

    if (data.isEmpty())
    {
        //emit connectionClosed();
        return;
    }

    json js = json::parse(data.data());
    int msgtype = js["msgid"].get<int>();

    switch (msgtype)
    {
    case LOGIN_MSG_ACK:
    {
        doLoginResponse(js);
    }
        break;

    case ONE_CHAT_MSG:
    {
        doRecvFriendMsg(js);

    }
        break;
    case GROUP_CHAT_MSG:
    {
        doGroupChatMsg(js);
    }
        break;
    case CREATE_GROUP_ACK:
    {
        doCreateGroup(js);
    }
        break;

    case ADD_FRIEND_MSG:
    {
        doAddFriend(js);
    }
        break;
    case ADD_FRIEND_ACK:
    {
        doAckAddFriend(js);
    }
        break;
    case ADD_GROUP_ACK:
    {
        doAckAddGroup(js);
    }
        break;
    case REG_MSG_ACK:
    {
        doRegResponse(js);
    }
        //            //rwsem.release(); // 通知主线程注册完成
        break;
    case DEL_FRIEND_ACK:
    {
        doDelFriendResponse(js);
        qDebug()<<__FUNCTION__<<"DEL_FRIEND_ACK";
    }
        break;
    case FRIEND_STATE_CHANGE:
    {
        qDebug()<<__FUNCTION__<<"FRIEND_STATE_CHANGE";
        doFriendStateChange(js);

    }
        break;
    default:
        qDebug() << "Unknown message type:" << msgtype;
        break;
    }

}
