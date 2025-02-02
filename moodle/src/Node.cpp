#include "Node.hpp"
#include <string>

template <typename T>
// Construtor padrão
Node<T>::Node() : valor(T()), cabeca(nullptr), esquerda(nullptr), direita(nullptr)
{
}

template <typename T>
// Função para adicionar um índice ao array de índices
void Node<T>::adicionarIndice(int indice)
{
    if (qntIndices == 0)
    {
        indices = new int[1];
        indices[0] = indice;
        qntIndices++;
    }
    else
    {
        int *temp = new int[qntIndices + 1];
        for (int i = 0; i < qntIndices; i++)
        {
            temp[i] = indices[i];
        }
        temp[qntIndices] = indice;
        qntIndices++;
        delete[] indices;
        indices = temp;
    }
}

template <typename T>
// Construtor com valor inicial
Node<T>::Node(T val) : valor(val), cabeca(nullptr), esquerda(nullptr), direita(nullptr)
{
}

template <typename T>
// Destrutor para liberar memória alocada
inline Node<T>::~Node()
{
    delete[] indices;
}

// Instanciação explícita para os tipos necessários
template class Node<int>;
template class Node<float>;
template class Node<double>;
template class Node<std::string>;
template class Node<time_t>;
// Adicione mais tipos conforme necessário
