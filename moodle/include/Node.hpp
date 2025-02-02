#ifndef NODE_HPP
#define NODE_HPP

template <typename T>
class Node
{
public:
    T valor;            // Valor armazenado no nó
    int *indices;       // Array de índices
    Node *cabeca;       // Ponteiro para o nó cabeça
    Node *esquerda;     // Ponteiro para o nó esquerdo
    Node *direita;      // Ponteiro para o nó direito
    int qntIndices = 0; // Quantidade de índices
    Node();
    Node(T val);
    ~Node();
    void adicionarIndice(int indice); // Adiciona um índice ao array de índices
};

#endif // NODE_HPP
