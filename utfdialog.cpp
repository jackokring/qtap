#include "utfdialog.h"
#include "ui_utfdialog.h"

UTFDialog::UTFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UTFDialog)
{
    ui->setupUi(this);
}

UTFDialog::~UTFDialog()
{
    delete ui;
}

void UTFDialog::setBytes(QByteArray ba) {
    old = ba;
}

QByteArray UTFDialog::bytes() {
    return old;
}
