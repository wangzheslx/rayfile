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
        QMessageBox::information(Index::getinstance().getFriend(),"搜索",QString("%1用户存在并且在线").arg(name),"添加好友","取消");
        QString strCurname = Client::getInstance().m_strLogName;
        PDU*pdu = mkPDU(0);
        pdu->uiMsgType =ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
        memcpy(pdu->caData,strCurname.toStdString().c_str(),32);
        memcpy(pdu->caData+32,name,32);
        Client::getInstance().sendPDU(pdu);
    }else if(ret == 0){
        QMessageBox::information(Index::getinstance().getFriend(),"搜索",QString("%1用户存在但是不在线").arg(name));
    }else if(ret == -1){
        QMessageBox::critical(Index::getinstance().getFriend(),"搜索","没有查到此用户");
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

void ResHandler::addfriendreturn(PDU *pdu)
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    qDebug()<<ret;
    if(ret == -1){
        QMessageBox::warning(Index::getinstance().getFriend(),"添加好友","添加错误");
        return;
    }
    if(ret == 0){
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","用户不在线");
        return;
    }
    if(ret == -2){
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","该用户已经是你的好友");
        return;
    }
    //发送请求
    QMessageBox::information(Index::getinstance().getFriend(),"添加好友","添加好友请求已发送，等待对方同意");
}

void ResHandler::requestaddfriend(PDU *pdu)
{
    char applyname[32] = {'\0'};
    memcpy(applyname,pdu->caData,32);
    int ret = QMessageBox::question(Index::getinstance().getFriend(),"添加好友请求",QString("%1请求添加你为好友，是否同意").arg(applyname));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;
    memcpy(respdu->caData,pdu->caData,64);
    Client::getInstance().sendPDU(respdu);
}

void ResHandler::respondaddfriend()
{
    QMessageBox::information(Index::getinstance().getFriend(),"添加好友","添加好友成功");
    Index::getinstance().getFriend()->flushFriend();
}

void ResHandler::getfriend(PDU *pdu)
{
    uint usersize = pdu->uiMsgLen/32;
    QStringList friendlist;
    for(uint i = 0;i < usersize;i++){
        char name[32] = {'\0'};
        memcpy(name,pdu->caData+i*32,32);
        friendlist.append(QString(name));
    }
    Index::getinstance().getFriend()->showfriend(friendlist);
}

void ResHandler::delfriend(PDU *pdu)
{
    bool flag;
    memcpy(&flag,pdu->caData,sizeof(bool));
    if(flag){
        QMessageBox::information(Index::getinstance().getFriend(),"删除好友","成功");

    }else{
        QMessageBox::warning(Index::getinstance().getFriend(),"删除好友","失败");
    }
    Index::getinstance().getFriend()->flushFriend();
}

void ResHandler::updatechat(PDU *pdu, bool self)
{
    if(Index::getinstance().getFriend()->getchat()->isHidden()){
        Index::getinstance().getFriend()->getchat()->show();
    }
    QString sender;
    QString opposite;
    char curname[32] = {'\0'};
    char tarname[32] = {'\0'};
    memcpy(curname,pdu->caData,32);
    memcpy(tarname,pdu->caData+32,32);
    if(self){
        sender = QString(curname);
        opposite = QString(opposite);
    }else{
       sender = QString(curname);
       opposite = QString(curname);
    }
    Index::getinstance().getFriend()->getchat()->show_TEXT(sender,opposite,pdu);

}



