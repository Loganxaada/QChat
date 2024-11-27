#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>
#include"ComAPI/json.hpp"
using json = nlohmann::json;
class CTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpSocket(QObject *parent = nullptr);
    ~CTcpSocket();

    bool connect(const QHostAddress &host, const int &port);
    bool connect(const QString &host, const int &port);


    bool send(const char *data, const qint64 len);

public slots:


};

#endif // CTCPSOCKET_H
