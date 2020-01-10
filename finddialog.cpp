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
    findDefault = ui->findText->text();
    delete ui;
}

QString FindDialog::findDefault = QString();

void FindDialog::setText(ATextEdit *text) {
    txt = text;
    connect(ui->findButton, &QPushButton::clicked, this, &FindDialog::find);
    connect(ui->replaceButton, &QPushButton::clicked, this, &FindDialog::replace);
    connect(ui->replaceAllButton, &QPushButton::clicked, this, &FindDialog::replaceAll);
    if(txt->textCursor().hasSelection()) {
        ui->findText->setText(txt->textCursor().selectedText());
    } else {
        ui->findText->setText(findDefault);
    }
    ui->findText->setFocus();
    ui->findText->selectAll();
}

void FindDialog::find() {
    QTextCursor next = txt->document()->find(ui->findText->text(), txt->textCursor());
    txt->setTextCursor(next);
}

void FindDialog::replace() {
    QTextCursor next = txt->textCursor();
    if(next.hasSelection()) {
        if(next.selectedText() == ui->findText->text()) {
            next.insertText(ui->replaceText->text());
        }
    }
    find();
}

void FindDialog::replaceAll() {
    QTextCursor next = txt->textCursor();
    find();
    while(next.hasSelection()) replace();
}
