#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>

class OperateDb : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase m_db;//数据库对象
    static OperateDb& getinstance();
    void sql_connect();
    bool handleRegist(char* name,char* pwd);
    bool handleLogin(char* name,char* pwd);
    void handleoffline(const char *name);
    int handleFindUser(const char *name);
    QStringList handleOnlineUser();
    int handleAddFriend(const char* curName,const char* tarName);
    void handleAgreefriend(const char* curName,const char* tarName);
    ~OperateDb();

private:
    explicit OperateDb(QObject *parent = nullptr);
    OperateDb(const OperateDb&) = delete;
    OperateDb& operator=(const OperateDb&) = delete;

signals:

};

#endif // OPERATEDB_H
