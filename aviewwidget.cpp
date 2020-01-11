#include "aviewwidget.h"
#include <QAction>
#include "mainwindow.h"

AViewWidget::AViewWidget(QWidget *parent) :
    QWidget(parent) {

}

QString AViewWidget::getViewName() {
    return "<abstract blank view>";
}

QString AViewWidget::getIconName() {
    return nullptr;//void icon
}

QKeySequence AViewWidget::getShortCut() {
    return 0;//no shortcut
}

QString AViewWidget::getToolTipHelp() {
    return tr("Show the ") + getViewName().replace("&", "").toLower() + tr(" view.");
}

void AViewWidget::checkAvailable() {
    setAvailable(true);//pass through (when saved)
}

void AViewWidget::defaultAvailable() {
    setAvailable(false);//pass through (default zero state or unsaved)
}

void AViewWidget::clear() {
    //no action
}

void AViewWidget::create() {
    //this is the on show call
}

bool AViewWidget::needsSave() {
    return false;//no save needed
}

bool AViewWidget::canCache() {
    return false;//save a cache
}

QString AViewWidget::getExtension() {
    return "no.dot.thing";
}

QString AViewWidget::blockingSave() {
    return "";//file content to save
}

void AViewWidget::cacheLoad(QString input) {
    //to do an auto load of some cached data if it exists
    Q_UNUSED(input)
}

void AViewWidget::setCommands() {
    //add any addMenu calls here
}

void AViewWidget::readSettings(QSettings *settings) {
    //read used settings
    Q_UNUSED(settings)
}

void AViewWidget::writeSettings(QSettings *settings) {
    //write used settings
    Q_UNUSED(settings)
}

bool AViewWidget::hasRegenerate() {
    return false;//can make the .txt file back again
}

QString AViewWidget::regenerate() {
    return "";//the regeneration
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

void AViewWidget::progress100(QString message, QString cancel) {
    progress = new QProgressDialog(message, cancel, 0, 100, this);
    progress->setModal(true);
    progress->setMaximum(100);
    progress->setValue(0);
}

bool AViewWidget::setProgress(int percent) {
    progress->setValue(percent);
    return progress->wasCanceled();
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
