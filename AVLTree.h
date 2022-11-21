#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <memory>

using namespace std;

template <class T, class K>
class AVLTree
{
public:
    AVLTree();
    AVLTree(const AVLTree &tree);
    AVLTree &operator=(const AVLTree &tree);
    // TODO: Destructor?

    void addNode(const K &key, const T &data);
    void removeNode(const K &key);
    T &getNode(const K &key) const;

private:
    class AVLNode
    {
    public:
        K key;
        T data;
        unique_ptr<AVLNode> rightChild;
        unique_ptr<AVLNode> leftChild;
        int height;

        /**
         *  Constructor of AVLNode class.
         *
         *  @param data - a reference to data
         */
        AVLNode(const T &data);

        AVLNode(const AVLNode &) = default;
        AVLNode &operator=(const AVLNode &) = default;
    };

    void RRrotation(unique_ptr<AVLNode> root);
    void LLrotation(unique_ptr<AVLNode> root);
    void RLrotation(unique_ptr<AVLNode> root);
    void LRrotation(unique_ptr<AVLNode> root);
};

template <class T, class K>
void AVLTree<T, K>::RLrotation(unique_ptr<AVLNode> root) {
    unique_ptr<AVLNode> tempLeft = move(root->rightChild->leftChild->leftChild);
    unique_ptr<AVLNode> tempRight = move(root->rightChild->leftChild->rightChild);
    root->rightChild->leftChild->leftChild = move(root);
    root->rightChild->leftChild->rightChild = move(root->rightChild);
    tempLeft = 


}

template <class T, class K>
void AVLTree<T, K>::RRrotation(unique_ptr<AVLNode> root) {
    unique_ptr<AVLNode> tempNode = move(root->rightChild->leftChild);
    root->rightChild->leftChild = move(root);
    root->rightChild = move(tempNode);
}

#endif // AVLTREE_H_