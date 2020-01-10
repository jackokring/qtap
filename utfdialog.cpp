#include "utfdialog.h"
#include "ui_utfdialog.h"
#include "mainwindow.h"

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

bool UTFDialog::event(QEvent *e) {
    if(e->type()==QEvent::StatusTip){
      QStatusTipEvent *ev = (QStatusTipEvent*)e;
      ((MainWindow *)parentWidget())->status(ev->tip());
      return true;
    }
    return QDialog::event(e);
}
