#include "reducers.h"

template<class T, class K>
FoolReducer<T, K>::FoolReducer() {

}

template<class T, class K>
FoolReducer<T, K>::~FoolReducer() {

}

//===================================================
// INTERFACE
//===================================================
template<class T, class K>
void FoolReducer<T, K>::insert(QList<K> values) {
    todo.append(values);
}

template<class T, class K>
T FoolReducer<T, K>::value() {
    return todo;//a full internal list as a singular
}

template<class T, class K>
void FoolReducer<T, K>::remove(QList<K> values) {
    QListIterator<K> i;
    for(i = values.begin(); i != values.end(); ++i) {
        todo.removeAll(*i);
    }
}
