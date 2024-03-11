#include "operatedb.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
OperateDb &OperateDb::getinstance()
{
    static OperateDb instance;
    return instance;
}

void OperateDb::sql_connect()
{
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setPort(3306);
    m_db.setDatabaseName("pan_data");
    if(m_db.open()){
        qDebug()<<"数据库连接成功";
    }else{
        QMessageBox::critical(0,"连接数据库",m_db.lastError().text());
    }
}

bool OperateDb::handleRegist(char *name, char *pwd)
{
    qDebug()<< "用户注册数据库操作";
    if(name == NULL||pwd == NULL){
        qDebug()<<"handleRegist 返回值 name == NULL||pwd == NULL";
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    QSqlQuery q;
    //exec()语句是否运行 next()是否有值
    if(!q.exec(sql)||q.next()){
         qDebug()<<"handleRegist QSqlQuery 返回值 !q.exec(sql)||q.next()";
        return false;
    }
    sql = QString("insert into user_info(name ,pwd) values('%1','%2')").arg(name).arg(pwd);
    return q.exec(sql);//返回运行结果
}

bool OperateDb::handleLogin(char *name, char *pwd)
{
    qDebug()<<"用户登录数据库操作";
    if(name ==NULL||pwd == NULL){
        qDebug()<<"handleLogin 返回值 name == NULL||pwd == NULL";
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    QSqlQuery q;
    if(!q.exec(sql) || !q.next()){
         qDebug()<<"handleRegist QSqlQuery 返回值 !q.exec(sql) || !q.next()";
         return false;
    }
    sql = QString("update user_info set online = 1 where name = '%1'").arg(name);
    return q.exec(sql);
}

void OperateDb::handleoffline(const char *name)
{
    qDebug()<<"用户登出数据库操作";
    if(name == NULL){
        qDebug()<<"handleoffline 返回值 name == NULL";
        return;
    }
    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(name);
    QSqlQuery q;
    qDebug()<<"将用户"<<name<<"online置位0";
    q.exec(sql);
}

int OperateDb::handleFindUser(const char *name)
{
    qDebug()<<"用户好友界面查找数据库操作进行";
    if(name ==NULL){
        qDebug()<<"handleFindUser name ==NULL";
        return -1;
    }
    QString sql = QString("select online from user_info where name= '%1'").arg(name);
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();
    }
    return -1;
}

QStringList OperateDb::handleOnlineUser()
{
    qDebug()<<"用户好友界面数据库查找在线用户进行";
    QString sql = QString("select name from user_info where online = 1");
    QSqlQuery q;
    q.exec(sql);
    QStringList result;
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}

int OperateDb::handleAddFriend(const char *curName, const char *tarName)
{
    if(curName==NULL||tarName==NULL){
        qDebug()<<"添加好友为空";
        return -1;
    }
    qDebug()<<"用户好友界面添加好友进行";
    QString sql = QString(R"(
                          select * from friend where
                          (
                            user_id = (select id from user_info where name = '%1')
                            and
                            friend_id = (select id from user_info where name = '%2')
                           )
                          or
                          (
                            user_id = (select id from user_info where name = '%3')
                            and
                            friend_id = (select id from user_info where name = '%4')
                          );
                          )").arg(curName).arg(tarName).arg(tarName).arg(curName);
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -2;
    }
    sql = QString("select online from user_info where name= '%1'").arg(tarName);
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();
    }
    return -1;
}

void OperateDb::handleAgreefriend(const char *curName, const char *tarName)
{
    qDebug()<<"处理同意用户好友添加好友";
    if(curName==NULL||tarName==NULL){
        qDebug()<<"添加好友为空";
        return ;
    }
    QString sql = QString(R"(
                          insert into friend(user_id,friend_id)
                          select u1.id,u2.id
                          from user_info u1,user_info u2
                          where u1.name = '%1' and u2.name = '%2';
                          )").arg(curName).arg(tarName);
    QSqlQuery q;
    q.exec(sql);
}

QStringList OperateDb::handleGetfriend(const char *name)
{
    qDebug()<<"数据库查找好友用户";

    QSqlQuery q ;
    QString sql = QString(R"(select name from user_info where online = 1 and id in
                          (
                          select friend_id from friend where user_id  = (select id from user_info where name = '%1')
                          union
                          select user_id from friend where friend_id  = (select id from user_info where name = '%2')
                          );)").arg(name).arg(name);
    q.exec(sql);
    QStringList result;
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;

}

OperateDb::~OperateDb()
{
    m_db.close();
}

OperateDb::OperateDb(QObject *parent) : QObject(parent)
{
    m_db =  QSqlDatabase::addDatabase("QMYSQL");
}
