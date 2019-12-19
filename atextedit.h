#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>

class ATextEdit : public QPlainTextEdit
{
public:
    ATextEdit(QWidget *parent);

    void pushCursor();
    void popCursor();
    void pushDoc();
    void popDoc();

private:
    static int currPos;
    static QTextDocument *text;
};

#endif // ATEXTEDIT_H
