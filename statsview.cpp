#include "statsview.h"
#include "mainwindow.h"
#include "ui_statsview.h"
#include <QAction>

StatsView::StatsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatsView)
{
    ui->setupUi(this);
}

StatsView::~StatsView()
{
    delete ui;
}

QString StatsView::getViewName() {
    return tr("&Statistics");
}

QString StatsView::getIconName() {
    return "view-statistics";
}

QKeySequence StatsView::getShortCut() {
    return QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
}

QString StatsView::getToolTipHelp() {
    return tr("Show the ") + getViewName().replace("&", "") + tr(" view.");
}

void StatsView::checkAvailable() {
    setAvailable(true);//pass through
}

void StatsView::defaultAvailable() {
    setAvailable(false);//pass through (default zero state or unsaved)
}

void StatsView::create() {
    //this is the on show call
}

bool StatsView::needsSave() {
    return false;
}

QString StatsView::getExtension() {
    return nullptr;//no save
}

QString StatsView::blockingSave() {
    return nullptr;//file content to save
}

void StatsView::setCommands() {
    //add any addMenu calls here
}

void StatsView::setMainWindow(QMainWindow *mw) {
    main = mw;
}

void StatsView::selectView() {
    if(main != nullptr) {
        create();
        ((MainWindow *)main)->setMain(this);
    }
}

void StatsView::addMenu(void(StatsView::*fp)(),
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
