#include "jockey.h"
#include "ui_jockey.h"

Jockey::Jockey(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Jockey) {
    ui->setupUi(this);
}

Jockey::~Jockey() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Jockey::getViewName() {
    return tr("&Jockey");
}

QString Jockey::getIconName() {
    return "view-jockey";
}

QKeySequence Jockey::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_J);
}
