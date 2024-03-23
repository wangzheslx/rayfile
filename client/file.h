#ifndef FILE_H
#define FILE_H

#include "protocol.h"
#include "sahrefile.h"
#include <QFile>
#include <QListWidgetItem>
#include <QUrl>
#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    QString m_CurPath;
    QString m_RootPath;
    QString m_strsharefile;
    explicit File(QWidget *parent = nullptr);
    void flush_file();
    void updateFileList(QList<FileInfo *> pFileList);
    void uploadFile();
    void dowmloadFile(qint64 fsize);
    void downloadingFile(char* buffer,qint64 fsize);
    SahreFile* getshare();
    ~File();

private slots:
    void on_mkDir_PB_clicked();

    void on_flushFile_PB_clicked();

    void on_delDir_PB_clicked();

    void on_delFile_PB_clicked();

    void on_rename_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_moveFile_PB_clicked();

    void on_uploadFile_PB_clicked();

    void on_shareFile_PB_clicked();

    void on_downloadFile_PB_clicked();

private:
    Ui::File *ui;
    QList<FileInfo*> m_fileList;

    QString m_mvPath;
    QString m_mvFile;
    QString m_uploadfilePath;

    SahreFile * m_sharefile;
    bool m_uploding;

    bool m_downloading;
    QString m_savePath;
    QFile m_saveFILE;
    qint64 m_downsize;
    qint64 m_downedsize;
};

#endif // FILE_H
