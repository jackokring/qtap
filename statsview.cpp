#include "statsview.h"
#include "mainwindow.h"
#include "ui_statsview.h"

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

}

bool StatsView::needsSave() {
    return false;
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
