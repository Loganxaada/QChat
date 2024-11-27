#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QVariant>
#include "user.h"
#include "group.h"

class Global
{
public:
    static Global *getInstance()
    {
        return &instance;
    }

    //=======================系统配置部分=========================//
    static QString m_appPath; // 应用程序路径

    static QString m_configPath;   // 配置目录
    static QString m_strSoundPath; // 配置目录

    static QString m_cfgIniFile; // 配置文件

    static QString m_serverIP; // 服务器地址配置
    static int m_msgPort;      // 聊天消息服务器端口配置
    static int m_filePort;     // 文件转发服务器端口配置
    static int m_nGroupPort;   // 群组聊天窗口

    static QStringList m_userIDs; // 用户名
    static QString m_strPassword; // 用户密码
    static QString m_strHeadFile; // 头像文件

    static User m_currentUser;                   // current login user
    static vector<User> m_currentUserFriendList; // friend lis of current user
    static vector<Group> m_currentUserGroupList; // group lis of current user

    static void init(const QString &appPath);

    static void checkDirs();

    // 获取系统时间（聊天信息需要添加时间信息）
    static string getCurrentTime();

    // 创建配置文件
    static void createSettingFile();
    // 读配置文件，加载系统配置
    static void readSettingFile();
    // 读取/修改配置文件
    static void setSettingFile(const QString &group, const QString &key, const QVariant &value);

    static QVariant getSettingFile(const QString &group, const QString &key, const QVariant &value = QVariant());

private:
    Global() {}
    static Global instance; // 饿汉式

    // 禁用复制和赋值
    Global(const Global &) = delete;
    Global &operator=(const Global &) = delete;
};

#endif // GLOBAL_H
