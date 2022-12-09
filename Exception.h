#ifndef Exception_h
#define Exception_h

#include <exception>

using namespace std;

class InvalidArgumets{};
class KeyNotFound: public exception {
public:
    const char* what() const noexcept override {
        return "Key not found";
    }
};
class NoNextInorder {};
class NoPrevInorder {};
#endif /* Exception_h */
