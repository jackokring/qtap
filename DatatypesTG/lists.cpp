#include "lists.h"

List::List() : QList<List>() {

}

List::~List() {

}

//===================================================
// INTERFACE
//===================================================
QString List::name() {
    return named;
}

void List::setName(QString name) {
    named = name;
}



TList::TList() : List() {

}

TList::~TList() {

}



KList::KList() {

}

KList::~KList() {

}
