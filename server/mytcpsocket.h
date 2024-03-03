#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include "protocol.h"
#include "msghandle.h"
#include <QTcpSocket>



class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    PDU* readPDU();
    PDU* handleMsg(PDU* pdu);
    void sendPDU(PDU* pdu);

    QString m_strLogName;
    MsgHandle* m_pmh;
private:

public slots:
    void recvMsg();
    void clientOffline();

signals:
    //发给tcpsocket的删除客户端信号
    void offline(MyTcpSocket* mytcpsocket);
};

#endif // MYTCPSOCKET_H
