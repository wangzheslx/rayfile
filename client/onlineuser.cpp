#include "Client.h"
#include "onlineuser.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::showOnlineUser(QStringList mlist)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(mlist);
}

void OnlineUser::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"添加好友";
    QString strCurname =Client::getInstance().m_strLogName;
    QString strTarname = item->text();
    //在线用户添加好友功能实现
    PDU*pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData,strCurname.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarname.toStdString().c_str(),32);
    Client::getInstance().sendPDU(pdu);
}
