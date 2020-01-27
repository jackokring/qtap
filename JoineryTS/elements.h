#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <QWidget>
#include "aviewwidget.h"

namespace Ui {
class Elements;
}

class Elements : public AViewWidget {
    Q_OBJECT

public:
    explicit Elements(QWidget *parent = nullptr);
    ~Elements();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Elements *ui;
};

#endif // ELEMENTS_H
