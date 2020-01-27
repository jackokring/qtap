#ifndef TURING_H
#define TURING_H

#include <QWidget>
#include "aviewwidget.h"

namespace Ui {
class Turing;
}

class Turing : public AViewWidget {
    Q_OBJECT

public:
    explicit Turing(QWidget *parent = nullptr);
    ~Turing();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Turing *ui;
};

#endif // TURING_H
