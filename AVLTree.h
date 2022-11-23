#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <memory>
#include <math.h>

using namespace std;

template <class T, class K>
class AVLTree
{
private:

    class AVLNode
    {
    public:
        K key;
        T data;
        AVLNode* rightChild;
        AVLNode* leftChild;
//        AVLNode* parent;
        int height;

        /**
         *  Constructor of AVLNode class.
         *
         *  @param data - a reference to data
         */
        AVLNode(const T &data, const K& key);

        AVLNode(const AVLNode &) = default;
        AVLNode &operator=(const AVLNode &) = default;
    };

    AVLNode* root;

    T& searchByNode(const AVLNode* node, const K& key);
    AVLNode* insertByNode(AVLNode* node, const K& key, const T& data);
    AVLNode* RRrotation(AVLNode* parent);
    AVLNode* LLrotation(AVLNode* parent);
    AVLNode* RLrotation(AVLNode* parent);
    AVLNode* LRrotation(AVLNode* parent);
    int getBalanceFactor(AVLNode* node);
    void updateHeight(AVLNode* node);

public:
    AVLTree();
    AVLTree(const AVLTree &tree);
    AVLTree &operator=(const AVLTree &tree);
    // TODO: Destructor?

    AVLNode* insert(const K &key, const T &data);
    AVLNode* removeNode(const K &key);
    T &search(const K &key) const;


};

template <class T, class K>
int AVLTree<T, K>::getBalanceFactor(AVLTree::AVLNode* node) {
    if(node == nullptr)
        return 0;
    return (node->leftChild->height) - (node->rightChild->height);
}

template <class T, class K>
void AVLTree<T, K>::updateHeight(AVLTree::AVLNode* node) {
    node->height = max(node->leftChild->height, node->rightChild->height) + 1;
}

template <class T, class K>
typename AVLTree<T, K>::AVLNode* AVLTree<T, K>::RLrotation(AVLTree::AVLNode* parent) {
    AVLTree::AVLNode* parentRight = parent->rightChild;
    AVLTree::AVLNode* newParent = parentRight->leftChild;
    AVLTree::AVLNode* newParentLeft = newParent->leftChild;
    AVLTree::AVLNode* newParentRight = newParent->rightChild;

    newParent->leftChild = parent;
    newParent->rightChild = parentRight;
    parent->rightChild = newParentLeft;
    parentRight->leftChild = newParentRight;

    updateHeight(parent);
    updateHeight(parentRight);
    updateHeight(newParent);

    return newParent;
}

template <class T, class K>
typename AVLTree<T, K>::AVLNode* AVLTree<T, K>::RRrotation(AVLTree::AVLNode* parent) {
    AVLTree::AVLNode* newParent = parent->rightChild;
    AVLTree::AVLNode* parentRightLeft = newParent->leftChild;

    newParent->leftChild = parent;
    parent->rightChild = parentRightLeft;

    updateHeight(parent);
    updateHeight(newParent);

    return newParent;
}

template <class T, class K>
T& AVLTree<T, K>::searchByNode(const AVLTree::AVLNode* node, const K& key) {
    if(node == nullptr)
        return nullptr;
    if(node->key == key)
        return node->data;
    if(node->key > key)
        searchByNode(node->leftChild);
    else
        searchByNode(node->rightChild);
}

template <class T, class K>
T& AVLTree<T, K>::search(const K &key) const {
    return searchByNode(root);
}

template <class T, class K>
typename AVLTree<T, K>::AVLNode* AVLTree<T, K>::insertByNode(AVLTree::AVLNode* node, const K& key, const T& data) {
    if(node == nullptr) {
        AVLTree::AVLNode* newLeaf = new AVLTree::AVLNode(data, key);
        return newLeaf;
    }
    if(node->key > key)
        node->leftChild = insertByNode(node->leftChild, key, data);
    else
        node->rightChild= insertByNode(node->rightChild, key, data);

    updateHeight(node);

    int nodeBF = getBalanceFactor(node);
    int nodeLeftChildBF = getBalanceFactor(node->leftChild);
    int nodeRightChildBF = getBalanceFactor(node->rightChild);

    if((nodeBF == 2) && (nodeLeftChildBF == -1))
        return LRrotation(node);

    if((nodeBF == 2) && (nodeLeftChildBF >= 0))
        return LLrotation(node);

    if((nodeBF == -2) && (nodeRightChildBF == 1))
        return RLrotation(node);

    if((nodeBF == -2) && (nodeRightChildBF <= 0))
        return RRrotation(node);

    return node;

}



template <class T, class K>
typename AVLTree<T, K>::AVLNode* AVLTree<T, K>::insert(const K &key, const T &data) {
    insertByNode(root, key, data);
}

#endif // AVLTREE_H_