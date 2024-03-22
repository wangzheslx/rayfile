#include "sahrefile.h"
#include "ui_sahrefile.h"

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
