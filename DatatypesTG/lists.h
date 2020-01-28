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

class TList : List {
public:
    TList();
    ~TList();

    //===================================================
    // INTERFACE
    //===================================================


protected:

};

class KList : List {
public:
    KList();
    ~KList();

    //===================================================
    // INTERFACE
    //===================================================


protected:

};

template<class T, class K>
class NOP {
public:
    NOP();
    ~NOP();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void inset(K add);
    virtual void remove(K sub);
    virtual T evaluate();
    virtual KList recall();
    virtual KList hold();

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
    virtual void inset(K add) override;
    virtual void remove(K sub) override;
    virtual T evaluate() override;
    virtual KList recall() override;
    virtual KList hold() override;

protected:

};

#endif // LISTS_H
