#ifndef STATSVIEW_H
#define STATSVIEW_H

#include <QWidget>

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

public slots:
    virtual void selectView(bool triggered);

private:
    Ui::StatsView *ui;
};

#endif // STATSVIEW_H
