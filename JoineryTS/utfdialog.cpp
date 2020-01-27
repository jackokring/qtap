#include "utfdialog.h"
#include "ui_utfdialog.h"
#include "adialog.h"
#include "../mainwindow.h"

#define UNICODEMAX 0x10ffff
#define SURRAMIN 0xd800
#define SURRAMAX 0xdfff

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

/* bool checkBad(uchar i) {
    if(i >= 0xc0 && i <= 0xc1) return true;
    if(i >= 0xf4) return true;
    return false;
} */

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
        } else if(((uchar)*i) == 127) {//DEL
            output += QByteArray(QString(0x2421).toUtf8());
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

bool switchLimits(int code, int count) {
    bool error = false;
    switch(count) {
    case 1:
        //ASCII (NO ERROR)
        break;
    case 2:
        //11 BIT (ENC OF ASCII?)
        if(code < 128) error = true;
        break;
    case 3:
        //16 BIT (ENC OF < 12 BIT)
        if(code < 2048) error = true;
        break;
    case 4:
        //21 BIT (ENC OF < 17 BIT)
        if(code < 65536 * 2) error = true;
        if(code > UNICODEMAX) error = true;
        break;
    default:
        break;
    }
    if(code >= SURRAMIN && code <= SURRAMAX) error = true;
    return error;
}

void UTFDialog::markWarn() {
    //marks all overlong and use of invalid UTF-8 code points such as surragates
    QByteArray input = old;
    QByteArray output = QByteArray();
    QByteArray::iterator i;
    bool error = false;
    bool check = false;
    int c = 0;
    int last;
    int lastCount;
    int count = 0;
    for(i = input.begin(); i != input.end(); ++i) {
        last = c;
        lastCount = count;
        if(((uchar)*i) > 127) {//continuation?
            if(((uchar)*i) > 127 + 64) {//double
                check = true;
                count = 1;
                c = (*i) & 0x1f;//5 bit initial
                if(((uchar)*i) > 127 + 64 + 32) {//triple
                    c &= 0xf;//cut to 4 bit
                    if(((uchar)*i) > 127 + 64 + 32 + 16) {//quad
                        c &= 0x7;//cut to 3 bit
                        if(((uchar)*i) > 127 + 64 + 32 + 16 + 8) {//error
                            c = 0;//might trigger error as good side effect
                        }
                    }
                }
            } else {
                count++;
                if(count > 4) {
                    count = 4;//limit for locking
                } else {
                    c = (c << 6) + ((*i) & 0x3f);//6 bit extend
                }
            }
        } else {
            //ASCII always OK
            c = (*i);
            count = 1;
            check = true;
        }
        if(check) {
            check = false;
            error = switchLimits(last, lastCount);
        }
        if(error) {
            output += marker();
            error = false;
        }
        output += (*i);
    }
    error = switchLimits(c, count);
    if(error) {
        output += marker();
        error = false;
    }
    old = QByteArray(output);
    setModified();
}

void UTFDialog::markError() {
    //catches all except overlong and surragates (depends on OEM as to further errors)
    QByteArray input = old;
    QByteArray output = QByteArray();
    QByteArray::iterator i;
    bool error = false;
    int count = 0;
    for(i = input.begin(); i != input.end(); ++i) {
        /* if(checkBad(*i)) {
            error = true;
            count = 0;
        } */
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
