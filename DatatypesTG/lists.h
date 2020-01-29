#ifndef LISTS_H
#define LISTS_H

#include <QtCore>
#include "../kringlicence.h"

class List : QList<List> {
public:
    List();
    ~List();

    //===================================================
    // INTERFACE
    //===================================================
    virtual QString name();
    virtual void setName(QString name);

protected:
    QString named;
};

template<class T, class K>
class NOP {
public:
    NOP();
    ~NOP();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void push(K add);
    virtual K pop();
    virtual T evaluate();
    virtual K recall();
    virtual K hold();

protected:
    QList<K> inside;
};

template<class T, class K>
class LIT : NOP<T, K> {
public:
    LIT();
    ~LIT();

    //===================================================
    // INTERFACE
    //===================================================
    //void inset(K add) override;
    //void remove(K sub) override;
    T evaluate() override;
    //QList<K> recall() override;
    //K hold() override;

protected:

};

#endif // LISTS_H
