#ifndef SAHREFILE_H
#define SAHREFILE_H

#include <QWidget>

namespace Ui {
class SahreFile;
}

class SahreFile : public QWidget
{
    Q_OBJECT

public:
    explicit SahreFile(QWidget *parent = nullptr);
    ~SahreFile();

private:
    Ui::SahreFile *ui;
};

#endif // SAHREFILE_H
