#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::setText(ATextEdit *text) {
    txt = text;
    connect(ui->findButton, &QPushButton::clicked, this, &FindDialog::find);
    connect(ui->replaceButton, &QPushButton::clicked, this, &FindDialog::replace);
    connect(ui->replaceAllButton, &QPushButton::clicked, this, &FindDialog::replaceAll);
}

void FindDialog::find() {

}

void FindDialog::replace() {

}

void FindDialog::replaceAll() {

}
