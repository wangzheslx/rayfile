#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "mytcpsocket.h"

#include <QTcpServer>


class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) override;
    void resend(char *tarName,PDU* pdu);
public slots:
    void deleteSocket(MyTcpSocket * mytcpsocket);
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer&) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<MyTcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
