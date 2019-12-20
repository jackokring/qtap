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
