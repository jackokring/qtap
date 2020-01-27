#include "internut.h"
#include "ui_internut.h"

Internut::Internut(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Internut) {
    ui->setupUi(this);
}

Internut::~Internut() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Internut::getViewName() {
    return tr("&Internut");
}

QString Internut::getIconName() {
    return "view-internut";
}

QKeySequence Internut::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_I);
}
