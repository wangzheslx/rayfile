#include "operatedb.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
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

OperateDb::~OperateDb()
{
    m_db.close();
}

OperateDb::OperateDb(QObject *parent) : QObject(parent)
{
    m_db =  QSqlDatabase::addDatabase("QMYSQL");
}
