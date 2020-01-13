#include "statsview.h"
#include "ui_statsview.h"

StatsView::StatsView(QWidget *parent) :
    AViewWidget(parent),
    ui(new Ui::StatsView) {
    ui->setupUi(this);
}

StatsView::~StatsView() {
    delete ui;
}

QString StatsView::getViewName() {
    return tr("&Statistics");
}

QString StatsView::getIconName() {
    return "view-statistics";
}

QKeySequence StatsView::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_W);
}

void StatsView::checkAvailable(bool saved) {
    Q_UNUSED(saved)
    setAvailable(true);//pass through
}

void StatsView::clear() {

}

void StatsView::create() {
    //this is the on show call
}

bool StatsView::needsSave() {
    return false;
}

bool StatsView::canCache() {
    return false;
}

QString StatsView::getExtension() {
    return nullptr;//no save
}

QString StatsView::blockingSave() {
    return nullptr;//file content to save
}

void StatsView::cacheLoad(QString input) {
    //to do an auto load of some cached data if it exists
}

void StatsView::setCommands() {
    //add any addMenu calls here
}

void StatsView::readSettings(QSettings *settings) {

}

void StatsView::writeSettings(QSettings *settings) {

}

bool StatsView::hasRegenerate() {
    return false;
}

QString StatsView::regenerate() {
    return nullptr;
}

bool StatsView::canCut() {
    return false;
}

bool StatsView::canCopy() {
    return false;
}

bool StatsView::canPaste() {
    return false;
}

bool StatsView::canUndo() {
    return false;
}

bool StatsView::canRedo() {
    return false;
}

void StatsView::cut() {

}

void StatsView::copy() {

}

void StatsView::paste() {

}

void StatsView::undo() {

}

void StatsView::redo() {

}
