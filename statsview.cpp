#include "statsview.h"
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
    return QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
}

QString StatsView::getToolTipHelp() {
    return tr("Show the ") + getViewName().replace("&", "") + tr(" view.");
}

void StatsView::setMainWindow(MainWindow *mw) {
    main = mw;
}

void StatsView::selectView() {
    if(main != nullptr) main->setMain(this);
}
