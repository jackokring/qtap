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
    //bool nest;
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

/*=============================================================================
 * A more compact QMultiHash with tradeoff of removal.
 * ---------------------------------------------------
 * As the T key is not stored in full within the structure a large saving in
 * memory is achived as long as no iteration over included keys is required.
 *
 * Indirectly all keys become 32 bit ints, and this is feature compression.
 * The memory requirement for keys still can be quite large for many keys.
 *
 * Reducing the values memory requirements is more difficult. The 'key'
 * storage can however be improved upon, along with some more benefits.
 * ==========================================================================*/
template<class T, class K>
class FoolMap {
public:
    FoolMap();
    ~FoolMap();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void insert(T key, K value);
    virtual QList<K> values(T key);
    virtual void remove(T key, K value);//requires both

protected:
    QMultiHash<uint32_t, K> map;
};

/*=============================================================================
 * A compact QMultiHash with interesting properties for AI.
 * --------------------------------------------------------
 * The T key is stored in a bloom filter type structure to allow the QMultiHash
 * to be keyed by the value class.
 *
 * This has implications for sequencing, when the K value class is considered
 * to be some kind of microcode and needs placement of the values (action
 * control) to be closely associated based on kind, for example when using
 * a genetic algorithm population space within K.
 *
 * Further K can also be keyed into the K production map, to give more
 * precise sequential execution and T stimulus tuning to a good K starter.
 *
 * Particular emphisis is placed on *types in the K class. As they become
 * non K value variants, and so can place locality changes without K hash
 * alteration on the &/sizeof() hash implementation expected.
 * ==========================================================================*/
template<class T, class K>
class MasterMap {
public:
    MasterMap();
    ~MasterMap();

    //===================================================
    // INTERFACE
    //===================================================
    void insert(T key, K value) override;
    QList<K> values(T key) override;
    void remove(T key, K value) override;//requires both
    virtual QList<K> next(uint32_t index);//for jump pointers and program counters

protected:
    TripleFilter<T> stimulusMap[32];
    virtual void keyHook(T key, ClassFilter<T> *filter, bool keyBit);
};

#endif // CLASSFILTER_H
