#include "aviewwidget.h"
#include <QAction>
#include "mainwindow.h"

AViewWidget::AViewWidget(QWidget *parent) :
    QWidget(parent) {

}

QString AViewWidget::getViewName() {
    return tr("&Statistics");
}

QString AViewWidget::getIconName() {
    return "view-statistics";
}

QKeySequence AViewWidget::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
}

QString AViewWidget::getToolTipHelp() {
    return tr("Show the ") + getViewName().replace("&", "").toLower() + tr(" view.");
}

void AViewWidget::checkAvailable() {
    setAvailable(true);//pass through
}

void AViewWidget::defaultAvailable() {
    setAvailable(false);//pass through (default zero state or unsaved)
}

void AViewWidget::clear() {

}

void AViewWidget::create() {
    //this is the on show call
}

bool AViewWidget::needsSave() {
    return false;
}

bool AViewWidget::canCache() {
    return false;
}

QString AViewWidget::getExtension() {
    return nullptr;//no save
}

QString AViewWidget::blockingSave() {
    return nullptr;//file content to save
}

void AViewWidget::cacheLoad(QString input) {
    //to do an auto load of some cached data if it exists
}

void AViewWidget::setCommands() {
    //add any addMenu calls here
}

void AViewWidget::readSettings(QSettings *settings) {

}

void AViewWidget::writeSettings(QSettings *settings) {

}

bool AViewWidget::hasRegenerate() {
    return false;
}

QString AViewWidget::regenerate() {
    return nullptr;
}

bool AViewWidget::canCut() {
    return false;
}

bool AViewWidget::canCopy() {
    return false;
}

bool AViewWidget::canPaste() {
    return false;
}

bool AViewWidget::canUndo() {
    return false;
}

bool AViewWidget::canRedo() {
    return false;
}

void AViewWidget::cut() {

}

void AViewWidget::copy() {

}

void AViewWidget::paste() {

}

void AViewWidget::undo() {

}

void AViewWidget::redo() {

}

QWidget *AViewWidget::focused() {
    return MainWindow::focused(this);
}

void AViewWidget::setMainWindow(QMainWindow *mw) {
    main = mw;
}

void AViewWidget::selectView() {
    if(main != nullptr) {
        create();
        ((MainWindow *)main)->setMain(this);
    }
}

QString AViewWidget::getTextFromMain() {
    return ((MainWindow *)main)->getText();
}

template<class slotz>
void AViewWidget::addMenu(void(slotz::*fp)(),
             QString named,
             QString entry,
             QKeySequence shorty,
             QString help) {
    const QIcon newIcon = MainWindow::getIconRC(named);
    if(entry == nullptr) entry = ">>Blank entry<<";
    QAction *newAct = new QAction(newIcon, entry, this);
    if(shorty > 0) newAct->setShortcut(shorty);
    if(help != nullptr) newAct->setStatusTip(help);
    if(fp != nullptr) connect(newAct, &QAction::triggered, this, fp);
    ((MainWindow *)main)->setCommand(newAct, this);
}
