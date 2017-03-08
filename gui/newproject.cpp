#include "newproject.h"
#include "ui_newproject.h"

newproject::newproject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newproject)
{
    ui->setupUi(this);
}

newproject::~newproject()
{
    delete ui;
}
