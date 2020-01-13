#include "atextedit.h"

ATextEdit::ATextEdit(QWidget *parent) : QPlainTextEdit(parent) {

}

QString ATextEdit::getBaseExtension() {
    return "txt";
}

QString ATextEdit::getBaseTypeDescription() {
    return tr("Text");
}

