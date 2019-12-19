#include "atextedit.h"

ATextEdit::ATextEdit(QWidget *parent) : QPlainTextEdit(parent) {

}

void ATextEdit::pushCursor() {
    currPos = textCursor().position();
}

void ATextEdit::popCursor() {
    QTextCursor cursor = textCursor();
    cursor.setPosition(currPos, QTextCursor::MoveAnchor);//non select
    setTextCursor(cursor);
}

void ATextEdit::pushDoc() {
    text = document()->clone();
    pushCursor();
}

void ATextEdit::popDoc() {
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(text);
    text->setDocumentLayout(layout);
    setDocument(text);
    popCursor();
}

int ATextEdit::currPos = 0;
QTextDocument *ATextEdit::text = nullptr;

