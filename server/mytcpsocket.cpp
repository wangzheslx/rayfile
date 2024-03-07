#include "mytcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
MyTcpSocket::MyTcpSocket()
{
    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
    connect(this,&QTcpSocket::disconnected,this,&MyTcpSocket::clientOffline);
    m_pmh = new MsgHandle;
}

MyTcpSocket::~MyTcpSocket()
{
    delete m_pmh;
}

void MyTcpSocket::recvMsg()
{
    PDU* pdu = readPDU();
    PDU* respdu = handleMsg(pdu);
    sendPDU(respdu);
    free(pdu);
    pdu = NULL;

}

void MyTcpSocket::clientOffline()
{
    //tcpsocket断开连接的槽函数
    //向数据库发送断开连接下线放置0的操作
    OperateDb::getinstance().handleoffline(m_strLogName.toStdString().c_str());
    //发送信号给断开客户端列表
    emit offline(this);
}

PDU *MyTcpSocket::readPDU()
{
    //socket处理消息
    qDebug()<<"接收消息长度"<<this->bytesAvailable();
    uint uiPDULen = 0;
    //读取协议长度
    this->read((char*)&uiPDULen,sizeof(uint));
    uint uiMSGLen = uiPDULen-sizeof(PDU);
    PDU *pdu = mkPDU(uiMSGLen);
    //继续读取剩余数据
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
           <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
          <<"数据"<<pdu->caMsg;
    return pdu;
}

PDU *MyTcpSocket::handleMsg(PDU *pdu)
{
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_REQUEST:
        return m_pmh->Regist(pdu);
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
        return m_pmh->Login(pdu,m_strLogName);
    case ENUM_MSG_TYPE_FINDUSER_REQUEST:
        return m_pmh->FindUser(pdu);
    case ENUM_MSG_TYPE_ONLINEUSER_REQUEST:
        return m_pmh->OnlineUser();
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
        return m_pmh->AddFriend(pdu);
    default:
        break;
    }
    return NULL;
}

void MyTcpSocket::sendPDU(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    this->write((char*)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu = NULL;
}
