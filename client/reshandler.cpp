#include "reshandler.h"

#include "Client.h"
#include "index.h"

#include <QMessageBox>


ResHandler::ResHandler()
{

}


void ResHandler::regist(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&Client::getInstance(),"注册","注册成功");
    }else{
        QMessageBox::information(&Client::getInstance(),"注册","注册失败，使用非法的名称或密码");
    }

}

void ResHandler::login(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&Client::getInstance(),"登录","登录成功");
        Index::getinstance().show();
        Client::getInstance().hide();
    }else{
        QMessageBox::information(&Client::getInstance(),"登录","登录失败，使用错误的名称或密码");
    }
}

void ResHandler::finduser(PDU *pdu)
{
    char name[32] = {'\0'};
    int ret;
    memcpy(name,pdu->caData,32);
    memcpy(&ret,pdu->caData+32,sizeof(int));
    if(ret == 1){
        QMessageBox::information(&Client::getInstance(),"搜索",QString("%1用户存在并且在线").arg(name));
    }else if(ret == 0){
        QMessageBox::information(&Client::getInstance(),"搜索",QString("%1用户存在但是不在线").arg(name));
    }else if(ret == -1){
        QMessageBox::critical(&Client::getInstance(),"搜索","没有查到此用户");
    }
}

void ResHandler::onlineuser(PDU *pdu)
{
    uint usersize = pdu->uiMsgLen/32;
    QStringList userlist;
    for(uint i = 0 ; i < usersize;i++){
        char name[32] = {'\0'};
        memcpy(name,pdu->caData+i*32,32);
        if(QString(name) == Client::getInstance().m_strLogName){
            continue;
        }
        userlist.append(QString(name));
    }
    Index::getinstance().getFriend()->getonlineuser()->showOnlineUser(userlist);
}

