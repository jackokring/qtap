#ifndef CLASSFILTER_H
#define CLASSFILTER_H

#include <QtCore>
#include "kringlicence.h"

template<class T>
class ClassFilter {
public:
    ClassFilter(double densityIn = 0.5, int hashCountIn = 5, int lengthIn = 4096);
    ~ClassFilter();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void add(T thing);
    virtual bool in(T thing);

    ClassFilter<T> *more = nullptr;
    int density;
    int hashCount;
    int length;
    uint32_t *array;
    long count = 0;//used for density
    QChar *hashMix;
protected:
    bool testBit(int bit, ClassFilter **last = nullptr);
    bool setBit(int bit, bool propergate = false);//return need propergate
    int hashThing(T thing, int count);
};

template<class T>
class DoubleFilter {
public:
    DoubleFilter(double densityIn = 0.5, int hashCountIn = 5, int lengthIn = 4096);
    ~DoubleFilter();

    //===================================================
    // INTERFACE
    //===================================================
    void add(T thing) override;
    bool in(T thing) override;
    virtual void subtract(T thing);

    ClassFilter<T> *anti;
    ClassFilter<T> *pro;
};

#endif // CLASSFILTER_H
