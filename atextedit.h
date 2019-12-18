#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>

class ATextEdit : public QPlainTextEdit
{
public:
    ATextEdit(QWidget *parent);
    ~ATextEdit();

    void saveCursor();
    void restoreCursor();
    void saveDoc();
    void restoreDoc();

private:
    static int currPos;
    static QString text;
    static bool isModified;
};

#endif // ATEXTEDIT_H
