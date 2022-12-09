#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdio.h>
#include <iostream>
#include "Pair.h"
#include "Exception.h"
#include <cassert>
#define COUNT 10

using namespace std;

template <class K, class T>
class AVLTree
{
    class AVLNode
    {
    public:
        K key;
        T data;
        AVLNode *leftChild;
        AVLNode *rightChild;
        int height;
        
        AVLNode();
        AVLNode(const K& key, const T& data);

        AVLNode(const AVLNode &) = default;
        AVLNode& operator=(const AVLNode &) = default;

        void updateHeight();
        bool isLeaf();

        AVLNode* RRrotation();
        AVLNode* LLrotation();
        AVLNode* RLrotation();
        AVLNode* LRrotation();
        AVLNode* execRotation();
    };
    
    AVLNode* root;
    int numNodes;
    AVLNode* execRemove(AVLNode* node);
    static const AVLNode* searchByNode(const AVLNode* node, const K& key);
    AVLNode* insertByNode(AVLNode* node, const K& key, const T& data);
    typename AVLTree<K, T>::AVLNode* removeAux(const K& key, AVLNode* node, T* data);
    void buildNearlyComplete(int size);
    void removeLeavesAux(AVLNode* node, AVLNode* parent, int size);
    AVLNode* baseRemove(AVLNode* node);
    AVLNode* removeInorder(AVLNode* node, AVLNode* replacedNode);
    void treeRotate(AVLNode* node);
    void replaceParent(AVLNode* node, AVLNode* parent);
    static void buildNearlyCompleteAux(AVLNode* node, int height);
    static int getNodeBalanceFactor(AVLNode* node);
    static int getNodeHeight(AVLNode* node);
    static void deleteTreeAux(AVLNode* node);
    static void inorderToArrayAUX(const AVLNode* node, Pair<K,T>* array, int* index);
    static void populateFromArrayAux(AVLNode* node, Pair<K, T>* array, int* index);
    static const T& firstInRangeAux(AVLNode *node, T* lastValid, const K &range);

public:
    AVLTree();
    AVLTree(const AVLTree &tree) = delete;
    AVLTree& operator=(const AVLTree &tree) = delete;
    ~AVLTree();

    AVLNode* insert(const K& key, const T& data);
    T remove(const K& key);
    bool isExist(const K& key) const;
    const T& search(const K& key) const;
    const T& nextInorder(const K& currentKey) const;
    const T& prevInorder(const K& currentKey) const;
    void inorderDataToArray(Pair<K,T>* array) const;
    void uniteTrees(const AVLTree& tree);
    int getNumNodes() const;
    const T& findFirstInRange(const K& range) const;
    bool isEmpty() const;
    void printTree() const;
    
    // TODO: implement
    void populateFromArray(Pair<K, T>* array, int size);
    static void mergeArrays(Pair<K, T>* newArr, int newArrSize, Pair<K, T>* arr1,
                            int arr1Size, Pair<K, T>* arr2, int arr2Size);
    static void print2DUtil(AVLNode* node, int space);
    static int maxValue(int value1, int value2);
};

template <class K, class T>
AVLTree<K, T>::AVLTree():
    root(nullptr),
    numNodes(0)
{}

template <class K, class T>
AVLTree<K, T>::~AVLTree() {
    if(!isEmpty()) {
        deleteTreeAux(root);
    }
}

template <class K, class T>
int AVLTree<K, T>::getNumNodes() const {
    return numNodes;
}

template <class K, class T>
void AVLTree<K, T>::deleteTreeAux(AVLNode *node)
{
    if (node->leftChild != nullptr)
    {
        deleteTreeAux(node->leftChild);
    }

    if (node->rightChild != nullptr)
    {
        deleteTreeAux(node->rightChild);
    }

    delete node;
}

template<class K, class T>
AVLTree<K, T>::AVLNode::AVLNode(const K& key, const T& data):
    key(key),
    data(data),
    leftChild(nullptr),
    rightChild(nullptr),
    height(0)
{}

template<class K, class T>
AVLTree<K, T>::AVLNode::AVLNode():
    leftChild(nullptr),
    rightChild(nullptr),
    height(0)
{}
 

template<class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::AVLNode::RRrotation()
{
    AVLNode* newParent = rightChild;
    AVLNode* rightGrandChild = newParent->leftChild;

    newParent->leftChild = this;
    rightChild = rightGrandChild;

    updateHeight();
    newParent->updateHeight();

    return newParent;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::AVLNode::LLrotation()
{
    AVLNode* newParent = leftChild;
    AVLNode* newLeftGrandChild = newParent->rightChild;

    newParent->rightChild = this;
    leftChild = newLeftGrandChild;
    
    updateHeight();
    newParent->updateHeight();
    
    return newParent;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::AVLNode::RLrotation()
{
    this->rightChild = rightChild->LLrotation();
    AVLNode* newParent = RRrotation();
    
    return newParent;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::AVLNode::LRrotation()
{
    this->leftChild = leftChild->RRrotation();
    AVLNode* newParent = LLrotation();
    return newParent;
}

template <class K, class T>
const typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::searchByNode(const AVLTree::AVLNode* node, const K& key) {
    if (node == nullptr)
        throw KeyNotFound();

    if (node->key == key)
        return node;

    if (node->key > key)
        return searchByNode(node->leftChild, key);
    else
        return searchByNode(node->rightChild, key);
}

template <class K, class T>
const T& AVLTree<K, T>::search(const K &key) const {
    return (searchByNode(root, key))->data;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::insertByNode(AVLTree::AVLNode *node,
                                                             const K &key, const T &data) {
    if (node == nullptr) {
        AVLTree::AVLNode *newLeaf = new AVLTree::AVLNode(key, data);
        return newLeaf;
    }
    if (node->key > key)
        node->leftChild = insertByNode(node->leftChild, key, data);
    else
        node->rightChild = insertByNode(node->rightChild, key, data);

    node->updateHeight();
    return node->execRotation();
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::AVLNode::execRotation() {
    int nodeBF = getNodeBalanceFactor(this);

    int nodeLeftChildBF = getNodeBalanceFactor(leftChild);
    int nodeRightChildBF = getNodeBalanceFactor(rightChild);
    
    if ((nodeBF == 2) && (nodeLeftChildBF == -1))
        return LRrotation();

    if ((nodeBF == 2) && (nodeLeftChildBF >= 0))
        return LLrotation();

    if ((nodeBF == -2) && (nodeRightChildBF == 1))
        return RLrotation();

    if ((nodeBF == -2) && (nodeRightChildBF <= 0))
        return RRrotation();

    return this;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::insert(const K &key, const T &data)
{
    root = insertByNode(root, key, data);
    numNodes++;
    return root;
}

template <class T, class K>
int AVLTree<T, K>::getNodeHeight(AVLTree::AVLNode *node) {
    if (node == nullptr)
        return -1;
    return node->height;
}

template <class K, class T>
void AVLTree<K, T>::AVLNode::updateHeight() {
    height = maxValue(getNodeHeight(leftChild), getNodeHeight(rightChild)) + 1;
}

template <class K, class T>
int AVLTree<K, T>::getNodeBalanceFactor(AVLNode* node) {
    if(node == nullptr) {
        return 0;
    }
    
    return getNodeHeight(node->leftChild) - getNodeHeight(node->rightChild);
}

template <class K, class T>
bool AVLTree<K, T>::isEmpty() const {
    return root == nullptr;
}

template <class K, class T>
T AVLTree<K, T>::remove(const K& key) {
    if(isEmpty()) {
        throw KeyNotFound();
    }

    T data;
    root = removeAux(key, root, &data);
    numNodes--;
    return data;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeAux(const K& key, AVLNode* node, T* data) {
    if(node == nullptr) {
        throw KeyNotFound();
    }
    
    if(node->key == key) {
        *data = node->data;
        return execRemove(node);
    }
    
    if(node->key > key) {
        node->leftChild = removeAux(key, node->leftChild, data);
    }
    
    if(node->key < key) {
        node->rightChild = removeAux(key, node->rightChild, data);
    }
    
    return node->execRotation();
}


template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::execRemove(AVLNode* node) {
    if(node->leftChild != nullptr && node->rightChild != nullptr) {
        node->rightChild = removeInorder(node->rightChild, node);
        return node->execRotation();
        
    }
    
    return baseRemove(node);
}


template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeInorder(AVLNode* node, AVLNode* replacedNode) {
    if(node->leftChild == nullptr) {
        replacedNode->key = node->key;
        replacedNode->data = node->data;
        return baseRemove(node);
    }
    
    
    node->leftChild = removeInorder(node->leftChild, replacedNode);
    return node->execRotation();
}

template<class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::baseRemove(AVLNode* node) {
    AVLNode* nodeToReturn;
    if(node->leftChild != nullptr) {
        nodeToReturn = node->leftChild;
    } else {
        nodeToReturn = node->rightChild;
    }
        
    delete node;
    return nodeToReturn;
}




template<class K, class T>
const T& AVLTree<K, T>::nextInorder(const K& currentKey) const {
    const AVLNode* node = searchByNode(root, currentKey);
    AVLNode* next = node->rightChild;
    if(next != nullptr) {
        while(next->leftChild != nullptr) {
            next = next->leftChild;
        }
        
        return next->data;
    }
    
    next = nullptr;
    
    AVLNode* treeRoot = root;
    while(treeRoot != nullptr) {
        if(node->key < treeRoot->key) {
            next = treeRoot;
            treeRoot = treeRoot->leftChild;
        } else if(node->key > treeRoot->key) {
            treeRoot = treeRoot->rightChild;
        } else if(next != nullptr) {
            return next->data;
        } else {
            break;
        }
    }
    
    throw NoNextInorder();
}

template<class K, class T>
const T& AVLTree<K, T>::prevInorder(const K& currentKey) const {
    const AVLNode* node = searchByNode(root, currentKey);
    AVLNode* next = node->leftChild;
    if(next != nullptr) {
        while(next->rightChild != nullptr) {
            next = next->rightChild;
        }
        
        return next->data;
    }
    
    next = nullptr;
    
    AVLNode* treeRoot = root;
    while(treeRoot != nullptr) {
        if(node->key < treeRoot->key) {
            treeRoot = treeRoot->leftChild;
        }  else if(node->key > treeRoot->key) {
            next = treeRoot;
            treeRoot = treeRoot->rightChild;
        } else if(next != nullptr) {
            return next->data;
        } else {
            break;
        }
    }

    throw NoPrevInorder();
}

template<class K, class T>
void AVLTree<K, T>::print2DUtil(AVLNode* root, int space) {
    if (root == nullptr) {
        return;
    }

    space += COUNT;

    print2DUtil(root->rightChild, space);
    cout << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout << root->key;
    print2DUtil(root->leftChild, space);
}

template<class K, class T>
void AVLTree<K, T>::printTree() const {
    print2DUtil(root, 0);
    cout << endl;
    cout << "---------------" << endl;
}

template<class K, class T>
void AVLTree<K, T>::inorderToArrayAUX(const AVLNode* node, Pair<K, T>* array, int* index) {
    if(node != nullptr) {
        inorderToArrayAUX(node->leftChild, array, index);
        array[*index] = Pair<K, T>(node->key, node->data);
        (*index)++;
        inorderToArrayAUX(node->rightChild, array, index);
    }
}

template<class K, class T>
void AVLTree<K, T>::inorderDataToArray(Pair<K,T>* array) const {
    int index = 0;
    inorderToArrayAUX(root, array, &index);
}

template<class K, class T>
bool AVLTree<K, T>::isExist(const K &key) const {
    try {
        search(key);
    }
    catch(const KeyNotFound& keyNotFound) {
        return false;
    }
    return true;
}

template<class K, class T>
void AVLTree<K, T>::mergeArrays(Pair<K, T>* newArr, int newArrSize, Pair<K, T>* arr1,
                                int arr1Size, Pair<K, T>* arr2, int arr2Size) {
    int newArrIndex = 0, arr1Index = 0, arr2Index = 0;
    
    while(arr1Index < arr1Size &&
          arr2Index < arr2Size &&
          newArrIndex < newArrSize) {
        if(arr1[arr1Index].getKey() < arr2[arr2Index].getKey()) {
            newArr[newArrIndex] = arr1[arr1Index];
            arr1Index++;
        } else {
            newArr[newArrIndex] = arr2[arr2Index];
            arr2Index++;
        }
        
        newArrIndex++;
    }
    
    while(arr1Index < arr1Size) {
        newArr[newArrIndex] = arr1[arr1Index];
        arr1Index++;
        newArrIndex++;
    }
    
    while(arr2Index < arr2Size) {
        newArr[newArrIndex] = arr2[arr2Index];
        arr2Index++;
        newArrIndex++;
    }
}

template<class K, class T>
const T& AVLTree<K, T>::firstInRangeAux(AVLNode *node, T* lastValid, const K &range) {
    if(node == nullptr) {
        if(lastValid == nullptr) {
            throw KeyNotFound();
        }

        return *lastValid;
    }
        
    
    if(node->key < range) {
        return firstInRangeAux(node->rightChild, lastValid, range);
    }

    lastValid = &(node->data);
    return firstInRangeAux(node->leftChild, lastValid, range);
}

template<class K, class T>
const T& AVLTree<K, T>::findFirstInRange(const K& range) const {
    return firstInRangeAux(root, nullptr, range);
}

template<class K, class T>
int AVLTree<K, T>::maxValue(int value1, int value2) {
    if(value1 >= value2)
        return value1;
    else
        return value2;
}

template<class K, class T>
void AVLTree<K, T>::populateFromArray(Pair<K, T>* array, int size) {
    buildNearlyComplete(size);
    
    int index = 0;
    
    populateFromArrayAux(root, array, &index);
}

template<class K, class T>
void AVLTree<K, T>::populateFromArrayAux(AVLNode* node, Pair<K, T>* array, int* index) {
    if(node != nullptr) {
        populateFromArrayAux(node->leftChild, array, index);
        node->key = array[*index].getKey();
        node->data = array[*index].getValue();
        (*index)++;
        populateFromArrayAux(node->rightChild, array, index);
    }
}

template<class K, class T>
void AVLTree<K, T>::buildNearlyComplete(int size) {
    if(size <= 0) {
        return;
    }
    assert(this->isEmpty());
    int height = 0;
    int fullTreeSize = 1;
    
    while(fullTreeSize-1<size) {
        height++;
        fullTreeSize*=2;
    }
    
    root = new AVLNode();
    buildNearlyCompleteAux(root, height-1);
    numNodes = fullTreeSize-1;
    removeLeavesAux(root, nullptr, size);
}

template<class K, class T>
void AVLTree<K, T>::removeLeavesAux(AVLNode* node, AVLNode* parent, int size) {
    if(numNodes > size && node->isLeaf()) {
        if(parent->leftChild == node) {
            parent->leftChild = nullptr;
        } else {
            parent->rightChild = nullptr;
        }
        
        delete node;
        
        numNodes--;
        return;
    }
    
    if(size == numNodes) {
        return;
    }
    
    removeLeavesAux(node->rightChild, node, size);
    removeLeavesAux(node->leftChild, node, size);
}

template<class K, class T>
void AVLTree<K, T>::buildNearlyCompleteAux(AVLNode* node, int height) {
    if(height <= 0) {
        node->leftChild = nullptr;
        node->rightChild = nullptr;
        return;
    }
    
    node->leftChild = new AVLNode();
    buildNearlyCompleteAux(node->leftChild, height-1);
    node->rightChild = new AVLNode();
    buildNearlyCompleteAux(node->rightChild, height-1);
}

template<class K, class T>
bool AVLTree<K, T>::AVLNode::isLeaf() {
    return this->leftChild == nullptr && this->rightChild == nullptr;
}

// TODO: Remove before submission
//template<class K, class T>
//void AVLTree<K, T>::debugging_printTree(const std::string& prefix, const AVLTree::AVLNode* node, bool isLeft, std::string& str)
//{
//    if( node != nullptr )
//    {
//        str += prefix;
//
//        str += (isLeft ? "└──" : "├──" );
//
//        // print the value of the node
//        str += std::to_string((*(node->content)).get_id());
//        str += "\n";
//
//        // enter the next tree level - left and right branch
//        AVLTree<K, T>::debugging_printTree( prefix + (isLeft ? "    " : "│   "), node->right, false, str);
//        AVLTree<K, T>::debugging_printTree( prefix + (isLeft ? "    " : "│   "), node->left, true, str);
//    }
//}
//
//template<class K, class T>
//void AVLTree<K, T>::debugging_printTree(const AVLTree::AVLNode* node, std::string& str)
//{
//    debugging_printTree("", node, true, str);
//}
//
//template<class K, class T>
//std::string AVLTree<K, T>::debugging_printTree()
//{
//    std::string tree = "";
//    debugging_printTree(root, tree);
//    return tree;
//}
#endif // AVLTREE_H_

