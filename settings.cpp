#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    AViewWidget(parent),//stop stats UI render
    ui(new Ui::Settings) {
    ui->setupUi(this);
}

Settings::~Settings() {
    delete ui;
}

//SPECIAL AS ONLY NEEDS A FEW OVERRIDES

void Settings::readSettings(QSettings *settings) {
    QList<AViewWidget *>::iterator i;//restore
    for(i = ((MainWindow *)getMain())->begin(); i != ((MainWindow *)getMain())->end(); ++i) {
        settings->beginGroup((*i)->getViewName());
        readSettings(settings, *i);
        settings->endGroup();
    }
}

void Settings::writeSettings(QSettings *settings) {
    QList<AViewWidget *>::iterator i;//restore
    for(i = ((MainWindow *)getMain())->begin(); i != ((MainWindow *)getMain())->end(); ++i) {
        settings->beginGroup((*i)->getViewName());
        writeSettings(settings, *i);
        settings->endGroup();
    }
}

//===================================================
// ACTION
//===================================================S
bool Settings::canCut() {
    return false;
}

bool Settings::canCopy() {
    return false;
}

bool Settings::canPaste() {
    return false;
}

bool Settings::canUndo() {
    return false;
}

bool Settings::canRedo() {
    return false;
}

void Settings::cut() {

}

void Settings::copy() {

}

void Settings::paste() {

}

void Settings::undo() {

}

void Settings::redo() {

}

//===================================================
// USEFUL
//===================================================
void Settings::readSettings(QSettings *settings, AViewWidget *view) {

}

void Settings::writeSettings(QSettings *settings, AViewWidget *view) {

}
