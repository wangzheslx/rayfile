#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef unsigned int uint;
//登录状态状态机
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    //注册
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPEND,
    //登录
    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPEND,
    //查找用户
    ENUM_MSG_TYPE_FINDUSER_REQUEST,
    ENUM_MSG_TYPE_FINDUSER_RESPEND,
    //显示在线用户
    ENUM_MSG_TYPE_ONLINEUSER_REQUEST,
    ENUM_MSG_TYPE_ONLINEUSER_RESPEND,
    //添加在线用户好友
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESPEND,
    //添加回应同意好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND,
    //添加回应同意好友请求
    ENUM_MSG_TYPE_GET_FRIEND_REQUEST,
    ENUM_MSG_TYPE_GET_FRIEND_RESPEND,
    //添加删除好友请求
    ENUM_MSG_TYPE_DEL_FRIEND_REQUEST,
    ENUM_MSG_TYPE_DEL_FRIEND_RESPEND,
    //聊天
    ENUM_MSG_TYPE_CHAT_REQUEST,
    ENUM_MSG_TYPE_CHAT_RESPEND,
    //聊天
    ENUM_MSG_TYPE_MKDIR_REQUEST,
    ENUM_MSG_TYPE_MKDIR_RESPEND,
    //刷新文件
    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,
    ENUM_MSG_TYPE_FLUSH_FILE_RESPEND,
    //删除文件夹
    ENUM_MSG_TYPE_DEL_DIR_REQUEST,
    ENUM_MSG_TYPE_DEL_DIR_RESPEND,
    //删除文件
    ENUM_MSG_TYPE_DEL_FILE_REQUEST,
    ENUM_MSG_TYPE_DEL_FILE_RESPEND,
    //重命名文件
    ENUM_MSG_TYPE_RENAME_FILE_REQUEST,
    ENUM_MSG_TYPE_RENAME_FILE_RESPEND,
    //移动文件
    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,
    ENUM_MSG_TYPE_MOVE_FILE_RESPEND,
    //上传文件
    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND,
    //正在上传文件
    ENUM_MSG_TYPE_UPLOADING_FILE_REQUEST,
    ENUM_MSG_TYPE_UPLOADING_FILE_RESPEND,
    //下载文件
    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPEND,
    //正在下载文件
    ENUM_MSG_TYPE_DOWNLOADING_FILE_REQUEST,
    ENUM_MSG_TYPE_DOWNLOADING_FILE_RESPEND,
    //分享文件
    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,
    ENUM_MSG_TYPE_SHARE_FILE_RESPEND,
    //同意分享文件
    ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST,
    ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPEND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff,
};
//协议数据结构体
struct PDU{
    uint uiPDUlen;//总长度
    uint uiMsgLen;//数据长度
    uint uiMsgType;//协议消息类型
    char caData[64];//参数
    char caMsg[];//数据
};

struct FileInfo{
    char CaName[32];
    int iFileType;
};

PDU* mkPDU(uint uiMSGLen);

#endif // PROTOCOL_H
