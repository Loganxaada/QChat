#include "global.h"
#include<QSettings>
#include<QFile>
#include<QDir>

// 应用程序配置目录
QString Global::m_appPath         = "./";

QString Global::m_configPath        = "";


QString Global::m_strSoundPath       = "";

// 配置文件
QString Global::m_cfgIniFile         = "config.ini";


// 服务器相关配置
QString Global::m_serverIP        = "127.0.0.1";
int     Global::m_msgPort           = 60000;
int     Global::m_filePort          = 60001;

QStringList Global::m_userIDs       ={"1","2","3"};
QString Global::m_strPassword        = "123456";
QString Global::m_strHeadFile        = "head-64.png";

User Global::m_currentUser; //current login user
vector<User> Global::m_currentUserFriendList;// friend lis of current user
vector<Group> Global::m_currentUserGroupList;// group lis of current user

void Global::init(const QString &appPath)
{
    m_appPath = appPath;

    m_configPath = m_appPath+"/config/";
    m_cfgIniFile  = m_configPath + "config.ini";

    checkDirs();

    createSettingFile();

    readSettingFile();
}

void Global::checkDirs()
{
    QDir dir;


    // 配置文件目录
    dir.setPath(m_configPath);
    if (!dir.exists()) {
        dir.mkdir(m_configPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
}

string Global::getCurrentTime()
{
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
            (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
            (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}

void Global::createSettingFile()
{
    // 写入配置文件
    QSettings settings(m_cfgIniFile, QSettings::IniFormat);
    QString strGroups = settings.childGroups().join("");
    if (!QFile::exists(m_cfgIniFile) || (strGroups.isEmpty()))
    {

        /*系统设置*/
        settings.beginGroup("UserCfg");
        settings.setValue("UserIDs",   m_userIDs);

        settings.endGroup();

        /*其他配置*/
        settings.beginGroup("Server");
        settings.setValue("HostAddr", m_serverIP);
        settings.setValue("MsgPort",  m_msgPort);
        settings.setValue("FilePort",  m_filePort);

        settings.endGroup();
        settings.sync();

    }
#ifdef Q_WS_QWS
    QProcess::execute("sync");
#endif
}

void Global::readSettingFile()
{
    QSettings settings(m_cfgIniFile, QSettings::IniFormat);
    settings.beginGroup("UserCfg");
    m_userIDs = settings.value("UserIDs").toStringList();
    //m_strPassword = settings.value("Passwd", "123456")  .toString();
    settings.endGroup();


    settings.beginGroup("Server");
    m_serverIP = settings.value("HostAddr").toString();
    m_msgPort    = settings.value("MsgPort")  .toInt();
    m_filePort   = settings.value("FilePort")  .toInt();

    settings.endGroup();
}

void Global::setSettingFile(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_cfgIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.sync();
}

QVariant Global::getSettingFile(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_cfgIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    return settings.value(key, value);
}
