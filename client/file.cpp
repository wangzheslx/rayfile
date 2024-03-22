#include "Client.h"
#include "file.h"
#include "ui_file.h"

#include <QDialog>
#include <QFileDialog>
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
    m_uploding = false;
    m_downloading = false;
    flush_file();
    m_sharefile = new SahreFile;
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

void File::uploadFile()
{
    QFile file(m_uploadfilePath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"上传文件","打开文件失败");
        return;
    }
    PDU* datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_UPLOADING_FILE_REQUEST;
    qint64 ret = 0;
    while(true){
       ret = file.read(datapdu->caMsg,4096);
       if(ret == 0){
           break;
       }
       if(ret<0){
           QMessageBox::warning(this,"上传文件","读取错误");
           break;
       }
       datapdu->uiMsgLen = ret;
       datapdu->uiPDUlen = ret+sizeof(PDU);
       Client::getInstance().getTcpsocket().write((char*)datapdu,datapdu->uiPDUlen);
    }
    m_uploding = false;
    file.close();
    free(datapdu);
    datapdu = NULL;
}

void File::dowmloadFile(qint64 fsize)
{

    m_saveFILE.setFileName(m_savePath);
    if(!m_saveFILE.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"下载文件","客户端打开文件失败");
        return;
        //ret = 0;

        //memcpy(pdu->caData,&ret,sizeof(bool));
        //Client
    }
    m_downsize = fsize;
    m_downedsize = 0;
    m_downloading = true;
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOADING_FILE_REQUEST;
    Client::getInstance().sendPDU(pdu);

}

void File::downloadingFile(char *buffer, qint64 fsize)
{
    m_saveFILE.write(buffer,fsize);
    m_downedsize+=fsize;
    if(m_downedsize<m_downsize){
        return;
    }
    m_saveFILE.close();
    m_downloading = false;
    bool ret = m_downsize==m_downedsize;
    if(ret){
        QMessageBox::information(this,"下载文件","下载文件成功");
    }else{
        QMessageBox::information(this,"下载文件","下载文件出错");
    }
}

File::~File()
{
    delete ui;
    delete m_sharefile;
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

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strDirName = item->text();
    foreach(FileInfo* pFileInfo,m_fileList){
        if(pFileInfo->CaName == strDirName && pFileInfo->iFileType!=0){
            QMessageBox::warning(this,"提示","选择的不是一个文件夹");
            return;
        }
    }
    m_CurPath = m_CurPath+"/" +strDirName;
    flush_file();
}

void File::on_return_PB_clicked()
{
    if(m_CurPath == m_RootPath){
        QMessageBox::warning(this,"返回","已在根目录，不可返回");
        return;
    }
    int idx = m_CurPath.lastIndexOf('/');
    m_CurPath.remove(idx,m_CurPath.size()-idx);
    flush_file();
}

void File::on_moveFile_PB_clicked()
{
    if(ui->moveFile_PB->text()=="移动文件"){
        QListWidgetItem* item = ui->listWidget->currentItem();
        if(item == NULL){
            QMessageBox::warning(this,"移动文件","请选择所要移动的文件");
            return;
        }
        QMessageBox::information(this,"移动文件","请选择索要移动的目录");
        ui->moveFile_PB->setText("确认/取消");
        m_mvFile = item->text();
        m_mvPath = m_CurPath+"/"+m_mvFile;
        return;
    }
    QListWidgetItem* item = ui->listWidget->currentItem();
    QString strTarPath;
    QString boxMsg;
    if(item == NULL){
        strTarPath = m_CurPath+"/"+m_mvFile;
        boxMsg = QString("是否移动到当前目录");
    }else{
        foreach(FileInfo* pfileinfo,m_fileList){
            if(pfileinfo->CaName == item->text() && pfileinfo->iFileType!=0){
                QMessageBox::warning(this,"提示","当前选择的不是文件夹");
                return;
            }
        }
        strTarPath = m_CurPath+"/"+item->text()+"/"+m_mvFile;
        boxMsg = QString("是否移动到已选择的目录下");
    }
    int ret = QMessageBox::information(this,"移动文件",boxMsg,"确认","取消");
    ui->moveFile_PB->setText("移动文件");
    if(ret !=0){
        return;
    }
    int srclen = m_mvPath.toStdString().size();
    int tarlen = strTarPath.toStdString().size();
    PDU* pdu = mkPDU(srclen+tarlen+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    memcpy(pdu->caData,&srclen,sizeof(int));
    memcpy(pdu->caData+32,&tarlen,sizeof(int));

    memcpy(pdu->caMsg,m_mvPath.toStdString().c_str(),srclen);
    memcpy(pdu->caMsg+srclen,strTarPath.toStdString().c_str(),tarlen);
    Client::getInstance().sendPDU(pdu);
}

void File::on_uploadFile_PB_clicked()
{
    if(m_uploding){
        QMessageBox::warning(this,"上传文件","有文件正在上传请稍后");
        return;
    }
    m_uploadfilePath =  QFileDialog::getOpenFileName();
    qDebug()<<"m_uploadfile"<<m_uploadfilePath;
    int index = m_uploadfilePath.lastIndexOf('/');
    QString strFilename = m_uploadfilePath.right(m_uploadfilePath.size()-index-1);//这里不能转stdstring,与上面的字符数不匹配了
    if(strFilename.toStdString().size()>32){
        QMessageBox::warning(this,"文件上传","文件名称超限");
        return;
    }
    QFile file(m_uploadfilePath);
    qint64 fsize= file.size();
    PDU* pdu = mkPDU(m_CurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memcpy(pdu->caMsg,m_CurPath.toStdString().c_str(),m_CurPath.toStdString().size());
    memcpy(pdu->caData,strFilename.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&fsize,sizeof(qint64));
    Client::getInstance().sendPDU(pdu);


}

void File::on_shareFile_PB_clicked()
{
    QListWidgetItem *pitem = ui->listWidget->currentItem();
    if(pitem==NULL){
        QMessageBox::warning(this,"分享文件","请选择所要分享的文件");
        return;
    }
    m_strsharefile = pitem->text();
    m_sharefile->updateFriend_LW();
    if(m_sharefile->isHidden()){
        m_sharefile->show();
    }
}

void File::on_downloadFile_PB_clicked()
{
    if(m_downloading){
        QMessageBox::warning(this,"下载文件","有文件正在下载请稍后");
        return;
    }
    QListWidgetItem* pitem = ui->listWidget->currentItem();
    if(pitem == NULL){
        QMessageBox::warning(this,"下载文件","请选择所要下载的文件");
        return ;
    }
    QString strfilename = pitem->text();
    m_savePath.clear();
    m_savePath =  QFileDialog::getSaveFileName();
    if(m_savePath.isEmpty()){
        QMessageBox::warning(this,"下载文件","请选择所要保存的位置");
        return ;
    }
    qDebug()<<"m_savePath"<<m_savePath;
    QString strpath = QString("%1/%2").arg(m_CurPath).arg(strfilename);
    PDU* pdu = mkPDU(strpath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->caMsg,strpath.toStdString().c_str(),strpath.toStdString().size());
    Client::getInstance().sendPDU(pdu);
}
