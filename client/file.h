#ifndef FILE_H
#define FILE_H

#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();

private slots:
    void on_mkDir_PB_clicked();

private:
    Ui::File *ui;
    QString m_CurPath;
    QString m_RootPath;
};

#endif // FILE_H
