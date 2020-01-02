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
    return tr("Statisti&cs");
}

QString StatsView::getIconName() {
    return "void";
}

QKeySequence StatsView::getShortCut() {
    return 0;
}

QString StatsView::getToolTipHelp() {
    return "Show the " + getViewName().replace("&", "") + " view.";
}

void StatsView::selectView() {
    ((MainWindow *)parentWidget())->setMain(this);
}
