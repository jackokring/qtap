#ifndef HELLO_H
#define HELLO_H

#include <QWidget>
#include "aviewwidget.h"

namespace Ui {
class Hello;
}

class Hello : public AViewWidget {
    Q_OBJECT

public:
    explicit Hello(QWidget *parent = nullptr);
    ~Hello();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Hello *ui;
};

#endif // HELLO_H
