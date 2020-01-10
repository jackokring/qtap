#ifndef UTFDIALOG_H
#define UTFDIALOG_H

#include <QDialog>

namespace Ui {
class UTFDialog;
}

class UTFDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UTFDialog(QWidget *parent = nullptr);
    ~UTFDialog();
    void setBytes(QByteArray ba);
    QByteArray bytes();

private:
    Ui::UTFDialog *ui;
    QByteArray old;
};

#endif // UTFDIALOG_H
