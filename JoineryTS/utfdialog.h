#ifndef UTFDIALOG_H
#define UTFDIALOG_H

#include "adialog.h"

namespace Ui {
class UTFDialog;
}

class UTFDialog : public ADialog {
    Q_OBJECT

public:
    explicit UTFDialog(QWidget *parent = nullptr);
    ~UTFDialog();
    void setBytes(QByteArray ba);
    QByteArray bytes();
    void setModified();

    void latinInput();
    void visibleCtl();
    void markAnsi();
    void markWarn();
    void markError();

private:
    Ui::UTFDialog *ui;
    QByteArray old;
};

#endif // UTFDIALOG_H
