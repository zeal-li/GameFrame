#include "basedialog.h"
#include "ui_basedialog.h"

BaseDialog::BaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseDialog)
{
    ui->setupUi(this);
}

BaseDialog::~BaseDialog()
{
    delete ui;
}

bool BaseDialog::init()
{
    setTitle();

    return true;
}

void BaseDialog::showDialog(bool modal)
{
    this->setModal(modal);
    this->show();
    this->exec();
}

void BaseDialog::setTitle(QString title)
{
    this->setWindowTitle(title);
}
