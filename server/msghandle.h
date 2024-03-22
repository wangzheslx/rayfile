#ifndef MSGHANDLE_H
#define MSGHANDLE_H

#include "protocol.h"

#include <QFile>
#include <QString>



class MsgHandle
{
public:
    MsgHandle();
    PDU* Regist(PDU* pdu);
    PDU* Login(PDU* pdu,QString & strusername);
    PDU* FindUser(PDU* pdu);
    PDU* OnlineUser();
    PDU* AddFriend(PDU*pdu);
    PDU* AddFriendagree (PDU*pdu);
    PDU* GetFriend(PDU*pdu);
    PDU* DelFriend(PDU* pdu);
    void ResendChat(PDU* pdu);
    PDU* MkDir(PDU* pdu);
    PDU* flush_file(PDU* pdu);
    PDU* del_dir(PDU* pdu);
    PDU* del_file(PDU* pdu);
    PDU* rename_file(PDU* pdu);
    PDU* move_file(PDU* pdu);
    PDU* upload_file(PDU* pdu);
    PDU* uploading_file(PDU* pdu);

    bool m_uploading;
    QFile m_uploadFILE;
    qint64 m_uploadtotal;
    qint64 m_uploadedbyte;




};

#endif // MSGHANDLE_H
