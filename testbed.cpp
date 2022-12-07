#include "AVLTree.h"
#include "Player.h"
#include "Exception.h"

//
//template<class T>
//void printArray(T* arr, int size);

int main() {
    AVLTree<int, int> *tree = new AVLTree<int, int>();
    int size1 = 6;
    for(int i=1; i<=size1; i++) {
        tree->insert(i, i);
    }
    
    int size2 = 20;
    for(int i=10; i<=size2; i++) {
        tree->insert(i, i);
    }

    tree->printTree();
    cout << endl;
    try {
        cout << "First in range is " << tree->findFirstInRange(20) << endl;
    } catch(const KeyNotFound& error) {
        cout << "not found" << endl;
    }
    
//    tree->printTree();
    
//    for(int i=0; i<size; i++) {
//        cout << output[i].getValue() << " ";
//    }
//
    cout << endl;

//    try {
//
//    } catch(const NoNextInorder& error) {
//        cout << key << " doesn't have next" << endl;
//    } catch(const NoPrevInorder& error) {
//        cout << key << " doesn't have prev" << endl;
//    } catch(const KeyNotFound& error) {
//        cout << key << " not found" << endl;
//    }
 }

//template<class T>
//void printArray(T* arr, int size) {
//    for(int i=0; i<size; i++) {
//        cout << arr[i] << " ";
//    }
//
//    cout << endl;
//}


