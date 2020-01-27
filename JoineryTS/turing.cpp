#include "turing.h"
#include "ui_turing.h"

Turing::Turing(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Turing) {
    ui->setupUi(this);
}

Turing::~Turing() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Turing::getViewName() {
    return tr("T&uring");
}

QString Turing::getIconName() {
    return "view-turing";
}

QKeySequence Turing::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_U);
}
