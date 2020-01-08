#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
//included to get class inheritance
#include "statsview.h"

namespace Ui {
class Settings;
}

class Settings : public StatsView //inherit
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
