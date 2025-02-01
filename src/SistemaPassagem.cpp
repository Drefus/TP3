#include "SistemaPassagem.hpp"
#include "Voo.hpp"     // Ensure Voo class is included
#include "ArvoreB.hpp" // Ensure ArvoreB class is included
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>

// Utility functions
std::string *split(const std::string &str, const std::string &delimiter, int &numTokens)
{
    size_t pos = 0;
    size_t start = 0;
    numTokens = 0;

    while ((pos = str.find(delimiter, start)) != std::string::npos)
    {
        numTokens++;
        start = pos + delimiter.length();
    }
    numTokens++; // For the last token

    std::string *tokens = new std::string[numTokens];
    start = 0;
    int index = 0;

    while ((pos = str.find(delimiter, start)) != std::string::npos)
    {
        tokens[index++] = str.substr(start, pos - start);
        start = pos + delimiter.length();
    }
    tokens[index] = str.substr(start);

    return tokens;
}

time_t convertStringToTimeT(const std::string &dateTimeStr)
{
    struct tm tm;
    int year, month, day, hour, min;
    sscanf(dateTimeStr.c_str(), "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &min);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    return mktime(&tm);
}

std::string convertTimeTToString(time_t dateTime)
{
    struct tm *tm = localtime(&dateTime);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", tm);
    return std::string(buf);
}

// Comparison functions
int comparePrice(Voo a, Voo b)
{
    if (a.getPreco() == b.getPreco())
    {
        return 0;
    }
    return a.getPreco() < b.getPreco() ? -1 : 1;
}

int compareDuration(Voo a, Voo b)
{
    if (a.getDuracao() == b.getDuracao())
    {
        return 0;
    }
    return a.getDuracao() < b.getDuracao() ? -1 : 1;
}

int compareStops(Voo a, Voo b)
{
    if (a.getParadas() == b.getParadas())
    {
        return 0;
    }
    return a.getParadas() < b.getParadas() ? -1 : 1;
}

int (*findComparator(char c))(Voo, Voo)
{
    switch (c)
    {
    case 'p':
        return comparePrice;
    case 'd':
        return compareDuration;
    case 's':
        return compareStops;
    }
    return nullptr;
}

// Sorting function
void stableSort(Voo *arr, int size, int (*comp)(Voo, Voo), int (*tieBreaker1)(Voo, Voo), int (*tieBreaker2)(Voo, Voo))
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (comp(arr[i], arr[j]) > 0)
            {
                Voo temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            else if (comp(arr[i], arr[j]) == 0)
            {
                if (tieBreaker1(arr[i], arr[j]) > 0)
                {
                    Voo temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
                else if (tieBreaker1(arr[i], arr[j]) == 0)
                {
                    if (tieBreaker2(arr[i], arr[j]) > 0)
                    {
                        Voo temp = arr[i];
                        arr[i] = arr[j];
                        arr[j] = temp;
                    }
                }
            }
        }
    }
}

// SistemaPassagem methods
void SistemaPassagem::readData(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file >> numFlights;
    flights = new Voo[numFlights];
    for (int i = 0; i < numFlights; i++)
    {
        std::string origem, destino;
        double preco = 0.0;
        int assentos = 0, paradas = 0;
        time_t partida = 0, chegada = 0;
        std::string partidaStr, chegadaStr;
        file >> origem >> destino >> preco >> assentos >> partidaStr >> chegadaStr >> paradas;
        partida = convertStringToTimeT(partidaStr);
        chegada = convertStringToTimeT(chegadaStr);

        flights[i] = Voo(origem, destino, preco, assentos, partida, chegada, paradas);
        originTree->insert(origem, i);
        destinationTree->insert(destino, i);
        priceTree->insert(preco, i);
        seatsTree->insert(assentos, i);
        departureTree->insert(partida, i);
        arrivalTree->insert(chegada, i);
        stopsTree->insert(paradas, i);
        durationTree->insert(difftime(chegada, partida), i);
    }

    file >> numOperations;
    for (int i = 0; i < numOperations; i++)
    {
        OperationLine line;
        file >> line.numResults >> line.orderCriteria >> line.operation;
        executeOperation(line);
    }

    file.close();
}

void SistemaPassagem::executeOperation(OperationLine line)
{
    std::string clearOp = "";
    for (char c : line.operation)
    {
        if (c != '(' && c != ')')
        {
            clearOp += c;
        }
    }

    int numOp;

    std::string *ops = split(clearOp, "&&", numOp);

    bool *result = processOperations(ops, 0, numOp);

    int countResult = 0;
    for (int i = 0; i < numFlights; i++)
    {
        if (result[i])
        {
            countResult++;
        }
    }
    Voo *resultados = new Voo[countResult];
    int indexResult = 0;
    for (int i = 0; i < numFlights; i++)
    {
        if (result[i])
        {
            resultados[indexResult] = flights[i];
            indexResult++;
        }
    }

    stableSort(resultados, countResult, findComparator(line.orderCriteria[0]), findComparator(line.orderCriteria[1]), findComparator(line.orderCriteria[2]));

    std::cout << line.numResults << " " << line.orderCriteria << " " << line.operation << std::endl;
    for (int i = 0; i < line.numResults; i++)
    {
        std::cout << resultados[i].getOrigem() << " " << resultados[i].getDestino() << " " << resultados[i].getPreco() << " " << resultados[i].getAssentos() << " " << convertTimeTToString(resultados[i].getPartida()) << " " << convertTimeTToString(resultados[i].getChegada()) << " " << resultados[i].getParadas() << std::endl;
    }

    delete[] resultados;
    delete[] result;
    delete[] ops;
}

bool *SistemaPassagem::processOperations(std::string *operations, int index, int maxIndex)
{
    if (index == maxIndex)
    {
        bool *result = new bool[numFlights];
        for (int i = 0; i < numFlights; i++)
        {
            result[i] = true;
        }
        return result;
    }
    std::string op = operations[index];
    std::string type = op.substr(0, 3);
    std::string boolOp = op.substr(3, 2);
    std::string value = op.substr(5, op.size() - 5);
    if (type == "org")
    {
        return combineResults(originTree->searchWithOp(value, boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "dst")
    {
        return combineResults(destinationTree->searchWithOp(value, boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "prc")
    {
        return combineResults(priceTree->searchWithOp(std::stod(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "sea")
    {
        return combineResults(seatsTree->searchWithOp(std::stoi(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "dep")
    {
        return combineResults(departureTree->searchWithOp(std::stoi(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "arr")
    {
        return combineResults(arrivalTree->searchWithOp(std::stoi(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "sto")
    {
        return combineResults(stopsTree->searchWithOp(std::stoi(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }
    else if (type == "dur")
    {
        return combineResults(durationTree->searchWithOp(std::stoi(value), boolOp, numFlights), processOperations(operations, index + 1, maxIndex), numFlights);
    }

    return nullptr;
}

bool *SistemaPassagem::combineResults(bool *result1, bool *result2, int size)
{
    bool *result = new bool[size];
    for (int i = 0; i < size; i++)
    {
        result[i] = result1[i] && result2[i];
    }
    delete[] result1;
    delete[] result2;
    return result;
}

// Constructor and Destructor
SistemaPassagem::SistemaPassagem()
    : originTree(nullptr), destinationTree(nullptr), priceTree(nullptr), seatsTree(nullptr), departureTree(nullptr), arrivalTree(nullptr), stopsTree(nullptr), durationTree(nullptr), flights(nullptr)
{
}

SistemaPassagem::~SistemaPassagem()
{
    delete originTree;
    delete destinationTree;
    delete priceTree;
    delete seatsTree;
    delete departureTree;
    delete arrivalTree;
    delete stopsTree;
    delete durationTree;
    delete[] flights;
}

void SistemaPassagem::run(std::string filename)
{
    originTree = new ArvoreB<std::string>();
    destinationTree = new ArvoreB<std::string>();
    priceTree = new ArvoreB<double>();
    seatsTree = new ArvoreB<int>();
    departureTree = new ArvoreB<time_t>();
    arrivalTree = new ArvoreB<time_t>();
    stopsTree = new ArvoreB<int>();
    durationTree = new ArvoreB<long int>();
    readData(filename);
}
