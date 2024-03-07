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

PDU* mkPDU(uint uiMSGLen);

#endif // PROTOCOL_H
