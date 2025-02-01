#ifndef SISTEMAPASSAGEM_HPP
#define SISTEMAPASSAGEM_HPP
#include "ArvoreB.hpp"
#include <string>
#include <ctime>
#include "Voo.hpp"

typedef struct OperationLine
{
    int numResults;
    std::string orderCriteria;
    std::string operation;
} OperationLine;

class SistemaPassagem
{
private:
    ArvoreB<std::string> *originTree;      // Origem do vôo
    ArvoreB<std::string> *destinationTree; // Destino do vôo
    ArvoreB<double> *priceTree;            // Preço de uma passagem
    ArvoreB<int> *seatsTree;               // Número de assentos disponíveis
    ArvoreB<time_t> *departureTree;        //  Data-hora de partida
    ArvoreB<time_t> *arrivalTree;          // Data-hora de chegada
    ArvoreB<int> *stopsTree;               // Número de paradas
    ArvoreB<long int> *durationTree;       // Duração total do vôo
    int numFlights = 0;
    Voo *flights;
    int numOperations = 0;

    void readData(std::string filename);
    void executeOperation(OperationLine line);

    bool *processOperations(std::string *operations, int index, int maxIndex);

    bool *combineResults(bool *result1, bool *result2, int size);

public:
    SistemaPassagem();
    ~SistemaPassagem();
    void run(std::string filename);
};

#endif