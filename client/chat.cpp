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
    if(isHidden()){
        show();
    }
    m_strChatName = opposite;
    setWindowTitle("Chat With "+m_strChatName);
    if(sender == Client::getInstance().m_strLogName){
        ui->textEdit->setAlignment(Qt::AlignRight);//自己发送，在左边显示
        ui->textEdit->append(QString("'%1':'%2'\n").arg(pdu->caMsg).arg(sender));
    }else{
        ui->textEdit->setAlignment(Qt::AlignLeft);//接收消息，在右边显示
        ui->textEdit->append(QString("'%1':'%2'\n").arg(sender).arg(pdu->caMsg));
    }


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
    this->show_TEXT(Client::getInstance().m_strLogName,m_strChatName,pdu);
    Client::getInstance().sendPDU(pdu);

}
