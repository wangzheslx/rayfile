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
    flush_file();
}

void File::flush_file()
{
    qDebug()<<"刷新文件请求";
    PDU* pdu = mkPDU(m_CurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg,m_CurPath.toStdString().c_str(),m_CurPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}

void File::updateFileList(QList<FileInfo *> pFileList)
{
    //释放并更新列表
    foreach(FileInfo* pFileinfo,m_fileList){
        delete pFileinfo;
    }
    m_fileList.clear();
    m_fileList = pFileList;

    //更新ui界面
    ui->listWidget->clear();
    foreach(FileInfo* pFileinfo,m_fileList){
        QListWidgetItem* pItem = new QListWidgetItem;
        if(pFileinfo->iFileType == 0){
            pItem->setIcon(QIcon(QPixmap(":/icon_dir")));
        }else if(pFileinfo->iFileType == 1){
            pItem->setIcon(QIcon(QPixmap(":/icon_file")));
        }
        pItem->setText(pFileinfo->CaName);
        ui->listWidget->addItem(pItem);
    }
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
    PDU* pdu = mkPDU(m_CurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_CurPath.toStdString().c_str(),m_CurPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}

void File::on_flushFile_PB_clicked()
{
    qDebug()<<"刷新文件按键";
    flush_file();
}

void File::on_delDir_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"删除文件夹","请选择所要删除的文件夹");
        return;
    }
    QString strDelDirname = pItem->text();
    foreach(FileInfo* dir,m_fileList){
        if(dir->CaName==strDelDirname&&dir->iFileType!=0){
            QMessageBox::warning(this,"删除文件夹","文件类型错误");
            return;
        }
//        else if(dir->CaName==strDelDirname&&dir->iFileType==0){
//            int ret = QMessageBox::question(this,"删除文件夹",QString("是否确认删除文件夹 %1").arg(strDelDirname));
//            if(ret != QMessageBox::Yes){
//                return;
//            }
//            QString strPath = m_CurPath+"/"+strDelDirname;
//            PDU* pdu = mkPDU(strPath.toStdString().size()+1);
//            pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
//            memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
//            Client::getInstance().sendPDU(pdu);
//            return;
//        }
    }
    int ret = QMessageBox::question(this,"删除文件夹",QString("是否确认删除文件夹 %1").arg(strDelDirname));
    if(ret != QMessageBox::Yes){
        return;
    }
    QString strPath = m_CurPath+"/"+strDelDirname;
    PDU* pdu = mkPDU(strPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}

void File::on_delFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"删除文件","请选择所要删除的文件");
        return;
    }
    QString strDelfilename = pItem->text();
    foreach(FileInfo* dir,m_fileList){
        if(dir->CaName==strDelfilename&&dir->iFileType!=1){
            QMessageBox::warning(this,"删除文件","文件类型错误");
            return;
        }
//        else if(dir->CaName==strDelDirname&&dir->iFileType==0){
//            int ret = QMessageBox::question(this,"删除文件夹",QString("是否确认删除文件夹 %1").arg(strDelDirname));
//            if(ret != QMessageBox::Yes){
//                return;
//            }
//            QString strPath = m_CurPath+"/"+strDelDirname;
//            PDU* pdu = mkPDU(strPath.toStdString().size()+1);
//            pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
//            memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
//            Client::getInstance().sendPDU(pdu);
//            return;
//        }
    }
    int ret = QMessageBox::question(this,"删除文件",QString("是否确认删除文件 %1").arg(strDelfilename));
    if(ret != QMessageBox::Yes){
        return;
    }
    QString strPath = m_CurPath+"/"+strDelfilename;
    PDU* pdu = mkPDU(strPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_REQUEST;
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}

void File::on_rename_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"重命名文件","请选择所要重命名的文件");
        return;
    }
    QString strOldname = pItem->text();
    QString strNewname = QInputDialog::getText(this,"重命名文件","文件名：");
    if(strNewname.isEmpty() || strNewname.size()>32){
        QMessageBox::warning(this,"重命名文件夹","名称长度非法");
        return;
    }
    PDU* pdu = mkPDU(m_CurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
    memcpy(pdu->caData,strOldname.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strNewname.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_CurPath.toStdString().c_str(),m_CurPath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}
