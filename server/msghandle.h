#ifndef MSGHANDLE_H
#define MSGHANDLE_H

#include "protocol.h"
#include <QFile>
#include <QString>


class MyTcpSocket;

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
    PDU* download_file(PDU* pdu);
    PDU* downloading_file(MyTcpSocket* m_tcpsocket);

    bool m_uploading;
    QFile m_uploadFILE;
    qint64 m_uploadtotal;
    qint64 m_uploadedbyte;

    bool m_downloading;
    QFile m_downloadFILE;




};

#endif // MSGHANDLE_H
