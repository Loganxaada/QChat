#include "ctcpsocket.h"
#include <QNetworkProxy>
#include <QDebug>

CTcpSocket::CTcpSocket(QObject *parent)
    : QTcpSocket(parent)
{
    // 设置代理为 NoProxy（如果需要）
    QNetworkProxy proxy(QNetworkProxy::NoProxy);
    setProxy(proxy);
}

CTcpSocket::~CTcpSocket()
{
    close(); // 确保资源释放
}

bool CTcpSocket::connect(const QHostAddress &host, const int &port)
{
    if (state() == QAbstractSocket::UnconnectedState) {
        connectToHost(host, port);
        qDebug() << __FUNCTION__ << "Connecting to" << host << ":" << port;

        // 等待连接完成
        if (!waitForConnected(5000)) { // 超时为 5000ms
            qDebug() << __FUNCTION__ << "Connection failed:" << errorString();
            return false;
        }
    } else {
        qDebug() << __FUNCTION__ << "Already connecting or connected. Current state:" << state();
    }

    if (state() == QAbstractSocket::ConnectedState) {
        qDebug() << __FUNCTION__ << "Connection established successfully.";
        return true;
    } else {
        qDebug() << __FUNCTION__ << "Failed to establish connection. Error:" << errorString();
        return false;
    }
}

bool CTcpSocket::connect(const QString &host, const int &port)
{
    if (state() == QAbstractSocket::UnconnectedState) {
        connectToHost(host, port);
        qDebug() << __FUNCTION__ << "Connecting to" << host << ":" << port;

        // 等待连接完成
        if (!waitForConnected(5000)) { // 超时为 5000ms
            qDebug() << __FUNCTION__ << "Connection failed:" << errorString();
            return false;
        }
    } else {
        qDebug() << __FUNCTION__ << "Already connecting or connected. Current state:" << state();
    }

    if (state() == QAbstractSocket::ConnectedState) {
        qDebug() << __FUNCTION__ << "Connection established successfully.";
        return true;
    } else {
        qDebug() << __FUNCTION__ << "Failed to establish connection. Error:" << errorString();
        return false;
    }
}



bool CTcpSocket::send(const char *data, const qint64 len)
{
    //    qint64 sendSize = write(data,len);
    //    if(-1 == sendSize){
    //        qDebug()<<"tcpsocket send error"<<errorString();
    //    }
    return write(data,len);
}


