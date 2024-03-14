#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    static Server& getinstance();
    void loadConfig();
    QString getrootpath();
    ~Server();

private:
    Ui::Server *ui;
    QString m_strIP;
    quint16  m_usPort;
    QString m_strRootPath;
    Server(QWidget *parent = nullptr);
    Server(const Server&instance) = delete;
    Server& operator=(const Server&) = delete;
};
#endif // SERVER_H
