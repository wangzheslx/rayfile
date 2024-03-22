#ifndef SAHREFILE_H
#define SAHREFILE_H

#include <QWidget>
#include<QListWidget>
namespace Ui {
class SahreFile;
}

class SahreFile : public QWidget
{
    Q_OBJECT

public:
    explicit SahreFile(QWidget *parent = nullptr);
    ~SahreFile();

    void updateFriend_LW();

private slots:
    void on_selectAll_clicked();

    void on_unselect_clicked();

    void on_confirm_clicked();

private:
    Ui::SahreFile *ui;
};

#endif // SAHREFILE_H
