#ifndef SISTEMAPASSAGEM_HPP
#define SISTEMAPASSAGEM_HPP

#include "ArvoreB.hpp"
#include <string>
#include <ctime>
#include "Voo.hpp"

// Estrutura para armazenar uma linha de operação
typedef struct LinhaOperacao
{
    int numResultados;             // Número de resultados esperados
    std::string criterioOrdenacao; // Critério de ordenação
    std::string operacao;          // Operação a ser executada
} LinhaOperacao;

class SistemaPassagem
{
private:
    ArvoreB<std::string> *arvoreOrigem;  // Árvore para armazenar as origens dos voos
    ArvoreB<std::string> *arvoreDestino; // Árvore para armazenar os destinos dos voos
    ArvoreB<double> *arvorePreco;        // Árvore para armazenar os preços das passagens
    ArvoreB<int> *arvoreAssentos;        // Árvore para armazenar o número de assentos disponíveis
    ArvoreB<time_t> *arvorePartida;      // Árvore para armazenar as datas e horas de partida
    ArvoreB<time_t> *arvoreChegada;      // Árvore para armazenar as datas e horas de chegada
    ArvoreB<int> *arvoreParadas;         // Árvore para armazenar o número de paradas
    ArvoreB<long int> *arvoreDuracao;    // Árvore para armazenar a duração total dos voos
    int numVoos = 0;                     // Número de voos
    Voo *voos;                           // Array de voos
    int numOperacoes = 0;                // Número de operações

    void lerDados(std::string nomeArquivo);     // Função para ler os dados do arquivo
    void executarOperacao(LinhaOperacao linha); // Função para executar uma operação

    bool *processarOperacoes(std::string *operacoes, int indice, int maxIndice); // Função para processar operações

    bool *combinarResultados(bool *resultado1, bool *resultado2, int tamanho); // Função para combinar resultados de duas operações

public:
    SistemaPassagem();                      // Construtor
    ~SistemaPassagem();                     // Destrutor
    void executar(std::string nomeArquivo); // Função para executar o sistema de passagem
};

#endif // SISTEMAPASSAGEM_HPP