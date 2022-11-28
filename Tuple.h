#ifndef Tuple_h
#define Tuple_h

class Tuple {
public:
    Tuple(int size);
    Tuple(const Tuple& tuple);
    Tuple& operator=(const Tuple& tuple);
    ~Tuple();
    
    void insertInIndex(int index, int data);
    
    class InvalidSize {};
    class InvalidIndex {};
    class TupleSizesDontMatch{};
private:
    int size;
    int* data;
};

#endif /* Tuple_h */
