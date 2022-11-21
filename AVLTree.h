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

#endif // AVLTREE_H_