#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include "adialog.h"
#include "atextedit.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public ADialog {
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    void setText(ATextEdit *text);

public slots:
    void find();
    void replace();
    void replaceAll();

private:
    Ui::FindDialog *ui;
    ATextEdit *txt;
    static QString findDefault;
    static QString replaceDefault;
};

#endif // FINDDIALOG_H
