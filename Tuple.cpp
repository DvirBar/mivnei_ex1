#include "Tuple.h"


Tuple::Tuple(int primary, int secondary, int tertiary) :
primaryValue(primary),
secondaryValue(secondary),
tertiaryValue(tertiary)
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


