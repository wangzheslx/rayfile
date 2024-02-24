#include "Client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client::getInstance().show();
    return a.exec();
}
