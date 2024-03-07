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
}

OnlineUser *Friend::getonlineuser()
{
    return m_onlineuser;
}

Friend::~Friend()
{
    delete ui;
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
