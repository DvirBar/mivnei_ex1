#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <memory>
#include "Pair.h"
#include <math.h>

using namespace std;

template <class T, class K>
class AVLTree
{
public:
    AVLTree();
    AVLTree(const AVLTree &tree) = delete;
    AVLTree &operator=(const AVLTree &tree) = delete;
    ~AVLTree();

    void addNode(const K &key, const T &data);
    void removeNode(const K &key);
    T &getNode(const K &key) const;
    int getHeight() const;
    bool isEmpty() const;

private:
    class AVLNode
    {
    public:
        K key;
        T data;
        AVLNode *rightChild;
        AVLNode *leftChild;
        int height;

        AVLNode(const T &data);

        AVLNode(const AVLNode &) = default;
        AVLNode &operator=(const AVLNode &) = default;

        int balanceFactor() const;
        void updateHeight();

        void RRrotation();
        void LLrotation();
        void RLrotation();
        void LRrotation();
    };

    AVLNode *root;
    void copyTree(const AVLTree &tree, AVLTree &newTree);
    void emptyTree();

    static void deleteTreeAux(AVLNode *node);
};

template <class T, class K>
AVLTree<T, K>::AVLTree() : root(nullptr)
{
}

template <class T, class K>
AVLTree<T, K>::AVLTree(const AVLTree &tree)
{
}

template <class T, class K>
AVLTree<T, K>::~AVLTree()
{
    deleteTreeAux(root);
}

template <class T, class K>
void AVLTree<T, K>::deleteTreeAux(AVLNode *node)
{
    if (node->leftChild != nullptr)
    {
        deleteNodeAux(node->leftChild);
    }

    if (node->rightChild != nullptr)
    {
        deleteNodeAux(node->rightChild);
    }

    delete node;
}

template <class T, class K>
void AVLTree<T, K>::removeNode(const K &key)
{
}

template <class T, class K>
void AVLTree<T, K>::AVLNode::LLrotation()
{
    AVLNode *newRoot = leftChild;
    AVLNode *newLeftGrandChild = newRoot->rightChild;
    newRoot->rightChild = oldRoot;
    leftChild = leftGrandChild;

    updateHeight();
    newRoot->updateHeight();
}

template <class T, class K>
void AVLTree<T, K>::AVLNode::LRrotation()
{
    leftChild->RRrotation();
    LLrotation();
}

template <class T, class K>
void AVLTree<T, K>::AVLNode::updateHeight()
{
    height = max(leftChild->height, rightChild->height) + 1;
}

template <class T, class K>
int AVLTree<T, K>::AVLNode::balanceFactor() const
{
    return leftChild->height - rightChild->height;
}

#endif // AVLTREE_H_