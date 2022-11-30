#include "AVLTree.h"
#include "Player.h"
#include "Exception.h"

int main() {
    AVLTree<int, int> *tree = new AVLTree<int, int>();
    for(int i=1; i<=10; i++) {
        tree->insert(i, i);
    }

    tree->printTree();
    tree->remove(1);
    tree->printTree();
    tree->remove(2);
    tree->printTree();
    tree->remove(3);
    tree->printTree();
    tree->remove(8);
    tree->printTree();
    tree->remove(9);
    tree->printTree();
    cout << endl;
//    try {
//        cout << "Next inorder of 6: " << tree->nextInorder(6) << endl;
//        cout << "Prev inorder of 1: " << tree->prevInorder(2) << endl;
//    } catch(const NoNextInorder& error) {
//        cout << "No next inorder for 5" << endl;
//    } catch (const NoPrevInorder& error) {
//        cout << "No prev inorder for 1" << endl;
//    } catch(const KeyNotFound& error) {
//        cout << "No such key" << endl;
//    }
    
}
