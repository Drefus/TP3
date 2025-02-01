#ifndef SISTEMAPASSAGEM_HPP
#define SISTEMAPASSAGEM_HPP
#include "ArvoreB.hpp"
#include <string>
#include <ctime>
#include "Voo.hpp"

typedef struct linhaOP
{
    int numDeResultados;
    std::string ordenation;
    std::string op;
} linhaOP;

class SistemaPassagem
{
private:
    ArvoreB<std::string> *org; // Origem do vôo
    ArvoreB<std::string> *dst; // Destino do vôo
    ArvoreB<double> *prc;      // Preço de uma passagem
    ArvoreB<int> *sea;         // Número de assentos disponíveis
    ArvoreB<time_t> *dep;      //  Data-hora de partida
    ArvoreB<time_t> *arr;      // Data-hora de chegada
    ArvoreB<int> *sto;         // Número de paradas
    ArvoreB<long int> *dur;    // Duração total do vôo
    int numDeVoos = 0;
    Voo *voos;
    int numDeOps = 0;

    void readData(std::string filename);
    void runOp(linhaOP line);

    bool *processar(std::string *ops, int index, int maxindex);

    bool *AndResults(bool *result1, bool *result2, int tam);

public:
    SistemaPassagem();
    ~SistemaPassagem();
    void run(std::string filename);
};

#endif