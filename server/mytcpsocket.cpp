#include "mytcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
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
           <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
          <<"数据"<<pdu->caMsg;
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_REQUEST:{
        qDebug()<<"服务器用户注册请求实现";
        char strname[32] = {'\0'};
        char strpwd[32] = {'\0'};
        memcpy(strname,pdu->caData,32);
        memcpy(strpwd,pdu->caData+32,32);
        bool ret =  OperateDb::getinstance().handleRegist(strname,strpwd);
        if(ret){
            qDebug()<<"注册成功";
        }else{
            qDebug()<<"注册失败";
        }
        //构建返回的pdu
        PDU * respdu = mkPDU(0);

        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPEND;
        memcpy(respdu->caData,&ret,sizeof(bool));
        this->write((char*)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu = NULL;
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}
