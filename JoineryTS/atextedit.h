#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>
#include <QSettings>

class ATextEdit : public QPlainTextEdit {
    Q_OBJECT
public:
    ATextEdit(QWidget *parent);

    //===================================================
    // FILE INTERFACE
    //===================================================
    virtual QString getBaseExtension();
    virtual QString getBaseTypeDescription();
    virtual void readSettings(QSettings *settings);

    //===================================================
    // SELECTION INTERFACE
    //===================================================
    virtual QString getIconName();
    virtual QString getInputName();
    virtual QKeySequence getShortcut();
    virtual QString getHelpText();

};

#endif // ATEXTEDIT_H
