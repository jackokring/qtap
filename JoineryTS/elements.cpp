#include "elements.h"
#include "ui_elements.h"

Elements::Elements(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Elements) {
    ui->setupUi(this);
}

Elements::~Elements() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Elements::getViewName() {
    return tr("&Elements");
}

QString Elements::getIconName() {
    return "view-elements";
}

QKeySequence Elements::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_E);
}
