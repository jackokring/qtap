#include "adialog.h"
#include "mainwindow.h"

ADialog::ADialog(QWidget *parent) :
    QDialog(parent) {

}

ADialog::~ADialog() {

}

bool ADialog::event(QEvent *e) {
    if(e->type()==QEvent::StatusTip){
      QStatusTipEvent *ev = (QStatusTipEvent*)e;
      ((MainWindow *)parentWidget())->status(ev->tip());
      return true;
    }
    return QDialog::event(e);
}
