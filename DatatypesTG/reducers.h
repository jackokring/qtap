#ifndef REDUCERS_H
#define REDUCERS_H

#include <QtCore>

template<class T, class K>
class FoolReducer {
public:
    FoolReducer();
    ~FoolReducer();

    //===================================================
    // INTERFACE
    //===================================================
    virtual void insert(QList<K> values);
    virtual T value();
    virtual void remove(QList<K> values);

protected:
    QList<K> todo;
};

#endif // REDUCERS_H
