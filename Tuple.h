#ifndef Tuple_h
#define Tuple_h

#include <iostream>

using namespace std;

class Tuple {
public:
    Tuple();
    Tuple(int primary, int secondary, int tertiary);
    Tuple(const Tuple& tuple) = default;
    Tuple& operator=(const Tuple& tuple) = default;
    ~Tuple() = default;

private:
    int primaryValue;
    int secondaryValue;
    int tertiaryValue;

    friend bool operator>(const Tuple& tuple1, const Tuple& tuple2);
    friend bool operator==(const Tuple& tuple1, const Tuple& tuple2);
    friend ostream& operator<<(ostream& os, const Tuple& tuple);
};

bool operator<(const Tuple& tuple1, const Tuple& tuple2);

#endif /* Tuple_h */
