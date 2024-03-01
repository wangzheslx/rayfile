#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>



class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString m_strLogName;
private:

public slots:
    void recvMsg();
    void clientOffline();
signals:
    //发给tcpsocket的删除客户端信号
    void offline(MyTcpSocket* mytcpsocket);
};

#endif // MYTCPSOCKET_H
