#include "sahrefile.h"
#include "ui_sahrefile.h"
#include "index.h"
SahreFile::SahreFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SahreFile)
{
    ui->setupUi(this);
}

SahreFile::~SahreFile()
{
    delete ui;
}

void SahreFile::updateFriend_LW()
{
    ui->listWidget->clear();
    QListWidget* friend_LW = Index::getinstance().getFriend()->get_friend_LW();
    for(int i = 0;i<friend_LW->count();i++){
        QListWidgetItem* friendItem = friend_LW->item(i);
        QListWidgetItem* newItem = new QListWidgetItem(*friendItem);
        ui->listWidget->addItem(newItem);
    }
}

void SahreFile::on_selectAll_clicked()
{
    for (int i =0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(true);
    }
}

void SahreFile::on_unselect_clicked()
{
    for (int i =0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(false);
    }
}

void SahreFile::on_confirm_clicked()
{

}
