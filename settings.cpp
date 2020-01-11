#include "settings.h"
#include "ui_settings.h"

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

}

void Settings::writeSettings(QSettings *settings) {

}

//EDIT OVERRIDES

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
