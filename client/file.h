#ifndef FILE_H
#define FILE_H

#include "protocol.h"

#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    void flush_file();
    void updateFileList(QList<FileInfo *> pFileList);
    ~File();

private slots:
    void on_mkDir_PB_clicked();

    void on_flushFile_PB_clicked();

    void on_delDir_PB_clicked();

    void on_delFile_PB_clicked();

    void on_rename_PB_clicked();

private:
    Ui::File *ui;
    QList<FileInfo*> m_fileList;
    QString m_CurPath;
    QString m_RootPath;
};

#endif // FILE_H
