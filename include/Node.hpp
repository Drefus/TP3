#ifndef NODE_HPP
#define NODE_HPP

#include <array>

template <typename T>
class Node
{
public:
    T value;
    int *indices;
    Node *head;
    Node *left;
    Node *right;
    int qIndices = 0;
    Node();
    Node(T val);
    int height();
    void addIndex(int index);
};

#endif // NODE_HPP