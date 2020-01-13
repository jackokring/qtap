#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>

class ATextEdit : public QPlainTextEdit {
    Q_OBJECT
public:
    ATextEdit(QWidget *parent);

    //===================================================
    // FILE INTERFACE
    //===================================================
    virtual QString getBaseExtension();
    virtual QString getBaseTypeDescription();

    //===================================================
    // SELECTION INTERFACE
    //===================================================
    virtual QString getIconName();
    virtual QString getInputName();
    virtual QKeySequence getShortcut();
    virtual QString getHelpText();

};

#endif // ATEXTEDIT_H
