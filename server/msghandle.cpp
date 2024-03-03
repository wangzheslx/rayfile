#include "msghandle.h"
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
