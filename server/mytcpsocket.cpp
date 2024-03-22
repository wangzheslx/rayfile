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
    //服务器粘包处理，客户端也要处理
    QByteArray data = readAll();
    buffer.append(data);
    //处理沾包半包情况
    //先判断包是否大于一个kongpdu的情况
    while(buffer.size()>=int(sizeof(PDU))){
        PDU* pdu = (PDU*)buffer.data();
        //如果pdu不完整break继续读
        if(int(pdu->uiPDUlen)>buffer.size()){
            break;
        }
        //ok就发送
        PDU* respdu = handleMsg(pdu);
        sendPDU(respdu);
        buffer.remove(0,pdu->uiPDUlen);
    }
    //这里pdu就不用释放了，不是用户自己在堆区申请的，而是重复利用的
}

void MyTcpSocket::clientOffline()
{
    //tcpsocket断开连接的槽函数
    //向数据库发送断开连接下线放置0的操作
    OperateDb::getinstance().handleoffline(m_strLogName.toStdString().c_str());
    //发送信号给断开客户端列表
    emit offline(this);
}

//PDU *MyTcpSocket::readPDU()
//{
//    //socket处理消息
//    qDebug()<<"接收消息长度"<<this->bytesAvailable();
//    uint uiPDULen = 0;
//    //读取协议长度
//    this->read((char*)&uiPDULen,sizeof(uint));
//    uint uiMSGLen = uiPDULen-sizeof(PDU);
//    PDU *pdu = mkPDU(uiMSGLen);
//    //继续读取剩余数据
//    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
//    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
//           <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
//          <<"数据"<<pdu->caMsg;
//    return pdu;
//}

PDU *MyTcpSocket::handleMsg(PDU *pdu)
{
    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
               <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
              <<"数据"<<pdu->caMsg;
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
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:
        return m_pmh->AddFriendagree(pdu);
    case ENUM_MSG_TYPE_GET_FRIEND_REQUEST:
        return m_pmh->GetFriend(pdu);
    case ENUM_MSG_TYPE_DEL_FRIEND_REQUEST:
        return m_pmh->DelFriend(pdu);
    case ENUM_MSG_TYPE_CHAT_REQUEST:
        m_pmh->ResendChat(pdu);
        break;
    case ENUM_MSG_TYPE_MKDIR_REQUEST:
        return m_pmh->MkDir(pdu);
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
        return m_pmh->flush_file(pdu);
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
        return m_pmh->del_dir(pdu);
    case ENUM_MSG_TYPE_DEL_FILE_REQUEST:
        return m_pmh->del_file(pdu);
    case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
        return m_pmh->rename_file(pdu);
    case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:
        return m_pmh->move_file(pdu);
    case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
        return m_pmh->upload_file(pdu);
    case ENUM_MSG_TYPE_UPLOADING_FILE_REQUEST:
        return m_pmh->uploading_file(pdu);
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
