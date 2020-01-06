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
    explicit StatsView(QWidget *parent);//construction app start
    ~StatsView();
    virtual QString getViewName();
    virtual QString getIconName();
    virtual QKeySequence getShortCut();
    virtual QString getToolTipHelp();
    void setMainWindow(QMainWindow *mw);
    virtual void checkAvailable();
    virtual void defaultAvailable();
    virtual void create();//run on show
    virtual bool needsSave();
    virtual QString getExtension();
    virtual QString blockingSave();
    virtual void setCommands();

public slots:
    void selectView();
private:
    void addMenu(void(StatsView::*fp)() = nullptr,
                 QString named = nullptr,
                 QString entry = nullptr,
                 QKeySequence shorty = 0,
                 QString help = nullptr);

signals:
    void setAvailable(bool isAvail);

private:
    Ui::StatsView *ui;
    QMainWindow *main;
};

#endif // STATSVIEW_H
