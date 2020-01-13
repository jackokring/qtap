#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>

class ATextEdit : public QPlainTextEdit {
    Q_OBJECT
public:
    ATextEdit(QWidget *parent);

    virtual QString getBaseExtension();
    virtual QString getBaseTypeDescription();

};

#endif // ATEXTEDIT_H
