#ifndef ADIALOG_H
#define ADIALOG_H

#include <QDialog>

class ADialog : public QDialog {
public:
    ADialog(QWidget *parent);
    ~ADialog();
    bool event(QEvent *e) override;
};

#endif // ADIALOG_H
