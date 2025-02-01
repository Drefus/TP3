#ifndef AVB_HPP
#define AVB_HPP

#include "Node.hpp"
#include <string>

template <typename T>
class ArvoreB
{
private:
    Node<T> *root;
    int tam = 0;

    void insert(Node<T> *&node, T value, int index);
    void balance(Node<T> *&node);

    void rightRotate(Node<T> *&node);

    void leftRotate(Node<T> *&node);

    Node<T> *search(T value);

    Node<T> *search(Node<T> *node, T value);

    void searchWithOpInTree(Node<T> *node, T value, const std::string &op, bool *&result, int tam);

public:
    ArvoreB() : root(nullptr) {}
    ~ArvoreB();
    void deleteNode(Node<T> *&node);
    void insert(T value, int index);
    bool *searchWithOp(T value, const std::string &op, int tam);
};

#endif // AVB_HPP
