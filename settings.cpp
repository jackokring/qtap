#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    StatsView(parent, true),//stop stats UI render
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}
