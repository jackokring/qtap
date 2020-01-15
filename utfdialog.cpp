#include "utfdialog.h"
#include "ui_utfdialog.h"
#include "adialog.h"
#include "mainwindow.h"

UTFDialog::UTFDialog(QWidget *parent) :
    ADialog(parent),
    ui(new Ui::UTFDialog) {
    ui->setupUi(this);
    connect(ui->latinButton, &QPushButton::clicked, this, &UTFDialog::latinInput);
    connect(ui->visibleCtlButton, &QPushButton::clicked, this, &UTFDialog::visibleCtl);
    connect(ui->escAnsiButton, &QPushButton::clicked, this, &UTFDialog::markAnsi);
    connect(ui->markWarnButton, &QPushButton::clicked, this, &UTFDialog::markWarn);
    connect(ui->markErrorsButton, &QPushButton::clicked, this, &UTFDialog::markError);
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

void UTFDialog::latinInput() {
    QByteArray input = old;
    QString output = QString();
    QByteArray::iterator i;
    for(i = input.begin(); i != input.end(); ++i) {
        output += (*i);
    }
    old = QByteArray(output.toUtf8());
}

void UTFDialog::visibleCtl() {

}

void UTFDialog::markAnsi() {

}

void UTFDialog::markWarn() {

}

void UTFDialog::markError() {

}
