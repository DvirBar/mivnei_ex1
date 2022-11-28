#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <memory>
#include <cmath>
#include <exception>

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

        AVLNode(const K& key, const T& data);

        AVLNode(const AVLNode &) = default;
        AVLNode& operator=(const AVLNode &) = default;

        int getBalanceFactor() const;
        void updateHeight();
        int getHeight(AVLTree::AVLNode *node);

        AVLNode* RRrotation();
        AVLNode* LLrotation();
        AVLNode* RLrotation();
        AVLNode* LRrotation();
        AVLNode* execRotation();
    };

    AVLNode* root;
    void execRemove(AVLNode* node, AVLNode* parent);
    AVLNode* searchByNode(const AVLNode* node, const K& key);
    AVLNode* insertByNode(AVLNode* node, const K& key, const T& data);
    T* removeAux(const K& key, AVLNode* node, AVLNode* parent);
    static void deleteTreeAux(AVLNode* node);
    
public:
    AVLTree();
    AVLTree(const AVLTree &tree) = delete;
    AVLTree& operator=(const AVLTree &tree) = delete;
    ~AVLTree();

    AVLNode* insert(const K& key, const T& data);
    T* remove(const K& key);
    T& search(const K& key) const;
    T& nextInorder(const K& currentKey) const;
    T& prevInorder(const K& currentKey) const;
    int getHeight() const;
    bool isEmpty() const;

    class KeyNotFound {};
    class TreeIsEmpty {};
    class NoNextInorder {};
    class NoPrevInorder {};
};


template <class K, class T>
AVLTree<K, T>::AVLTree():
    root(nullptr)
{}

template <class K, class T>
AVLTree<K, T>::~AVLTree() {
    deleteTreeAux(root);
}

template <class K, class T>
void AVLTree<K, T>::deleteTreeAux(AVLNode *node)
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

template<class K, class T>
AVLTree<K, T>::AVLNode::AVLNode(const K& key, const T& data):
    key(key),
    data(data),
    height(1),
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
    rightChild->LLrotation();
    AVLNode* newParent = RRrotation();

    return newParent;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::AVLNode::LRrotation()
{
    leftChild->RRrotation();
    AVLNode* newParent = LLrotation();
    return newParent;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::searchByNode(const AVLTree::AVLNode* node, const K& key) {
    if (node == nullptr)
        return nullptr;

    if (node->key == key)
        return node;

    if (node->key > key)
        searchByNode(node->leftChild);
    else
        searchByNode(node->rightChild);
}

template <class K, class T>
T& AVLTree<K, T>::search(const K &key) const {
    return searchByNode(root, key)->data;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::insertByNode(AVLTree::AVLNode *node, const K &key, const T &data) {
    if (node == nullptr) {
        try {
            AVLTree::AVLNode *newLeaf = new AVLTree::AVLNode(data, key);
            return newLeaf;
        }
        catch (const bad_alloc &error) {
            throw error;
        }
    }
    if (node->key > key)
        node->leftChild = insertByNode(node->leftChild, key, data);
    else
        node->rightChild = insertByNode(node->rightChild, key, data);

    node->updateHeight();
    return node->execRotation();
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::AVLNode::execRotation()
{
    int nodeBF = getBalanceFactor();
    int nodeLeftChildBF = leftChild->getBalanceFactor();
    int nodeRightChildBF = rightChild->getBalanceFactor();

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
    return root;
};

template <class T, class K>
int AVLTree<T, K>::AVLNode::getHeight(AVLTree::AVLNode *node) {
    if (node == nullptr)
        return -1;
    return node->height;
}

template <class K, class T>
void AVLTree<K, T>::AVLNode::updateHeight() {
    height = max(leftChild->getHeight(), rightChild->getHeight()) + 1;
}

template <class K, class T>
int AVLTree<K, T>::AVLNode::getBalanceFactor() const {
    return leftChild->getHeight() - rightChild->getHeight();
}

template <class K, class T>
T* AVLTree<K, T>::remove(const K &key) {
    if(isEmpty()) {
        throw TreeIsEmpty();
    }
    
    T* node = removeAux(key, root, nullptr);
    if(node == nullptr) {
        throw KeyNotFound();
    }
    
    return node;
}

template <class K, class T>
T* AVLTree<K, T>::removeAux(const K& key, AVLNode* node, AVLNode* parent) {
    T* result = nullptr;
    
    if(node->key == key) {
        // Assumption - T has a copy c'tor
        T data = node->data;
        execRemove(node, parent);
        return &data;
    }
    
    if(node->key > key) {
        if(node->leftChild == nullptr) {
            return nullptr;
        }
        
        result = removeAux(key, node->leftChild, node);
     }
    
    if(node->key < key) {
        if(node->rightChild == nullptr) {
            return nullptr;
        }
        
        result = removeAux(key, node->rightChild, node);
    }
    
    if(result != nullptr) {
        node->execRotation();
    }
    
    return result;
}


template <class K, class T>
void AVLTree<K, T>::execRemove(AVLNode* node, AVLNode *parent) {
    AVLNode* targetNode = node;
    AVLNode* targetParent = parent;
    if (node->leftChild != nullptr && node->rightChild != nullptr) {
        *targetNode = node->rightChild;
        targetParent = node;

        while (targetNode->leftChild != nullptr) {
            targetParent = targetNode;
            targetNode = targetNode->leftChild;
        }
        this->key = targetNode->key;
        this->data = targetNode->data;
    }

    AVLNode *targetChild;
    if (targetNode->leftChild != nullptr) {
        if(parent != nullptr) {
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
    
    else {
        if (targetParent->leftChild == targetChild) {
            targetParent->leftChild = targetNode->rightChild;
        }
        else {
            targetChild->rightChild = targetChild->rightChild;
        }
    }
    

    if (targetNode->height() == 0) {
        delete node;
    }
}

template<class K, class T>
T& AVLTree<K, T>::nextInorder(const K& currentKey) const {
    AVLNode* node = searchByNode(root, currentKey);
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
        }
    }
    
    throw NoNextInorder();
}

template<class K, class T>
T& AVLTree<K, T>::prevInorder(const K& currentKey) const {
    AVLNode* node = searchByNode(root, currentKey);
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
        }
    }
    
    throw NoPrevInorder();
}
#endif // AVLTREE_H_
