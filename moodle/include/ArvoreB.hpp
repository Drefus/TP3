#ifndef ARVOREB_HPP
#define ARVOREB_HPP

#include "Node.hpp"
#include <string>

template <typename T>
class ArvoreB
{
private:
    Node<T> *raiz;   // raiz da árvore
    int tamanho = 0; // tamanho da árvore

    void inserir(Node<T> *&no, T valor, int indice);
    void balancear(Node<T> *&no);

    void rotacaoDireita(Node<T> *&no);

    void rotacaoEsquerda(Node<T> *&no);

    Node<T> *buscar(T valor);

    Node<T> *buscar(Node<T> *no, T valor);

    void buscarComOperacaoNaArvore(Node<T> *no, T valor, const std::string &op, bool *&resultado, int tamanho);

    int altura(Node<T> *no);

    void deletarNo(Node<T> *&no);

public:
    ArvoreB() : raiz(nullptr) {}
    ~ArvoreB();
    void inserir(T valor, int indice);
    bool *buscarComOperacao(T valor, const std::string &op, int tamanho);
};

#endif // ARVOREB_HPP
