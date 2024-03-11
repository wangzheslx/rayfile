#include "chat.h"
#include "protocol.h"
#include "ui_chat.h"
#include "Client.h"
chat::chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);
}

void chat::show_TEXT(QString sender,QString opposite,PDU* pdu)
{
    m_strChatName = opposite;
    ui->textEdit->append(QString("'%1':'%2'").arg(sender).arg(pdu->caMsg));

}

chat::~chat()
{
    delete ui;
}

void chat::on_pushButton_clicked()
{
    QString strmsg = ui->lineEdit->text();
    if(strmsg ==""){
        return;
    }
    PDU* pdu = mkPDU(strmsg.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData,Client::getInstance().m_strLogName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,this->m_strChatName.toStdString().c_str(),32);

    memcpy(pdu->caMsg,strmsg.toStdString().c_str(),pdu->uiMsgLen);
    ui->lineEdit->clear();
    this->show_TEXT(Client::getInstance().m_strLogName,strmsg,pdu);
    Client::getInstance().sendPDU(pdu);

}
