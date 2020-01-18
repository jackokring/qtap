#include "classfilter.h"

template<class T>
ClassFilter<T>::ClassFilter(double densityIn, int hashCountIn, int lengthIn) {
    density = densityIn * lengthIn * 32;//bits set for extend
    hashCount = hashCountIn;
    length = lengthIn;
    array = new uint32_t[length];
    for(int i = 0; i != length; ++i) {
        array[i] = 0;//clear
    }
    hashMix = new QChar[hashCount];
    for(int i = 0; i != hashCount; ++i) {
        hashMix[i] = QChar(QRandomGenerator::global()->generate());//fill
    }
}

template<class T>
ClassFilter<T>::~ClassFilter() {
    delete array;
}

template<class T>
void ClassFilter<T>::add(T thing) {

}

template<class T>
bool ClassFilter<T>::in(T thing) {

}

template<class T>
bool ClassFilter<T>::testBitFind(int bit, ClassFilter **last) {
    if(last != nullptr && (*last) != nullptr) {
        int x = (bit / 32) % length;
        uint32_t a = (*last)->array[x];
        x = bit % 32;
        a &= (1 << x);
        return a != 0;
    } else {

    }
}

template<class T>
bool ClassFilter<T>::testBitCount(int bit, ClassFilter **last) {
    if(last != nullptr && (*last) != nullptr) {
        int x = (bit / 32) % length;
        uint32_t a = (*last)->array[x];
        x = bit % 32;
        a &= (1 << x);
        return a != 0;
    } else {

    }
}

template<class T>
bool ClassFilter<T>::setBit(int bit, bool propergate) {
    if(count < density) {
        int x = (bit / 32) % length;
        int y = bit % 32;
        if((array[x] & (1 << y)) != 0) {
            count++;//if not set
            array[x] |= (1 << y);
            return true;
        }
        return false;
    } else {
        if(more == nullptr) {
            if(propergate) {
                more = new ClassFilter(((double)length * 32.0) / ((double)density), hashCount, length);
                return more->setBit(bit, propergate);
            } else {
                int x = (bit / 32) % length;
                int y = bit % 32;
                if((array[x] & (1 << y)) != 0) {
                    count++;//if not set
                    array[x] |= (1 << y);
                    return true;
                }
                return false;
            }
        } else {
            return more->setBit(bit, propergate);
        }
    }
}

template<class T>
int ClassFilter<T>::hashThing(T thing, int count) {
    QString n = thing.toString() + hashMix[count];
    return qHash(n);
}
