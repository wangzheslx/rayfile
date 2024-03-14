#include "Client.h"
#include "file.h"
#include "ui_file.h"

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_CurPath = QString("%1/%2")
            .arg(Client::getInstance().getrootpath())
            .arg(Client::getInstance().m_strLogName);
    m_RootPath = m_CurPath;
}

File::~File()
{
    delete ui;
}

void File::on_mkDir_PB_clicked()
{
    qDebug()<<"创建文件夹";
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","文件夹名:");
    qDebug()<<strNewDir;
    if(strNewDir == NULL || strNewDir.size()>32){
        QMessageBox::warning(this,"新建文件夹","文件名非法");
        return ;
    }
    qDebug()<<m_CurPath.size()<<" "<<m_CurPath;
    PDU* pdu = mkPDU(m_CurPath.size());
    pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_CurPath.toStdString().c_str(),m_CurPath.size());
    Client::getInstance().sendPDU(pdu);
}
