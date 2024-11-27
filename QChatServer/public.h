#ifndef PUBLIC_H
#define PUBLIC_H

#include<string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

enum EnMsgType
{
    LOGIN_MSG = 1,    // 登录消息
    LOGIN_MSG_ACK,    // 登录响应消息
    LOGINOUT_MSG,     // 注销消息
    REG_MSG,          // 注册消息
    REG_MSG_ACK,      // 注册响应消息
    ONE_CHAT_MSG,     // 聊天消息
    ADD_FRIEND_MSG,   // 添加好友消息
    ADD_FRIEND_ACK,
    CREATE_GROUP_MSG, // 创建群组
    CREATE_GROUP_ACK,
    ADD_GROUP_MSG,    // 加入群组
    ADD_GROUP_ACK,
    GROUP_CHAT_MSG,   // 群聊天

    DEL_FRIEND_MSG,// delete friend
    DEL_FRIEND_ACK,// delete friend ack

    FRIEND_STATE_CHANGE,// friend online/offline
    UPDATE_SIGNATURE,
};

std::string getCurrentTime()
{
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为系统时间
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // 获取毫秒部分
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

    // 格式化时间为字符串
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << millis;

    return oss.str();
}

#endif // PUBLIC_H
