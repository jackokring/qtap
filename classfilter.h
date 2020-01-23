#ifndef CLASSFILTER_H
#define CLASSFILTER_H

#include <QtCore>
#include "kringlicence.h"

/*=============================================================================
 * An extendable bloom filter with an extension to allow subtractive
 * training on classification problems. If each class is represented
 * by one of these filters, the query time maybe serially high, but
 * accuracy and density for mutually exclusive sets would be high.
 *
 * It's not a neural deep net, and it has learning by counter example
 * of set exclusion. It may actually forget things, and very rarely
 * remember incorrect inclusives. The aim of the anti filter is to
 * remember exceptions, and reduce the load on the 3rd exception filter.
 * ==========================================================================*/

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
    virtual ClassFilter<T> extend();

protected:
    ClassFilter<T> *more = nullptr;
    uint64_t density;
    int hashCount;
    uint64_t length;
    uint32_t *array;
    uint64_t count = 0;//used for density
    uint32_t *hashMix;
    bool nest;
    bool testBit(uint64_t bit, ClassFilter **last = nullptr);
    bool setBit(uint64_t bit, bool propergate = false);//return need propergate
    uint64_t hashThing(T thing, int count);
};

template<class T>
class DoubleFilter : ClassFilter<T> {
public:
    DoubleFilter(double densityIn = 0.5, int hashCountIn = 5, uint64_t lengthIn = 4096);
    ~DoubleFilter();

    //===================================================
    // INTERFACE
    //===================================================
    void add(T thing) override;
    bool in(T thing) override;
    ClassFilter<T> extend() override;
    void subtract(T thing);//<- THIS IS NOT THE OPPOSITE OF add(T).

    ClassFilter<T> *anti;
    ClassFilter<T> *pro;
};

/*=============================================================================
 * Makes a nested double filter (7 stage (1 + !3(1 + !1 + 1) + 3(1 + !1 + 1)))
 * ==========================================================================*/
template<class T>
class TripleFilter : DoubleFilter<T> {
public:
    TripleFilter(double densityIn = 0.5, int hashCountIn = 5, uint64_t lengthIn = 4096);

    //===================================================
    // INTERFACE
    //===================================================
    ClassFilter<T> extend() override;
};


QString vectorDecompose(double *input, uint size, uint divisions = 16);

#endif // CLASSFILTER_H
