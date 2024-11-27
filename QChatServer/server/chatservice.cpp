#include "chatservice.hpp"
#include "../public.h"
#include <muduo/base/Logging.h>
#include <map>
#include<QDebug>
using namespace std;
using namespace std::placeholders;
using namespace muduo;

// 获取单例对象的接口函数
ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

// 注册消息以及对应的handler回调操作
ChatService::ChatService()
{
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({LOGINOUT_MSG, std::bind(&ChatService::loginout, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_FRIEND_ACK,std::bind(&ChatService::ackAddFriend,this,_1,_2,_3)});

    _msgHandlerMap.insert({DEL_FRIEND_MSG,std::bind(&ChatService::delFriend,this,_1,_2,_3)});

    // 群组业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3)});

     _msgHandlerMap.insert({UPDATE_SIGNATURE, std::bind(&ChatService::updateSignature, this, _1, _2, _3)});
}

// 处理登录业务 ORM 业务层操作的都是对象 DAO数据层
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    //{"msgid":1,"id":1,"password":"123456"}
    //{"msgid":1,"id":2,"password":"666666"}
    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = _userModel.query(id);
    if (user.getId() == id && user.getPwd() == pwd)
    {
        if (user.getState() == "online")
        {
            // 该用户已登录，不允许重复登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "Duplicate logins are not allowed";
            conn->send(response.dump());
        }
        else
        {
            // 登录成功,更新用户状态信息 offline -> online
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({id, conn});
            }

            user.setState("online");
            _userModel.updateState(user);

            notifyFriendsStateChange(user.getId(),user.getState());

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            response["state"]=user.getState();
            response["signature"]=user.getSignture();

            // 查询该用户是否有离线消息
            vector<string> vec = _offlineMsgModel.query(id);
            if (!vec.empty())
            {
                response["offlinemsg"] = vec;
                // 读取用户的离线消息后，把该用户的所有离线消息删除掉
                _offlineMsgModel.remove(id);
            }

            // 查询该用户的好友信息并返回
            vector<User> userVec = _friendModel.query(id);
            if (!userVec.empty())
            {
                vector<string> vec2;
                for (User &user : userVec)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    js["signature"]=user.getSignture();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            // 查询用户的群组信息
            vector<Group> groupuserVec = _groupModel.queryGroups(id);
            if (!groupuserVec.empty())
            {
                // group:[{groupid:[xxx, xxx, xxx, xxx]}]
                vector<string> groupV;
                for (Group &group : groupuserVec)
                {
                    json grpjson;
                    grpjson["id"] = group.getId();
                    grpjson["groupname"] = group.getName();
                    grpjson["groupdesc"] = group.getDesc();
                    vector<string> userV;
                    for (GroupUser &user : group.getUsers())
                    {
                        json js;
                        js["id"] = user.getId();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    grpjson["users"] = userV;
                    groupV.push_back(grpjson.dump());
                }

                response["groups"] = groupV;
            }

            conn->send(response.dump());
        }
    }
    else
    {
        // 该用户不存在,用户存在但是密码错误 登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "username or password error!!!";
        conn->send(response.dump());
    }
}

// 处理注册业务
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    //{"msgid":3,"name":"li si","password":"666666"}
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _userModel.insert(user);
    if (state)
    {
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();

        conn->send(response.dump());
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["id"] = user.getId();

        conn->send(response.dump());
    }
}

// 一对一聊天业务
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    qDebug()<<__FUNCTION__;
    //{"msgid":5,"from":1,"name":"zhang san","to":2,"msg":"hello, li si"}
    //{"msgid":5,"from":2,"name":"li si","to":1,"msg":"hello, zhang san"}
    int toid = js["toid"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end())
        {
            // toid在线 转发消息 服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }
    // toid不在线 存储离线消息
    _offlineMsgModel.insert(toid, js.dump());
}

// 添加好友业务 msgid id friendid
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{

    int friendid = js["friendid"].get<int>();
    qDebug()<<__FUNCTION__<<friendid;

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(friendid);
        if (it != _userConnMap.end())
        {
            // toid在线 转发消息 服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }

    _offlineMsgModel.insert(friendid, js.dump());

}

void ChatService::ackAddFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();
    qDebug()<<userid<<friendid;
    // 存储好友信息
    bool bAddFriend = _friendModel.insert(userid, friendid);
    if(bAddFriend){
        json responsejs;
        responsejs["msgid"]=ADD_FRIEND_ACK;
        responsejs["id"]=userid;
        responsejs["friendid"]=friendid;

        User user = _userModel.query(userid);
        User user2 = _userModel.query(friendid);

        json js1;
        js1["id"]=user.getId();
        js1["name"]=user.getName();
        js1["state"]=user.getState();
        js1["signature"]=user.getSignture();
        responsejs["user"]=js1;


        json js2;
        js2["id"]=user2.getId();
        js2["name"]=user2.getName();
        js2["state"]=user2.getState();
        js2["signature"]=user2.getSignture();
        responsejs["friend"]=js2;


        {
            lock_guard<mutex> lock(_connMutex);

            // 给 `userid` 发送消息
            auto it = _userConnMap.find(userid);
            if (it != _userConnMap.end())
            {
                // `userid` 在线，直接发送消息
                qDebug() << __FUNCTION__ << "Sending to userid:" << userid;
                it->second->send(responsejs.dump());
            }


            // 给 `friendid` 发送消息
            auto it2 = _userConnMap.find(friendid);
            if (it2 != _userConnMap.end())
            {
                // `friendid` 在线，直接发送消息
                qDebug() << __FUNCTION__ << "Sending to friendid:" << friendid;
                it2->second->send(responsejs.dump());
            }

        }

        return;


    }


}

void ChatService::delFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["userid"].get<int>();
    int friendid = js["friendid"].get<int>();

    bool bDel = _friendModel.delFriend(userid,friendid);

    json reponsejs;
    reponsejs["msgid"]=DEL_FRIEND_ACK;
    reponsejs["delstate"]=bDel;
    reponsejs["delid"]=friendid;

    qDebug()<<__FUNCTION__;
    conn->send(reponsejs.dump());
}

// 创建群组
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    qDebug()<<__FUNCTION__;
    int userid = js["id"].get<int>();
    string name = js["groupname"];
    string desc = js["groupdesc"];

    // 存储新创建的群组信息
    Group group(-1, name, desc);
    if (_groupModel.createGroup(group))
    {
        // 存储群组创建人信息
        if(_groupModel.addGroup(userid, group.getId(), "creator"))
        {
            js["msgid"]=CREATE_GROUP_ACK;
            js["groupid"]=group.getId();
            conn->send(js.dump());
        }
    }
}

// 加入群组业务
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    qDebug() << __FUNCTION__;
       int userid = js["id"].get<int>();
       int groupid = js["groupid"].get<int>();

       // 尝试将用户添加到群组
       if (_groupModel.addGroup(userid, groupid, "normal")) {
           qDebug() << __FUNCTION__ << "_groupModel.addGroup" << groupid;

           // 获取群组信息
           Group group = _groupModel.queryGroupById(groupid);

           // 准备返回给客户端的响应数据
           json groupjson;
           groupjson["msgid"] = ADD_GROUP_ACK;
           groupjson["groupid"] = groupid;
           groupjson["groupname"] = group.getName();
           groupjson["groupdesc"] = group.getDesc();

           // 创建一个数组存储群组成员信息
           json users = json::array();
           for (GroupUser &user : group.getUsers()) {
               json userJson;
               userJson["id"] = user.getId();
               qDebug()<<"userJsonid"<<user.getId();
               userJson["name"] = user.getName();
               userJson["state"] = user.getState();
               userJson["role"] = user.getRole();
               users.push_back(userJson);  // 直接将用户的json对象放入数组
           }
           groupjson["users"] = users;

           // 发送群组信息给客户端
           conn->send(groupjson.dump());
       } else {
           // 如果添加群组失败，返回错误消息
           json errorResponse;
           errorResponse["msgid"] = ADD_GROUP_ACK;
           errorResponse["status"] = "fail";
           errorResponse["error"] = "Failed to add user to group.";
           conn->send(errorResponse.dump());
       }
}


// 群组聊天业务
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    qDebug()<<__FUNCTION__;
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    vector<int> useridVec = _groupModel.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(_connMutex);
    for (int id : useridVec)
    {
        auto it = _userConnMap.find(id);
        if (it != _userConnMap.end())
        {
            // 转发群消息
            it->second->send(js.dump());
        }
        else
        {
            // 存储离线群消息
            _offlineMsgModel.insert(id, js.dump());
        }
    }
}

void ChatService::updateSignature(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["userid"].get<int>();
    string signature = js["signature"];

    _userModel.updateSignature(userid,signature);
}

// 处理注销业务
void ChatService::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(userid);
        if (it != _userConnMap.end())
        {
            _userConnMap.erase(it);
        }

        // 更新用户的状态信息
        User user(userid, "", "", "offline");
        _userModel.updateState(user);
    }
}

// 获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid)
{
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end())
    {
        // 返回一个默认的处理器，空操作
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time)
        {
            LOG_ERROR << "msgid: " << msgid << " can not find handler";
        };
    }
    else
    {
        return _msgHandlerMap[msgid];
    }
}

void ChatService::notifyFriendsStateChange(int userid, const string &state)
{
    vector<User> friendList = _friendModel.query(userid);

    lock_guard<mutex> lock(_connMutex);
    for (User &friendUser : friendList)
    {
        auto it = _userConnMap.find(friendUser.getId());
        if (it != _userConnMap.end())
        {
            json stateChangeMsg;
            stateChangeMsg["msgid"] = FRIEND_STATE_CHANGE;
            stateChangeMsg["userid"] = userid;
            stateChangeMsg["state"] = state;

            it->second->send(stateChangeMsg.dump());
        }
    }
}

// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    //{"msgid":1,"id":1,"password":"123456"}
    User user;
    {
        lock_guard<mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it)
        {
            if (it->second == conn)
            {
                // 从map表删除用户的连接信息
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }

    // 更新用户的状态信息
    if (user.getId() != -1)
    {
        user.setState("offline");
        _userModel.updateState(user);
    }

    notifyFriendsStateChange(user.getId(),user.getState());
}

// 服务器异常，业务重置方法
void ChatService::reset()
{
    // 把online状态用户，设置成offline
    _userModel.resetState();
}
