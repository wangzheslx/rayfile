#include "Client.h"
#include "index.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client::getInstance().show();
    return a.exec();
}
