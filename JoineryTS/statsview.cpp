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

//===================================================
// GUI
//===================================================
QString StatsView::getViewName() {
    return tr("&Statistics");
}

QString StatsView::getIconName() {
    return "view-statistics";
}

QKeySequence StatsView::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::Key_W);
}

//===================================================
// STATE
//===================================================
void StatsView::recycle() {

}

void StatsView::clear() {

}

void StatsView::create() {
    //this is the on show call
}

void StatsView::decreate() {
    //this is the prepare for on show call
}

QString StatsView::getExtension() {
    return nullptr;//no save
}

void StatsView::blockingSave(QDataStream *output) {
    //to do an auto save of some cached data if it exists
    Q_UNUSED(output)
}

void StatsView::cacheLoad(QDataStream *input) {
    //to do an auto load of some cached data if it exists
     Q_UNUSED(input)
}

void StatsView::setCommands() {
    //add any addMenu calls here
}

void StatsView::readSettings(QSettings *settings) {

}

bool StatsView::hasRegenerate() {
    return false;
}

QString StatsView::regenerate() {
    return nullptr;
}

//===================================================
// ACTION
//===================================================
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
