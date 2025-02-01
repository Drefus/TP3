#include "ArvoreB.hpp"

template <typename T>
void ArvoreB<T>::insert(T value, int index)
{
    insert(root, value, index);
}

template <typename T>
void ArvoreB<T>::insert(Node<T> *&node, T value, int index)
{
    if (node == nullptr)
    {
        node = new Node<T>(value);
        node->addIndex(index);
        tam++;
    }
    else
    {
        Node<T> *temp = search(value);
        if (temp != nullptr)
        {
            temp->addIndex(index);
            return;
        }
        if (value < node->value)
        {
            insert(node->left, value, index);
        }
        else if (value > node->value)
        {
            insert(node->right, value, index);
        }
        balance(node);
        tam++;
    }
}

template <typename T>
void ArvoreB<T>::balance(Node<T> *&node)
{
    if (node->left->height() - node->right->height() > 1)
    {
        if (node->left->left->height() >= node->left->right->height())
        {
            rightRotate(node);
        }
        else
        {
            leftRotate(node->left);
            rightRotate(node);
        }
    }
    else if (node->right->height() - node->left->height() > 1)
    {
        if (node->right->right->height() >= node->right->left->height())
        {
            leftRotate(node);
        }
        else
        {
            rightRotate(node->right);
            leftRotate(node);
        }
    }
}

template <typename T>
void ArvoreB<T>::rightRotate(Node<T> *&node)
{
    Node<T> *temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node = temp;
}

template <typename T>
void ArvoreB<T>::leftRotate(Node<T> *&node)
{
    Node<T> *temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node = temp;
}

template <typename T>
Node<T> *ArvoreB<T>::search(T value)
{
    return search(root, value);
}

template <typename T>
Node<T> *ArvoreB<T>::search(Node<T> *node, T value)
{
    if (node == nullptr || node->value == value)
    {
        return node;
    }
    if (value < node->value)
    {
        return search(node->left, value);
    }
    else
    {
        return search(node->right, value);
    }
}

template <typename T>
bool *ArvoreB<T>::searchWithOp(T value, const std::string &op, int tam)
{
    bool *result = new bool[tam];
    for (int i = 0; i < tam; i++)
    {
        result[i] = false;
    }
    searchWithOpInTree(root, value, op, result, tam);
    return result;
}

template <typename T>
void ArvoreB<T>::searchWithOpInTree(Node<T> *node, T value, const std::string &op, bool *&result, int tam)
{
    if (node == nullptr)
    {
        return;
    }

    if ((op == "==" && node->value == value) ||
        (op == "<=" && node->value <= value) ||
        (op == ">=" && node->value >= value))
    {
        for (int i = 0; i < node->qIndices; i++)
        {
            result[node->indices[i]] = true;
        }

        if (op == "==")
            return;
    }

    searchWithOpInTree(node->left, value, op, result, tam);
    searchWithOpInTree(node->right, value, op, result, tam);
}

// Explicit template instantiation
template class ArvoreB<int>;
template class ArvoreB<float>;
template class ArvoreB<double>;
template class ArvoreB<std::string>;
template class ArvoreB<time_t>;
