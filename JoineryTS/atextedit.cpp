#include "atextedit.h"

ATextEdit::ATextEdit(QWidget *parent) : QPlainTextEdit(parent) {

}

//===================================================
// FILE INTERFACE
//===================================================
QString ATextEdit::getBaseExtension() {
    return "txt";
}

QString ATextEdit::getBaseTypeDescription() {
    return tr("Text");
}

void ATextEdit::readSettings(QSettings *settings) {
    Q_UNUSED(settings)
}

//===================================================
// SELECTION INTERFACE
//===================================================
QString ATextEdit::getIconName() {
    return "view-text";//a basic dual, but ok
}

QString ATextEdit::getInputName() {
    return tr("&Text Input");
}

QKeySequence ATextEdit::getShortcut() {
    return QKeySequence(Qt::CTRL + Qt::Key_M);//M
}

QString ATextEdit::getHelpText() {
    return tr("Standard simple text input");
}

