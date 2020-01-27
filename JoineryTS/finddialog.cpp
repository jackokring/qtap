#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainwindow.h"

FindDialog::FindDialog(QWidget *parent) :
    ADialog(parent),
    ui(new Ui::FindDialog) {
    ui->setupUi(this);
}

FindDialog::~FindDialog() {
    findDefault = ui->findText->text();
    replaceDefault = ui->replaceText->text();
    delete ui;
}

QString FindDialog::findDefault = QString();
QString FindDialog::replaceDefault = QString();

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
    ui->replaceText->setText(replaceDefault);
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
            find();
        } else {
            find();
            replace();//no guard needed
        }
    } else {
        find();//avoid nesting replace here, just try a find
        next = txt->textCursor();
        if(next.hasSelection()) {
            replace();//guard against not found
        }
    }
}

void FindDialog::replaceAll() {
    QTextCursor next = txt->textCursor();
    if(!next.hasSelection()) find();//initialize
    while(next.hasSelection()) {
        replace();
        next = txt->textCursor();
    }
}
