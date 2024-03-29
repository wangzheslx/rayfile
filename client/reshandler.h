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
    void del_dir(PDU* pdu);
    void del_file(PDU* pdu);
    void rename_file(PDU*pdu);
    void move_file(PDU*pdu);
    void uploading(PDU* pdu);
    void uploadingresp(PDU* pdu);
    void downloading(PDU* pdu);
    void downloadingresp(PDU* pdu);
    void sharefile();
    void sharefileAgree(PDU* pdu);
    void sharefileRespond(PDU* pdu);
};

#endif // RESHANDLER_H
