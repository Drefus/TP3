#include "Node.hpp"
#include <string>

template <typename T>
Node<T>::Node() : value(T()), head(nullptr), left(nullptr), right(nullptr) {}

template <typename T>
void Node<T>::addIndex(int index)
{
    if (qIndices == 0)
    {
        indices = new int[1];
        indices[0] = index;
        qIndices++;
    }
    else
    {
        int *temp = new int[qIndices + 1];
        for (int i = 0; i < qIndices; i++)
        {
            temp[i] = indices[i];
        }
        temp[qIndices] = index;
        qIndices++;
        delete[] indices;
        indices = temp;
    }
}

template <typename T>
Node<T>::Node(T val) : value(val), head(nullptr), left(nullptr), right(nullptr) {}
template <typename T>
int Node<T>::height()
{
    if (this == nullptr)
    {
        return 0;
    }
    int leftHeight = left->height();
    int rightHeight = right->height();
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

template <typename T>
inline Node<T>::~Node()
{
    delete[] indices;
}

// Explicit instantiation for the types you need
template class Node<int>;
template class Node<float>;
template class Node<double>;
template class Node<std::string>;
template class Node<time_t>;
// Add more types as needed
