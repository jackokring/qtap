#ifndef INTERNUT_H
#define INTERNUT_H

#include <QWidget>
#include "JoineryTS/aviewwidget.h"

namespace Ui {
class Internut;
}

class Internut : public AViewWidget {
    Q_OBJECT

public:
    explicit Internut(QWidget *parent = nullptr);
    ~Internut();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Internut *ui;
};

#endif // INTERNUT_H
