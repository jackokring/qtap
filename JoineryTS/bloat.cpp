#include "bloat.h"
#include "ui_bloat.h"

Bloat::Bloat(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Bloat) {
    ui->setupUi(this);
}

Bloat::~Bloat() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Bloat::getViewName() {
    return tr("&Bloat");
}

QString Bloat::getIconName() {
    return "view-bloat";
}

QKeySequence Bloat::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_B);
}
