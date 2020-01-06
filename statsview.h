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
    virtual void checkAvailable();
    virtual void defaultAvailable();
    virtual void create();
    virtual bool needsSave();

public slots:
    void selectView();

signals:
    void setAvailable(bool isAvail);

private:
    Ui::StatsView *ui;
    QMainWindow *main;
};

#endif // STATSVIEW_H
