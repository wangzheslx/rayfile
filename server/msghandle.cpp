#include "msghandle.h"
#include <server.h>
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "operatedb.h"
#include<QDebug>
#include <QDir>
MsgHandle::MsgHandle()
{
    m_uploading = false;
    m_downloading = false;
}

PDU* MsgHandle::Regist(PDU *pdu)
{
    qDebug()<<"服务器用户注册请求实现";
    char strname[32] = {'\0'};
    char strpwd[32] = {'\0'};
    memcpy(strname,pdu->caData,32);
    memcpy(strpwd,pdu->caData+32,32);
    bool ret =  OperateDb::getinstance().handleRegist(strname,strpwd);
    if(ret){
        qDebug()<<"注册成功";
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getinstance().getrootpath()).arg(strname));
    }else{
        qDebug()<<"注册失败";
    }
    //构建返回的pdu
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::Login(PDU *pdu, QString &strusername)
{
    qDebug()<<"服务器用户登录请求实现";
    char strname[32] = {'\0'};
    char strpwd[32] = {'\0'};
    memcpy(strname,pdu->caData,32);
    memcpy(strpwd,pdu->caData+32,32);
    bool ret =  OperateDb::getinstance().handleLogin(strname,strpwd);
    if(ret){
        qDebug()<<"登录成功";
    }else{
        qDebug()<<"登录失败";
    }
    //构建返回的pdu
    PDU * respdu = mkPDU(0);
    strusername = strname;
    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::FindUser(PDU *pdu)
{
    qDebug()<<"服务器用户查找请求实现";
    char strname[32] = {'\0'};
    memcpy(strname,pdu->caData,32);
    int ret =  OperateDb::getinstance().handleFindUser(strname);

    //构建返回的pdu
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_FINDUSER_RESPEND;
    memcpy(respdu->caData,strname,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandle::OnlineUser()
{
    qDebug()<<"服务器用户在线显示请求实现";
    QStringList result = OperateDb::getinstance().handleOnlineUser();
    PDU * respdu = mkPDU(result.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_ONLINEUSER_RESPEND;
    for(int i = 0;i<result.size();i++){
        memcpy(respdu->caData+i*32,result.at(i).toStdString().c_str(),32);
        qDebug()<<"在线用户 "<<i+1<< ":" << result.at(i);
    }
    return respdu;
}

PDU *MsgHandle::AddFriend(PDU* pdu)
{
    qDebug()<<"服务器用户添加好友请求";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};


    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);

    int ret = OperateDb::getinstance().handleAddFriend(caCurName,caTarName);
    if(ret == 1){
        MyTcpServer::getInstance().resend(caTarName,pdu);
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;

}

PDU *MsgHandle::AddFriendagree(PDU *pdu)
{
    qDebug()<<"有好友同意请求消息";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    OperateDb::getinstance().handleAgreefriend(caCurName,caTarName);
    PDU*respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND;
    MyTcpServer::getInstance().resend(caCurName,respdu);
    return respdu;
}

PDU *MsgHandle::GetFriend(PDU *pdu)
{
    qDebug()<<"查找好友用户";
    char Curname[32] = {'\0'};
    memcpy(Curname,pdu->caData,32);
    QStringList result = OperateDb::getinstance().handleGetfriend(Curname);
    PDU * respdu = mkPDU(result.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_GET_FRIEND_RESPEND;
    for(int i = 0;i<result.size();i++){
        memcpy(respdu->caData+i*32,result.at(i).toStdString().c_str(),32);
        qDebug()<<"好友 "<<i+1<< ":" << result.at(i);
    }
    return respdu;

}

PDU *MsgHandle::DelFriend(PDU *pdu)
{
    qDebug()<<"删除好友";
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    bool flag = OperateDb::getinstance().handleDelFriend(caCurName,caTarName);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_RESPEND;
    memcpy(respdu->caData,&flag,sizeof(bool));
    return respdu;
}

void MsgHandle::ResendChat(PDU *pdu)
{
    MyTcpServer::getInstance().resend(pdu->caData+32,pdu);
}

PDU *MsgHandle::MkDir(PDU *pdu)
{
    qDebug()<<"新建文件夹";
    //pdu->caMsg[pdu->uiMsgLen] = '\0';
    QString strcurpath = pdu->caMsg;
    QDir dir;
    bool res = false;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_RESPEND;
    if(!dir.exists(strcurpath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        qDebug()<<"路径不存在";
        return respdu;
    }

    QString strNewPath = QString("%1/%2").arg(strcurpath).arg(pdu->caData);
    if(dir.exists(strNewPath) || !dir.mkdir(strNewPath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        qDebug()<<"已经存在或则";
        return respdu;
    }
    res = true;
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::flush_file(PDU *pdu)
{
    //pdu->caMsg[pdu->uiMsgLen] = '\0';
    QString strpath = pdu->caMsg;
    QDir dir(strpath);
    QFileInfoList fileInforList =  dir.entryInfoList();
    int iFileCount = fileInforList.size();

    PDU* respdu = mkPDU((iFileCount-2)*sizeof(FileInfo));
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPEND;
    QString strFileName;
    FileInfo* pFileInfo = NULL;
    //int index= 0;
    for(int i = 0,j = 0;i<iFileCount;i++){
        if(fileInforList[i].fileName() == "."||fileInforList[i].fileName() == "..")continue;
        strFileName = fileInforList[i].fileName();
        pFileInfo = (FileInfo*)(respdu->caMsg)+j++;
        //index++;
        memcpy(pFileInfo,strFileName.toStdString().c_str(),32);
        if(fileInforList[i].isDir()){
            pFileInfo->iFileType = 0;
        }else if(fileInforList[i].isFile()){
            pFileInfo->iFileType = 1;
        }
        qDebug()<<"filename :"<<strFileName<<"iFileType:"<<pFileInfo->iFileType;
    }
    return respdu;
}

PDU *MsgHandle::del_dir(PDU *pdu)
{
    QFileInfo fileinfo(pdu->caMsg);
    bool ret = false;
    if(fileinfo.isDir()){
       QDir dir(pdu->caMsg);
       ret = dir.removeRecursively();
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::del_file(PDU *pdu)
{
    QFileInfo fileinfo(pdu->caMsg);
    bool ret = false;
    if(fileinfo.isFile()){
       QFile file(pdu->caMsg);
       ret = file.remove();
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::rename_file(PDU *pdu)
{
    char oldname[32] ={'\0'};
    char newname[32] ={'\0'};
    QString strcurpath = pdu->caMsg;
    memcpy(oldname,pdu->caData,32);
    memcpy(newname,pdu->caData+32,32);
    QDir dir;
    bool res = false;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_RESPEND;
    if(!dir.exists(strcurpath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        qDebug()<<"路径不存在";
        return respdu;
    }
    QString strOldPath = QString("%1/%2").arg(strcurpath).arg(oldname);
    QString strNewPath = QString("%1/%2").arg(strcurpath).arg(newname);
    if(dir.exists(strNewPath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        qDebug()<<"已经存在或则创建失败";
        return respdu;
    }
    res = dir.rename(strOldPath,strNewPath);
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::move_file(PDU *pdu)
{
    int srclen = 0;
    int tarlen = 0;
    memcpy(&srclen,pdu->caData,sizeof(int));
    memcpy(&tarlen,pdu->caData+32,sizeof(int));

    char *pScrPath = new char[srclen+1];
    char* pTarPath = new char[tarlen+1];
    memset(pScrPath,'\0',srclen+1);
    memset(pTarPath,'\0',tarlen+1);


    memcpy(pScrPath,pdu->caMsg,srclen);
    memcpy(pTarPath,pdu->caMsg+srclen,tarlen);
    qDebug()<<"pScrPath: "<<pScrPath<<"pTarPath: "<<pTarPath;
    bool ret = QFile::rename(pScrPath,pTarPath);
    qDebug()<<"rename"<<ret;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));

    delete [] pScrPath;
    delete [] pTarPath;
    pScrPath = NULL;
    pTarPath = NULL;
    return respdu;

}

PDU *MsgHandle::upload_file(PDU *pdu)
{
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND;
    int ret = 0;
    if(m_uploading){
        qDebug()<<"m_uploading is true";
        ret = 1;
        memcpy(respdu->caData,&ret,sizeof(bool));
        return respdu;
    }
    char caFileName[32] = {'\0'};
    qint64 filesize = 0;
    memcpy(caFileName,pdu->caData,32);
    memcpy(&filesize,pdu->caData+32,sizeof(qint64));
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
    m_uploadFILE.setFileName(strPath);
    if(m_uploadFILE.open(QIODevice::WriteOnly)){
        m_uploading = true;
        m_uploadtotal = filesize;
        m_uploadedbyte = 0;
    }else{
        qDebug()<<"file open failed";
        ret = -1;
    }
    qDebug()<<"upload start success";
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::uploading_file(PDU *pdu)
{
    m_uploadFILE.write(pdu->caMsg,pdu->uiMsgLen);
    m_uploadedbyte += pdu->uiMsgLen;
    if(m_uploadedbyte<m_uploadtotal){
        return NULL;
    }
    m_uploadFILE.close();
    m_uploading = false;
    PDU* respdu = mkPDU(0);
    bool ret = m_uploadtotal==m_uploadedbyte;
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOADING_FILE_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::download_file(PDU *pdu)
{
    PDU* respdu =  mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPEND;
    int ret =1;
    if(m_downloading){
        qDebug()<<"m_downloading is true";
        ret = 0;
        memcpy(respdu->caData,&ret,sizeof(int));
        return respdu;
    }
    QString filepath = pdu->caMsg;
    m_downloadFILE.setFileName(filepath);
    //这种不对 QFile的文件获取size需要打开文件之后才能获取
    //qint64 filesize = m_downloadFILE.size();
    //不打开文件获取应该这样
    QFileInfo fileInfo(pdu->caMsg);
    qint64 filesize = fileInfo.size();
    if(!m_downloadFILE.open(QIODevice::ReadOnly)){
        ret = 0;
        memcpy(respdu->caData,&ret,sizeof(int));
        return respdu;
    }
    m_downloading = true;
    memcpy(respdu->caData,&ret,sizeof(int));
    memcpy(respdu->caData+32,&filesize,sizeof(qint64));
    return respdu;

}

PDU *MsgHandle::downloading_file(MyTcpSocket *m_tcpsocket)
{
    qDebug()<<"真开始传输文件";
    PDU* respdu = mkPDU(4096);
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOADING_FILE_RESPEND;
    qint64 ret = 0;
    while(true){
        ret = m_downloadFILE.read(respdu->caMsg,4096);
        if(ret <=0){
            break;
        }
        respdu->uiMsgLen = ret;
        respdu->uiPDUlen = ret+sizeof(PDU);
        m_tcpsocket->write((char*)respdu,respdu->uiPDUlen);
    }
    m_downloading  = false;
    m_downloadFILE.close();

    free(respdu);
    respdu = NULL;
    return NULL;
}

PDU *MsgHandle::shareFile(PDU *pdu)
{
    qDebug()<<"shareFile";
    char caCurName[32] = {'\0'};
    int friendNum = 0;

    memcpy(caCurName,pdu->caData,32);
    memcpy(&friendNum,pdu->caData+32,sizeof(int));
    qDebug()<<"caCurName"<<caCurName<<"friendNum"<<friendNum;
    int friendsize = 32*friendNum;
    PDU* resendpdu = mkPDU(pdu->uiMsgLen-friendsize);
    resendpdu->uiMsgType = pdu->uiMsgType;

    memcpy(resendpdu->caData,caCurName,32);
    memcpy(resendpdu->caMsg,pdu->caMsg+friendsize,pdu->uiMsgLen-friendsize);
    qDebug()<<"resendpdu->caData"<<resendpdu->caData<<"resendpdu->caMsg"<<resendpdu->caMsg;
    char caRecvName[32] = {'\0'};
    for(int i = 0;i<friendNum;i++){
        memcpy(caRecvName,pdu->caMsg+32*i,32);
        MyTcpServer::getInstance().resend(caRecvName,resendpdu);
    }
    free(resendpdu);
    resendpdu = NULL;

    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPEND;
    return respdu;
}

PDU *MsgHandle::shareFileAgree(PDU *pdu)
{
    QString strSharePath = pdu->caMsg;
    QString strRecvPath = QString("%1/%2").arg(Server::getinstance().getrootpath()).arg(pdu->caData);

    int index = strSharePath.lastIndexOf('/');
    QString strFileName = strSharePath.right(strSharePath.size()-index-1);
    strRecvPath = strRecvPath+'/'+strFileName;
    qDebug()<<"strSharePath"<<strSharePath<<"strRecvPath"<<strRecvPath;
    bool ret = QFile::copy(strSharePath,strRecvPath);
    qDebug()<<"ret:"<<ret;
    PDU* respdu = mkPDU(0);
    memcpy(respdu->caData,&ret,sizeof(bool));
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPEND;
    return respdu;


}

