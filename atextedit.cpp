#include "atextedit.h"

ATextEdit::ATextEdit(QWidget *parent) : QPlainTextEdit(parent) {
}

ATextEdit::~ATextEdit() {

}

void ATextEdit::saveCursor() {
    currPos = textCursor().position();
}

void ATextEdit::restoreCursor() {
    QTextCursor cursor = textCursor();
    cursor.setPosition(currPos, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
}

void ATextEdit::saveDoc() {
    text = document()->toPlainText();
    saveCursor();
    isModified = document()->isModified();
}

void ATextEdit::restoreDoc() {
    document()->setPlainText(text);
    restoreCursor();
    document()->setModified(isModified);
    setFocus();
}

int ATextEdit::currPos = 0;
QString ATextEdit::text = "";
bool ATextEdit::isModified = false;
