#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "atextedit.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    void setText(ATextEdit *text);

public slots:
    void find();
    void replace();
    void replaceAll();

protected:
    bool event(QEvent *e) override;

private:
    Ui::FindDialog *ui;
    ATextEdit *txt;
    static QString findDefault;
    static QString replaceDefault;
};

#endif // FINDDIALOG_H
