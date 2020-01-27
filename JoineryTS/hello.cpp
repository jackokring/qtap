#include "hello.h"
#include "ui_hello.h"

Hello::Hello(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::Hello) {
    ui->setupUi(this);
}

Hello::~Hello() {
    delete ui;
}

//===================================================
// GUI
//===================================================
QString Hello::getViewName() {
    return tr("&Hello");
}

QString Hello::getIconName() {
    return "view-hello";
}

QKeySequence Hello::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_H);
}
