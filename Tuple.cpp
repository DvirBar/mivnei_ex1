#include "Tuple.h"


Tuple::Tuple(int size):
    size(size)
{
    if(size <= 0) {
        throw InvalidSize();
    }
    
    data = new int[size];
}

Tuple::Tuple(const Tuple& tuple):
    size(tuple.size),
    data(new int[tuple.size])
{
    for(int i=0; i<size; i++) {
        data[i] = tuple.data[i];
    }
}

Tuple& Tuple::operator=(const Tuple& tuple) {
    if(this == &tuple) {
        return *this;
    }
    
    if(tuple.size != size) {
        throw TupleSizesDontMatch();
    }
    
    delete[] data;
    size = tuple.size;
    for (int i=0; i<=size; i++) {
        data[i] = tuple.data[i];
    }
    
    return *this;
}

Tuple::~Tuple() {
    delete[] data;
}

void Tuple::insertInIndex(int index, int data) {
    if(index < 0 || index >= size) {
        throw InvalidIndex();
    }
    
    data[index] = data;
}



