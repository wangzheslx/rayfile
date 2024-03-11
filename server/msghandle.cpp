#include "msghandle.h"
#include "mytcpserver.h"
#include "operatedb.h"
#include<QDebug>
MsgHandle::MsgHandle()
{

}

PDU* MsgHandle::Regist(PDU *pdu)
{
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
    return respdu;
}

PDU *MsgHandle::Login(PDU *pdu, QString &strusername)
{
    qDebug()<<"服务器用户登录请求实现";
    char strname[32] = {'\0'};
    char strpwd[32] = {'\0'};
    memcpy(strname,pdu->caData,32);
    memcpy(strpwd,pdu->caData+32,32);
    bool ret =  OperateDb::getinstance().handleLogin(strname,strpwd);
    if(ret){
        qDebug()<<"登录成功";
    }else{
        qDebug()<<"登录失败";
    }
    //构建返回的pdu
    PDU * respdu = mkPDU(0);
    strusername = strname;
    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::FindUser(PDU *pdu)
{
    qDebug()<<"服务器用户查找请求实现";
    char strname[32] = {'\0'};
    memcpy(strname,pdu->caData,32);
    int ret =  OperateDb::getinstance().handleFindUser(strname);

    //构建返回的pdu
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_FINDUSER_RESPEND;
    memcpy(respdu->caData,strname,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandle::OnlineUser()
{
    qDebug()<<"服务器用户在线显示请求实现";
    QStringList result = OperateDb::getinstance().handleOnlineUser();
    PDU * respdu = mkPDU(result.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_ONLINEUSER_RESPEND;
    for(int i = 0;i<result.size();i++){
        memcpy(respdu->caData+i*32,result.at(i).toStdString().c_str(),32);
        qDebug()<<"在线用户 "<<i+1<< ":" << result.at(i);
    }
    return respdu;
}

PDU *MsgHandle::AddFriend(PDU* pdu)
{
    qDebug()<<"服务器用户添加好友请求";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};


    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);

    int ret = OperateDb::getinstance().handleAddFriend(caCurName,caTarName);
    if(ret == 1){
        MyTcpServer::getInstance().resend(caTarName,pdu);
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;

}

PDU *MsgHandle::AddFriendagree(PDU *pdu)
{
    qDebug()<<"有好友同意请求消息";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    OperateDb::getinstance().handleAgreefriend(caCurName,caTarName);
    PDU*respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND;
    MyTcpServer::getInstance().resend(caCurName,respdu);
    return respdu;
}

PDU *MsgHandle::GetFriend(PDU *pdu)
{
    qDebug()<<"查找好友用户";
    char Curname[32] = {'\0'};
    memcpy(Curname,pdu->caData,32);
    QStringList result = OperateDb::getinstance().handleGetfriend(Curname);
    PDU * respdu = mkPDU(result.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_GET_FRIEND_RESPEND;
    for(int i = 0;i<result.size();i++){
        memcpy(respdu->caData+i*32,result.at(i).toStdString().c_str(),32);
        qDebug()<<"好友 "<<i+1<< ":" << result.at(i);
    }
    return respdu;

}

PDU *MsgHandle::DelFriend(PDU *pdu)
{
    qDebug()<<"删除好友";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    bool flag = OperateDb::getinstance().handleDelFriend(caCurName,caTarName);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_RESPEND;
    memcpy(respdu->caData,&flag,sizeof(bool));
    return respdu;
}
