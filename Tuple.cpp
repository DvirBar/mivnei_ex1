#include "Tuple.h"


Tuple::Tuple(int primary, int secondary, int tertiary) :
primaryValue(primary),
secondaryValue(secondary),
tertiaryValue(tertiary)
{}

Tuple::Tuple():
primaryValue(0),
secondaryValue(0),
tertiaryValue(0)
{}

bool operator>(const Tuple& tuple1, const Tuple& tuple2) {
    if(tuple1.primaryValue > tuple2.primaryValue)
        return true;

    if(tuple1.primaryValue == tuple2.primaryValue) {
        if(tuple1.secondaryValue < tuple2.secondaryValue)
            return true;

        if(tuple1.secondaryValue == tuple2.secondaryValue)
            return tuple1.tertiaryValue > tuple2.tertiaryValue;

        return false;
    }

    return false;

}

bool operator==(const Tuple& tuple1, const Tuple& tuple2) {
    return tuple1.primaryValue == tuple2.primaryValue && tuple1.secondaryValue == tuple2.secondaryValue && tuple1.tertiaryValue == tuple2.tertiaryValue;
}

bool operator<(const Tuple& tuple1, const Tuple& tuple2) {
    return tuple2 > tuple1;
}

ostream& operator<<(ostream& os, const Tuple& tuple) {
   os << "<" << tuple.primaryValue << ", " << tuple.secondaryValue <<
   ", " << tuple.tertiaryValue << ">";
   
   return os;
}
