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
        AVLNode *rightChild;
        AVLNode *leftChild;
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
    void execRemove(AVLNode* node, AVLNode* parent);
    static const AVLNode* searchByNode(const AVLNode* node, const K& key);
    AVLNode* insertByNode(AVLNode* node, const K& key, const T& data);
    T removeAux(const K& key, AVLNode* node, AVLNode* parent);
    void buildNearlyComplete(int size);
    void removeLeavesAux(AVLNode* node, int size);
    static void buildNearlyCompleteAux(AVLNode* node, int height);
    static int getNodeBalanceFactor(AVLNode* node);
    static int getNodeHeight(AVLNode* node);
    static void deleteTreeAux(AVLNode* node);
    static void inorderToArrayAUX(const AVLNode* node, Pair<K,T>* array);
    static void populateFromArrayAux(AVLNode* node, Pair<K, T>* array);
    static const T& firstInRangeAux(AVLNode* node, const K& range);
    
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
    deleteTreeAux(root);
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
    height(0),
    leftChild(nullptr),
    rightChild(nullptr)
{}

template<class K, class T>
AVLTree<K, T>::AVLNode::AVLNode():
    height(0),
    leftChild(nullptr),
    rightChild(nullptr)
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
};

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
T AVLTree<K, T>::remove(const K& key) {
    if(isEmpty()) {
        throw TreeIsEmpty();
    }

    T data = removeAux(key, root, nullptr);
    numNodes--;
    return data;
}

template <class K, class T>
bool AVLTree<K, T>::isEmpty() const {
    return root == nullptr;
}

template <class K, class T>
T AVLTree<K, T>::removeAux(const K& key, AVLNode* node, AVLNode* parent) {
    T result = root->data;
    if(node->key == key) {
        // TODO: Assumption - T has a copy c'tor
        T data = node->data;
        execRemove(node, parent);
        return data;
    }
    
    if(node->key > key) {
        if(node->leftChild == nullptr) {
            throw KeyNotFound();
        }
        
        result = removeAux(key, node->leftChild, node);
     }
    
    if(node->key < key) {
        if(node->rightChild == nullptr) {
            throw KeyNotFound();
        }
        
        result = removeAux(key, node->rightChild, node);
    }
    
    AVLNode* newParent = node->execRotation();
    if(node == root) {
        root = newParent;
    } else {
        if(parent->leftChild == node) {
            parent->leftChild = newParent;
        } else {
            parent->rightChild = newParent;
        }
        
    }
    
    return result;
}


template <class K, class T>
void AVLTree<K, T>::execRemove(AVLNode* node, AVLNode *parent) {
    AVLNode* targetNode = node;
    AVLNode* targetParent = parent;
    if (node->leftChild != nullptr && node->rightChild != nullptr) {
        targetNode = node->rightChild;
        targetParent = node;

        while (targetNode->leftChild != nullptr) {
            targetParent = targetNode;
            targetNode = targetNode->leftChild;
        }
        node->key = targetNode->key;
        node->data = targetNode->data;
    }
    
    if (targetNode->leftChild != nullptr) {
        if(targetParent != nullptr) {
            if (targetParent->leftChild == targetNode) {
                targetParent->leftChild = targetNode->leftChild;
            }
            else {
                targetParent->rightChild = targetNode->leftChild;
            }
        } else {
            root = targetNode->leftChild;
        }
        
    }
    
    else if(targetNode->rightChild != nullptr) {
        if(targetParent != nullptr) {
            if (targetParent->leftChild == targetNode) {
                targetParent->leftChild = targetNode->rightChild;
            }
            else {
                targetParent->rightChild = targetNode->rightChild;
            }
        } else {
            root = targetNode->rightChild;
        }
    }

    if (getNodeHeight(targetNode) == 0) {
        if(targetParent != nullptr) {
            if(targetParent->leftChild == targetNode) {
                targetParent->leftChild = nullptr;
            }
            
            if(targetParent->rightChild == targetNode) {
                targetParent->rightChild = nullptr;
            }
        } else {
            root = nullptr;
        }
    }
    
    delete targetNode;
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
void AVLTree<K, T>::uniteTrees(const AVLTree<K, T>& tree) {
    
}

template<class K, class T>
void AVLTree<K, T>::print2DUtil(AVLNode* root, int space) {
    if (root == nullptr) {
//        cout << "Empty tree" <<  endl;
        return;
    }
        
    space += COUNT;

    print2DUtil(root->rightChild, space);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);
    print2DUtil(root->leftChild, space);
}

template<class K, class T>
void AVLTree<K, T>::printTree() const {
    print2DUtil(root, 0);
    cout << "---------------" << endl;
}

template<class K, class T>
void AVLTree<K, T>::inorderToArrayAUX(const AVLNode* node, Pair<K, T>* array) {
    if(node != nullptr) {
        inorderToArrayAUX(node->leftChild, array);
        *array = Pair<K, T>(node->key, node->data);
        array++;
        inorderToArrayAUX(node->rightChild, array);
    }
}

template<class K, class T>
void AVLTree<K, T>::inorderDataToArray(Pair<K,T>* array) const {
    inorderToArrayAUX(root, array);
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
const T& AVLTree<K, T>::firstInRangeAux(AVLNode *node, const K &range) {
    if(node == nullptr)
        throw KeyNotFound();

    if(node->key < range)
        return firstInRangeAux(node->rightChild, range);

    if(node->key >= range && node->leftChild == nullptr)
        return node->data;

    if(node->key >= range && node->leftChild->key >= range)
        return firstInRangeAux(node->leftChild, range);
    else
        return node->data;
}

template<class K, class T>
const T& AVLTree<K, T>::findFirstInRange(const K& range) const {
    return firstInRangeAux(root, range);
}

template<class K, class T>
int AVLTree<K, T>::maxValue(int value1, int value2) {
    if(value1 >= value2)
        return value1;
    else
        return value2;
//    return value1 >= value2 ? value1 : value2;
}

template<class K, class T>
void AVLTree<K, T>::populateFromArray(Pair<K, T>* array, int size) {
    buildNearlyComplete(size);
    populateFromArrayAux(root, array);
}

template<class K, class T>
void AVLTree<K, T>::populateFromArrayAux(AVLNode* node, Pair<K, T>* array) {
    if(node != nullptr) {
        populateFromArrayAux(node->leftChild, array);
        node->key = array->getKey();
        node->data = array->getValue();
        array++;
        populateFromArrayAux(node->rightChild, array);
    }
}

template<class K, class T>
void AVLTree<K, T>::buildNearlyComplete(int size) {
    assert(this->isEmpty());
    int height = 1;
    for(int i=1; i<size; i*=2) {
        height++;
    }
    
    root = new AVLNode();
    buildNearlyCompleteAux(root, height-1);
    removeLeavesAux(root, size);
}

template<class K, class T>
void AVLTree<K, T>::removeLeavesAux(AVLNode* node, int size) {
    if(node->isLeaf()) {
        delete node;
        numNodes--;
        return;
    }
    
    if(size == numNodes) {
        return;
    }
    
    removeLeavesAux(node->rightChild, size);
    removeLeavesAux(node->leftChild, size);
}

template<class K, class T>
void AVLTree<K, T>::buildNearlyCompleteAux(AVLNode* node, int height) {
    if(height == 0) {
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
#endif // AVLTREE_H_

