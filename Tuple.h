#ifndef Tuple_h
#define Tuple_h

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
};

bool operator<(const Tuple& tuple1, const Tuple& tuple2);

#endif /* Tuple_h */
