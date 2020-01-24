#include "classfilter.h"

template<class T>
ClassFilter<T>::ClassFilter(double densityIn, int hashCountIn, uint64_t lengthIn) {
    density = densityIn * lengthIn * 32;//bits set for extend
    hashCount = hashCountIn;
    length = lengthIn;
    array = new uint32_t[length];
    for(int i = 0; i != length; ++i) {
        array[i] = 0;//clear
    }
    hashMix = new uint32_t[hashCount];
    for(int i = 0; i != hashCount; ++i) {
        hashMix[i] = QRandomGenerator::global()->generate();//fill
    }
}

template<class T>
ClassFilter<T>::~ClassFilter() {
    delete [] array;
    delete [] hashMix;
    if(more != nullptr) delete more;//mem fix
}

template<class T>
void ClassFilter<T>::add(T thing) {
    for(int i = 0; i != hashCount; ++i) {
        setBit(hashThing(thing, i), i == 0);//fill in likely hood
    }
}

template<class T>
bool ClassFilter<T>::in(T thing) {
    ClassFilter **last;
    ClassFilter *current;
    last = &current;//make an indirection
    (*last) = nullptr;//sets current
    ClassFilter *lastCurrent;
    lastCurrent = current;
    bool out = true;
    for(int i = 0; i != hashCount; ++i) {
        if(current != nullptr) {
            out &= current->testBit(hashThing(thing, i), last);
        } else {
            out &= testBit(hashThing(thing, i), last);//fill in likely hood
        }
    }
    if(out) return true;
    if(current == nullptr) return false;
    if(current->more == nullptr) return false;
    return current->more->in(thing);//check for later finds of all keys
}

template<class T>
ClassFilter<T> ClassFilter<T>::extend() {
    return new ClassFilter<T>(((double)length * 32.0) / ((double)density), hashCount, length);
}

template<class T>
bool ClassFilter<T>::testBit(uint64_t bit, ClassFilter **last) {
    uint64_t x = (bit / 32) % length;
    uint32_t a = array[x];
    x = bit % 32;
    a &= (1 << x);
    bool out = a != 0;
    if(out && last != nullptr && (*last) == nullptr) {//fill in this
        (*last) = this;
    } else {
        if(!out && more != nullptr) {
            out = more->testBit(bit, last);
        }
    }
    return out;
}

template<class T>
bool ClassFilter<T>::setBit(uint64_t bit, bool propergate) {
    if(count < density) {
        uint64_t x = (bit / 32) % length;
        int y = bit % 32;
        if((array[x] & (1 << y)) != 0) {
            count++;//if not set
            array[x] |= (1 << y);
        }
        return false;
    } else {
        if(more == nullptr) {
            if(propergate) {
                more = extend();
                return more->setBit(bit, propergate);
            } else {
                uint64_t x = (bit / 32) % length;
                int y = bit % 32;
                if((array[x] & (1 << y)) != 0) {
                    count++;//if not set
                    array[x] |= (1 << y);
                }
                return true;
            }
        } else {
            return more->setBit(bit, propergate);
        }
    }
}

template<class T>
uint64_t ClassFilter<T>::hashThing(T thing, int count) {
    return qHash(thing) * qHash(hashMix[count]);
}

template<class T>
DoubleFilter<T>::DoubleFilter(double densityIn, int hashCountIn, uint64_t lengthIn) :
    ClassFilter<T>(densityIn, hashCountIn, lengthIn) {//bane number 1 C++
    anti = extend();
    pro = extend();
}

template<class T>
DoubleFilter<T>::~DoubleFilter() {
    delete anti;
    delete pro;
}

template<class T>
void DoubleFilter<T>::add(T thing) {
    if(anti->in(thing)) {
        pro->in(thing);
    } else {
        ClassFilter<T>::add(thing);
    }
}

template<class T>
bool DoubleFilter<T>::in(T thing) {
    bool first = ClassFilter<T>::in(thing);
    if(first) {
        if(!anti->in(thing)) {
            return true;
        } else {
            return pro->in(thing);
        }
    } else {
        return pro->in(thing);
    }
}

template<class T>
ClassFilter<T> DoubleFilter<T>::extend() {
    return new ClassFilter<T>(((double)ClassFilter<T>::length * 32.0) /
                                  ((double)ClassFilter<T>::density),
                                  ClassFilter<T>::hashCount, ClassFilter<T>::length);
}

template<class T>
void DoubleFilter<T>::subtract(T thing) {//can't always do it!
    if(ClassFilter<T>::in(thing)) {
        anti->in(thing);
    } else {
        //do nothing as can't subtract an added with this data structure
    }
}

template<class T>
TripleFilter<T>::TripleFilter(double densityIn, int hashCountIn, uint64_t lengthIn) :
    DoubleFilter<T>(densityIn, hashCountIn, lengthIn) {//bane number 1 C++
    delete DoubleFilter<T>::pro;
    delete DoubleFilter<T>::anti;
    DoubleFilter<T>::anti = extend();
    DoubleFilter<T>::pro = extend();
}

template<class T>
ClassFilter<T> TripleFilter<T>::extend() {
    return new DoubleFilter<T>(((double)ClassFilter<T>::length * 32.0) /
                                  ((double)ClassFilter<T>::density),
                                  ClassFilter<T>::hashCount, ClassFilter<T>::length);
}

QString vectorDecompose(double *input, uint size, uint divisions) {
    QString output = QString();
    double max = 0.0000000000001;
    for(uint i = 0; i != size; ++i) {
        if(input[i] > max) max = input[i];
    }
    double norm[size];
    for(uint i = 0; i != size; ++i) {
        norm[i] = input[i] / max;
        if(norm[i] < 0) norm[i] = 0;
    }
    for(uint i = 0; i != size; ++i) {
        output += QChar((uint)(norm[i] * divisions) + 32);
    }
    return output;
}

template<class T, class K>
FoolMap<T, K>::FoolMap() {

}

template<class T, class K>
FoolMap<T, K>::~FoolMap() {

}

template<class T, class K>
void FoolMap<T, K>::insert(T key, K value) {
    map.insert(qHash(key), value);
}

template<class T, class K>
QList<K> FoolMap<T, K>::values(T key) {
    map.values(qHash(key));
}

template<class T, class K>
void FoolMap<T, K>::remove(T key, K value) {//requires both
    map.remove(qHash(key), value);
}

