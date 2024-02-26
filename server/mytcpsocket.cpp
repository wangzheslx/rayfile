#include "mytcpsocket.h"
#include<protocol.h>

MyTcpSocket::MyTcpSocket()
{
    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<"接收消息长度"<<this->bytesAvailable();
    uint uiPDULen = 0;
    //读取协议长度
    this->read((char*)&uiPDULen,sizeof(uint));
    uint uiMSGLen = uiPDULen-sizeof(PDU);
    PDU *pdu = mkPDU(uiMSGLen);
    //继续读取剩余数据
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
           <<"参数"<<pdu->caData<<endl
          <<"数据"<<pdu->caMsg;
}
