#include "index.h"
#include "ui_index.h"

Index &Index::getinstance()
{
    static Index instance;
    return instance;
}

Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
}

Friend *Index::getFriend()
{
    return ui->friend_Page;
}

File *Index::getFile()
{
    return ui->file_Page;
}

Index::~Index()
{
    delete ui;
}

void Index::on_friend_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->friend_PB->setStyleSheet("QPushButton{border:none;background:rgb(255,255,255);padding:20px}");
    ui->file_PB->setStyleSheet("QPushButton{border:none;background:rgba(255,255,255,0);padding:20px}");
}

void Index::on_file_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->friend_PB->setStyleSheet("QPushButton{border:none;background:rgba(255,255,255,0);padding:20px}");
    ui->file_PB->setStyleSheet("QPushButton{border:none;background:rgb(255,255,255);padding:20px}");
    getFile()->flush_file();
}
