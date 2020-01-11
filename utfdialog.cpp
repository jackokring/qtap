#include "utfdialog.h"
#include "ui_utfdialog.h"
#include "adialog.h"
#include "mainwindow.h"

UTFDialog::UTFDialog(QWidget *parent) :
    ADialog(parent),
    ui(new Ui::UTFDialog) {
    ui->setupUi(this);
}

UTFDialog::~UTFDialog() {
    delete ui;
}

void UTFDialog::setBytes(QByteArray ba) {
    old = ba;
}

QByteArray UTFDialog::bytes() {
    return old;
}

void UTFDialog::setModified() {
    ((MainWindow *)parentWidget())->setModified();
}
