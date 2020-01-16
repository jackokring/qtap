#include "utfdialog.h"
#include "ui_utfdialog.h"
#include "adialog.h"
#include "mainwindow.h"

UTFDialog::UTFDialog(QWidget *parent) :
    ADialog(parent),
    ui(new Ui::UTFDialog) {
    ui->setupUi(this);
    connect(ui->latinButton, &QPushButton::clicked, this, &UTFDialog::latinInput);
    connect(ui->visibleCtlButton, &QPushButton::clicked, this, &UTFDialog::visibleCtl);
    connect(ui->escAnsiButton, &QPushButton::clicked, this, &UTFDialog::markAnsi);
    connect(ui->markWarnButton, &QPushButton::clicked, this, &UTFDialog::markWarn);
    connect(ui->markErrorsButton, &QPushButton::clicked, this, &UTFDialog::markError);
}

UTFDialog::~UTFDialog() {
    delete ui;
}

void UTFDialog::setBytes(QByteArray ba) {
    old = ba;
}

QByteArray UTFDialog::bytes() {
    return old;
}

void UTFDialog::setModified() {
    ((MainWindow *)parentWidget())->setLoadModified();
}

void UTFDialog::latinInput() {
    QByteArray input = old;
    QString output = QString();
    QByteArray::iterator i;
    for(i = input.begin(); i != input.end(); ++i) {
        output += (*i);
    }
    old = QByteArray(output.toUtf8());
    setModified();
}

bool notNeeded(char i) {
    bool ret = true;
    if(i == '\t' || i == '\n') ret = false;
    return ret;
}

QByteArray mapASCII(uchar i) {
    QChar c(i + 0x2400);
    return QByteArray(QString(c).toUtf8());
}

QByteArray marker() {
    return QByteArray(QString(0x2620).toUtf8());
}

bool checkBad(uchar i) {
    if(i >= 0xc0 && i <= 0xc1) return true;
    if(i >= 0xf4) return true;
    return false;
}

QByteArray mapANSI(uchar i) {
    QChar c((i & 0x3f) + 0x80);
    if(c < (128 + 32)) {
        return QByteArray((QString(27) + QChar((i & 0x3f) + 0x40)).toUtf8());
    } else {
        return QByteArray(QString(c).toUtf8());
    }
}

void UTFDialog::visibleCtl() {
    markAnsi();
    QByteArray input = old;
    QByteArray output = QByteArray();
    QByteArray::iterator i;
    for(i = input.begin(); i != input.end(); ++i) {
        if(((uchar)*i) < 32 && notNeeded(*i)) {
            output += mapASCII(*i);
        } else {
            output += (*i);
        }
    }
    old = QByteArray(output);
    setModified();
}

void UTFDialog::markAnsi() {
    QByteArray input = old;
    QByteArray output = QByteArray();
    QByteArray::iterator i;
    for(i = input.begin(); i != input.end(); ++i) {
        if(((uchar)*i) == 0b11000010) {//
            if(i != input.end()) {
                ++i;
            } else {
                output += marker();
                break;
            }
            output += mapANSI(*i);
        } else {
            output += (*i);
        }
    }
    old = QByteArray(output);
    setModified();
}

void UTFDialog::markWarn() {

}

void UTFDialog::markError() {
    //catches all except overlong and surragates
    QByteArray input = old;
    QByteArray output = QByteArray();
    QByteArray::iterator i;
    bool error = false;
    int count = 0;
    for(i = input.begin(); i != input.end(); ++i) {
        if(checkBad(*i)) {
            error = true;
            count = 0;
        }
        if(((uchar)*i) > 127) {//continuation?
            if(((uchar)*i) > 127 + 64) {//double
                count = 1;
                if(((uchar)*i) > 127 + 64 + 32) {//triple
                    count = 2;
                    if(((uchar)*i) > 127 + 64 + 32 + 16) {//quad
                        count = 3;
                        if(((uchar)*i) > 127 + 64 + 32 + 16 + 8) {//error
                            error = true;
                            count = 0;
                        }
                    }
                }
            } else {
                count--;
                if(count < 0) {
                    error = true;
                    count = 0;
                }
            }
        } else {
            if(count > 0) error = true;//as return ascii before counted
        }
        if(error) {
            output += marker();
            error = false;
        }
        output += (*i);
    }
    if(count > 0) output += marker();//terminal
    old = QByteArray(output);
    setModified();
}
