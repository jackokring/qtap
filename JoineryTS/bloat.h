#ifndef BLOAT_H
#define BLOAT_H

#include <QWidget>
#include "aviewwidget.h"

namespace Ui {
class Bloat;
}

class Bloat : public AViewWidget {
    Q_OBJECT

public:
    explicit Bloat(QWidget *parent = nullptr);
    ~Bloat();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Bloat *ui;
};

#endif // BLOAT_H
