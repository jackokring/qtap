#ifndef JOCKEY_H
#define JOCKEY_H

#include <QWidget>
#include "aviewwidget.h"

namespace Ui {
class Jockey;
}

class Jockey : public AViewWidget {
    Q_OBJECT

public:
    explicit Jockey(QWidget *parent = nullptr);
    ~Jockey();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;

private:
    Ui::Jockey *ui;
};

#endif // JOCKEY_H
