#include "ArvoreB.hpp"

// Função para inserir um valor na árvore
template <typename T>
void ArvoreB<T>::inserir(T valor, int indice)
{
    inserir(raiz, valor, indice);
}

// Função auxiliar para inserir um valor na árvore
template <typename T>
void ArvoreB<T>::inserir(Node<T> *&no, T valor, int indice)
{
    if (no == nullptr)
    {
        no = new Node<T>(valor);
        no->adicionarIndice(indice);
        tamanho++;
    }
    else
    {
        Node<T> *temp = buscar(valor);
        if (temp != nullptr)
        {
            temp->adicionarIndice(indice);
            return;
        }
        if (valor < no->valor)
        {
            inserir(no->esquerda, valor, indice);
        }
        else if (valor > no->valor)
        {
            inserir(no->direita, valor, indice);
        }
        balancear(no);
        tamanho++;
    }
}

// Função para calcular a altura de um nó
template <typename T>
int ArvoreB<T>::altura(Node<T> *no)
{
    if (no == nullptr)
    {
        return 0;
    }
    int alturaEsquerda = altura(no->esquerda);
    int alturaDireita = altura(no->direita);
    return 1 + (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita);
}

// Função para balancear a árvore
template <typename T>
void ArvoreB<T>::balancear(Node<T> *&no)
{
    if (altura(no->esquerda) - altura(no->direita) > 1)
    {
        if (altura(no->esquerda->esquerda) >= altura(no->esquerda->direita))
        {
            rotacaoDireita(no);
        }
        else
        {
            rotacaoEsquerda(no->esquerda);
            rotacaoDireita(no);
        }
    }
    else if (altura(no->direita) - altura(no->esquerda) > 1)
    {
        if (altura(no->direita->direita) >= altura(no->direita->esquerda))
        {
            rotacaoEsquerda(no);
        }
        else
        {
            rotacaoDireita(no->direita);
            rotacaoEsquerda(no);
        }
    }
}

// Função para realizar rotação à direita
template <typename T>
void ArvoreB<T>::rotacaoDireita(Node<T> *&no)
{
    Node<T> *temp = no->esquerda;
    no->esquerda = temp->direita;
    temp->direita = no;
    no = temp;
}

// Função para realizar rotação à esquerda
template <typename T>
void ArvoreB<T>::rotacaoEsquerda(Node<T> *&no)
{
    Node<T> *temp = no->direita;
    no->direita = temp->esquerda;
    temp->esquerda = no;
    no = temp;
}

// Função para buscar um valor na árvore
template <typename T>
Node<T> *ArvoreB<T>::buscar(T valor)
{
    return buscar(raiz, valor);
}

// Função auxiliar para buscar um valor na árvore
template <typename T>
Node<T> *ArvoreB<T>::buscar(Node<T> *no, T valor)
{
    if (no == nullptr || no->valor == valor)
    {
        return no;
    }
    if (valor < no->valor)
    {
        return buscar(no->esquerda, valor);
    }
    else
    {
        return buscar(no->direita, valor);
    }
}

// Função para buscar valores com uma operação específica
template <typename T>
bool *ArvoreB<T>::buscarComOperacao(T valor, const std::string &op, int tamanho)
{
    bool *resultado = new bool[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        resultado[i] = false;
    }
    buscarComOperacaoNaArvore(raiz, valor, op, resultado, tamanho);
    return resultado;
}

// Função auxiliar para buscar valores com uma operação específica na árvore
template <typename T>
void ArvoreB<T>::buscarComOperacaoNaArvore(Node<T> *no, T valor, const std::string &op, bool *&resultado, int tamanho)
{
    if (no == nullptr)
    {
        return;
    }

    if ((op == "==" && no->valor == valor) ||
        (op == "<=" && no->valor <= valor) ||
        (op == ">=" && no->valor >= valor))
    {
        for (int i = 0; i < no->qntIndices; i++)
        {
            resultado[no->indices[i]] = true;
        }

        if (op == "==")
            return;
    }

    buscarComOperacaoNaArvore(no->esquerda, valor, op, resultado, tamanho);
    buscarComOperacaoNaArvore(no->direita, valor, op, resultado, tamanho);
}

// Função para deletar um nó da árvore
template <typename T>
void ArvoreB<T>::deletarNo(Node<T> *&no)
{
    if (no == nullptr)
    {
        return;
    }
    deletarNo(no->esquerda);
    deletarNo(no->direita);
    delete no;
}

// Destrutor da árvore
template <typename T>
inline ArvoreB<T>::~ArvoreB()
{
    deletarNo(raiz);
}

// Instanciação explícita de templates
template class ArvoreB<int>;
template class ArvoreB<float>;
template class ArvoreB<double>;
template class ArvoreB<std::string>;
template class ArvoreB<time_t>;
