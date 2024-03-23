#include "sahrefile.h"
#include "ui_sahrefile.h"
#include "index.h"
#include "Client.h"
SahreFile::SahreFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SahreFile)
{
    ui->setupUi(this);
}

SahreFile::~SahreFile()
{
    delete ui;
}

void SahreFile::updateFriend_LW()
{
    ui->listWidget->clear();
    QListWidget* friend_LW = Index::getinstance().getFriend()->get_friend_LW();
    for(int i = 0;i<friend_LW->count();i++){
        QListWidgetItem* friendItem = friend_LW->item(i);
        QListWidgetItem* newItem = new QListWidgetItem(*friendItem);
        ui->listWidget->addItem(newItem);
    }
}

void SahreFile::on_selectAll_clicked()
{
    for (int i =0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(true);
    }
}

void SahreFile::on_unselect_clicked()
{
    for (int i =0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(false);
    }
}

void SahreFile::on_confirm_clicked()
{
    //获取当前用户名和路径
    QString strCurPath = Index::getinstance().getFile()->m_CurPath;
    QString strPath = strCurPath+'/'+Index::getinstance().getFile()->m_strsharefile;
    QString strCurName = Client::getInstance().m_strLogName;
    //获取选择好友
    QList<QListWidgetItem *> pItems = ui->listWidget->selectedItems();
    int firendnum = pItems.size();
    //构建PDU
    qDebug()<<"strPath"<<strPath<<"strCurName"<<strCurName;
    PDU* pdu = mkPDU(32*firendnum+strPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;

    memcpy(pdu->caData,strCurName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&firendnum,sizeof(int));
    for(int i = 0;i<firendnum;i++){
        memcpy(pdu->caMsg+i*32,pItems.at(i)->text().toStdString().c_str(),32);
    }
    memcpy(pdu->caMsg+32*firendnum,strPath.toStdString().c_str(),strPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}
