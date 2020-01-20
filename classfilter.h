#ifndef CLASSFILTER_H
#define CLASSFILTER_H

#include <QtCore>
#include "kringlicence.h"

template<class T>
class ClassFilter {
public:
    ClassFilter(double densityIn = 0.5, int hashCountIn = 5, uint64_t lengthIn = 4096);
    ~ClassFilter();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void add(T thing);
    virtual bool in(T thing);

    ClassFilter<T> *more = nullptr;
    uint64_t density;
    int hashCount;
    uint64_t length;
    uint32_t *array;
    uint64_t count = 0;//used for density
    uint32_t *hashMix;
protected:
    bool testBit(uint64_t bit, ClassFilter **last = nullptr);
    bool setBit(uint64_t bit, bool propergate = false);//return need propergate
    uint64_t hashThing(T thing, int count);
};

template<class T>
class DoubleFilter {
public:
    DoubleFilter(double densityIn = 0.5, int hashCountIn = 5, uint64_t lengthIn = 4096);
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
