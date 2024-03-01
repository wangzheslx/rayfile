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

OperateDb::~OperateDb()
{
    m_db.close();
}

OperateDb::OperateDb(QObject *parent) : QObject(parent)
{
    m_db =  QSqlDatabase::addDatabase("QMYSQL");
}
