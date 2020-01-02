#ifndef STATSVIEW_H
#define STATSVIEW_H

#include <QWidget>
#include <QMainWindow>
//#include "mainwindow.h"

namespace Ui {
class StatsView;
}

class StatsView : public QWidget
{
    Q_OBJECT

public:
    explicit StatsView(QWidget *parent);
    ~StatsView();
    virtual QString getViewName();
    virtual QString getIconName();
    virtual QKeySequence getShortCut();
    virtual QString getToolTipHelp();
    void setMainWindow(QMainWindow *mw);

public slots:
    virtual void selectView();

private:
    Ui::StatsView *ui;
    QMainWindow *main;
};

#endif // STATSVIEW_H
