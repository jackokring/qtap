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
    explicit StatsView(QWidget *parent = nullptr);
    ~StatsView();

private:
    Ui::StatsView *ui;
};

#endif // STATSVIEW_H
