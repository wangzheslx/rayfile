#ifndef RESHANDLER_H
#define RESHANDLER_H


#include "protocol.h"



class ResHandler
{
public:
    ResHandler();

    void regist(PDU* pdu);
    void login(PDU*pdu);
    void finduser(PDU*pdu);
    void onlineuser(PDU* pdu);
    void addfriendreturn(PDU*pdu);
    void requestaddfriend(PDU* pdu);
    void respondaddfriend();
    void getfriend(PDU*pdu);
    void delfriend(PDU* pdu);
    void updatechat(PDU* pdu,bool self = 0);
    void mkdir(PDU*pdu);
    void flush_file(PDU*pdu);
};

#endif // RESHANDLER_H
