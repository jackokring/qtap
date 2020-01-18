#ifndef CLASSFILTER_H
#define CLASSFILTER_H

#include <QtCore>

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
    bool testBitFind(int bit, ClassFilter **last = nullptr);
    bool testBitCount(int bit, ClassFilter **last = nullptr);
    bool setBit(int bit, bool propergate = false);//return bit before set
    int hashThing(T thing, int count);
};

#endif // CLASSFILTER_H
