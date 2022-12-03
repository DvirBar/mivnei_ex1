//
// Created by 222ro on 02-Dec-22.
//

#ifndef MIVNEI_EX1_LINKEDLIST_H
#define MIVNEI_EX1_LINKEDLIST_H

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
    LinkedList* getNextNode();



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
 LinkedList<K, T>* LinkedList<K, T>::getNextNode() {
    return nextNode;
}



#endif //MIVNEI_EX1_LINKEDLIST_H
