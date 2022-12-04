#ifndef MIVNEI_EX1_LINKEDLIST_H
#define MIVNEI_EX1_LINKEDLIST_H

//template<class K, class T>
//struct LinkedNode {
//    K key;
//    T data;
//    LinkedNode* nextNode;
//    LinkedNode* prevNode;
//};

template<class K, class T>
class LinkedList
{
private:
        K key;
        T data;
        LinkedList* nextNode;
        LinkedList* prevNode;

public:
    LinkedList(const K& key, const T& data);
    LinkedList(const LinkedList& node);
    LinkedList& operator=(const LinkedList& node);
    ~LinkedList();
    void insert(const K& key, const T& data);
    void remove(const K& key);
    LinkedList* getNextNode() const;
    const K& getKey() const;
    const T& getData() const;



};

template<class K, class T>
LinkedList<K, T>::LinkedList(const K& key, const T& data) :
key(key),
data(data),
nextNode(nullptr),
prevNode(nullptr)
{}

template<class K, class T>
LinkedList<K, T>::LinkedList(const LinkedList& node) {
    LinkedList<K, T>* currentNode = node.nextNode;
    while(currentNode != nullptr) {
        this->insert(currentNode->key,currentNode->data);
        currentNode = currentNode->nextNode;
    }
}

template<class K, class T>
 LinkedList<K, T>* LinkedList<K, T>::getNextNode() const {
    return nextNode;
}

template<class K, class T>
const K& LinkedList<K, T>::getKey() const {
    return key;
}

template<class K, class T>
const T& LinkedList<K, T>::getData() const {
    return data;
}



#endif //MIVNEI_EX1_LINKEDLIST_H
