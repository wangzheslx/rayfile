#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>


class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) override;


private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer&) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<QTcpServer*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
