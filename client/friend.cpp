#include "Client.h"
#include "friend.h"
#include "protocol.h"
#include "ui_friend.h"
#include<QDebug>
#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_onlineuser = new OnlineUser;
    m_chat = new chat;
    flushFriend();
}

OnlineUser *Friend::getonlineuser()
{
    return m_onlineuser;
}

chat *Friend::getchat()
{
    return m_chat;
}

void Friend::getFriendRequest()
{
//    QString Curname = Client::getInstance().m_strLogName;
//    PDU* pdu =
    //    QStringList friendlist
}

void Friend::showfriend(QStringList list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(list);
}

void Friend::flushFriend()
{
    qDebug()<<"刷新好友";
    QString Curname = Client::getInstance().m_strLogName;
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GET_FRIEND_REQUEST;
    memcpy(pdu->caData,Curname.toStdString().c_str(),32);
    Client::getInstance().sendPDU(pdu);
}



Friend::~Friend()
{
    delete ui;
    delete m_chat;
    delete m_onlineuser;
}

void Friend::on_findUser_PB_clicked()
{
    //好友界面查找用户按钮功能实现
    bool ok;
    QString strname =  QInputDialog::getText(this,"查找用户","用户名",QLineEdit::Normal,QString(),&ok);
    qDebug()<<"查找用户 on_findUser_PB_clicked()";
    if(ok){
        if(strname.isEmpty()){
            QMessageBox::information(this,"用户名","用户名为空");
            return;
        }
        qDebug()<<"所查找的用户名为："<<strname;
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_FINDUSER_REQUEST;
        memcpy(pdu->caData,strname.toStdString().c_str(),strname.size());
        Client::getInstance().sendPDU(pdu);
    }

}

void Friend::on_onlineUser_PB_clicked()
{
    //好友界面显示所有在线用户界面
    PDU*pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ONLINEUSER_REQUEST;
    Client::getInstance().sendPDU(pdu);
    qDebug()<<"好友界面显示所有在线用户界面";
    if(m_onlineuser->isHidden()){
        m_onlineuser->show();
    }
}

void Friend::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{


}

void Friend::on_flushFriend_PB_clicked()
{
    flushFriend();

}

void Friend::on_delFriend_PB_clicked()
{
    QListWidgetItem * pItem = ui->listWidget->currentItem();
    if(!pItem){
        QMessageBox::information(this,"删除好友","请选择要删除的好友");
        return;
    }
    QString strname = pItem->text();
    int ret = QMessageBox::question(this,"删除好友",QString("是否删除好友'%1'").arg(strname));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* pdu = mkPDU(0);
    QString curname = Client::getInstance().m_strLogName;
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_REQUEST;
    memcpy(pdu->caData,curname.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strname.toStdString().c_str(),32);
    Client::getInstance().sendPDU(pdu);
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem * pItem = ui->listWidget->currentItem();
    if(!pItem){
        QMessageBox::information(this,"聊天","请选择要聊天的好友");
        return;
    }
    m_chat->m_strChatName = pItem->text();
    if(m_chat->isHidden()){
        m_chat->show();
    }

}
