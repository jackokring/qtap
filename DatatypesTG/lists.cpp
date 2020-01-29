#include "lists.h"

List::List() : QList<List>() {

}

List::~List() {

}

QString List::name() {
    return named;
}

void List::setName(QString name) {
    named = name;
}

//===================================================
// MORE SOPHISTICATED LISTS
//===================================================
template<class T, class K>
void NOP<T, K>::push(K add) {
    inside.push_front(add);
}

template<class T, class K>
K NOP<T, K>::pop() {
    return inside.pop_front();
}

template<class T, class K>
T NOP<T, K>::evaluate() {
    return T();
}

template<class T, class K>
K NOP<T, K>::recall() {
    return inside.first();
}

template<class T, class K>
K NOP<T, K>::hold() {
    LIT<T, K> val = LIT<T, K>();
    val.push(this);
    return val;
}

template<class T, class K>
T LIT<T, K>::evaluate() {
    return NOP<T, K>::inside;
}
