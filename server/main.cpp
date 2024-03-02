#include "server.h"
#include "operatedb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperateDb::getinstance().sql_connect();
    Server::getinstance();
    return a.exec();
}
