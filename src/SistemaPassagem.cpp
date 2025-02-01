#include "SistemaPassagem.hpp"
#include "Voo.hpp"     // Ensure Voo class is included
#include "ArvoreB.hpp" // Ensure ArvoreB class is included
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>

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

void SistemaPassagem::readData(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file >> numDeVoos;
    voos = new Voo[numDeVoos];
    for (int i = 0; i < numDeVoos; i++)
    {
        std::string origem, destino;
        double preco = 0.0;
        int assentos = 0, paradas = 0;
        time_t partida = 0, chegada = 0;
        std::string partidaStr, chegadaStr;
        file >> origem >> destino >> preco >> assentos >> partidaStr >> chegadaStr >> paradas;
        partida = convertStringToTimeT(partidaStr);
        chegada = convertStringToTimeT(chegadaStr);

        voos[i] = Voo(origem, destino, preco, assentos, partida, chegada, paradas);
        org->insert(origem, i);
        dst->insert(destino, i);
        prc->insert(preco, i);
        sea->insert(assentos, i);
        dep->insert(partida, i);
        arr->insert(chegada, i);
        sto->insert(paradas, i);
        dur->insert(difftime(chegada, partida), i);
    }

    file >> numDeOps;
    for (int i = 0; i < numDeOps; i++)
    {
        linhaOP line;
        file >> line.numDeResultados >> line.ordenation >> line.op;
        runOp(line);
    }

    file.close();
}

bool comparePreco(Voo a, Voo b)
{
    return a.getPreco() < b.getPreco();
}
bool compareDuracao(Voo a, Voo b)
{
    return a.getDuracao() < b.getDuracao();
}
bool compareAssentos(Voo a, Voo b)
{
    return a.getAssentos() < b.getAssentos();
}

void stableSort(Voo *arr, int tam, bool (*comp)(Voo, Voo))
{
    for (int i = 0; i < tam - 1; i++)
    {
        for (int j = 0; j < tam - i - 1; j++)
        {
            if (comp(arr[j + 1], arr[j]))
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void SistemaPassagem::runOp(linhaOP line)
{

    std::string clearOp = "";
    for (char c : line.op)
    {
        if (c != '(' && c != ')')
        {
            clearOp += c;
        }
    }

    int numOp;

    std::string *ops = split(clearOp, "&&", numOp);

    bool *result = processar(ops, 0, numOp);

    int countResult = 0;
    for (int i = 0; i < numDeVoos; i++)
    {
        if (result[i])
        {
            countResult++;
        }
    }
    Voo *resultados = new Voo[countResult];
    int indexResult = 0;
    for (int i = 0; i < numDeVoos; i++)
    {
        if (result[i])
        {
            resultados[indexResult] = voos[i];
            indexResult++;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        switch (line.ordenation[0])
        {
        case 'p':
            stableSort(resultados, countResult, comparePreco);
            break;
        case 'd':
            stableSort(resultados, countResult, compareDuracao);
            break;
        case 's':
            stableSort(resultados, countResult, compareAssentos);
            break;
        }
    }
    std::cout << line.numDeResultados << " " << line.ordenation << " " << line.op << std::endl;
    for (int i = 0; i < line.numDeResultados; i++)
    {
        std::cout << resultados[i].getOrigem() << " " << resultados[i].getDestino() << " " << resultados[i].getPreco() << " " << resultados[i].getAssentos() << " " << convertTimeTToString(resultados[i].getPartida()) << " " << convertTimeTToString(resultados[i].getChegada()) << " " << resultados[i].getParadas() << std::endl;
    }
}

bool *SistemaPassagem::processar(std::string *ops, int index, int maxindex)
{
    if (index == maxindex)
    {
        bool *result = new bool[numDeVoos];
        for (int i = 0; i < numDeVoos; i++)
        {
            result[i] = true;
        }
        return result;
    }
    std::string op = ops[index];
    std::string type = op.substr(0, 3);
    std::string boolOp = op.substr(3, 2);
    std::string value = op.substr(5, op.size() - 5);
    if (type == "org")
    {
        return AndResults(org->searchWithOp(value, boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "dst")
    {
        return AndResults(dst->searchWithOp(value, boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "prc")
    {
        return AndResults(prc->searchWithOp(std::stod(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "sea")
    {
        return AndResults(sea->searchWithOp(std::stoi(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "dep")
    {
        return AndResults(dep->searchWithOp(std::stoi(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "arr")
    {
        return AndResults(arr->searchWithOp(std::stoi(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "sto")
    {
        return AndResults(sto->searchWithOp(std::stoi(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }
    else if (type == "dur")
    {
        return AndResults(dur->searchWithOp(std::stoi(value), boolOp, numDeVoos), processar(ops, index + 1, maxindex), numDeVoos);
    }

    return nullptr;
}

bool *SistemaPassagem::AndResults(bool *result1, bool *result2, int tam)
{
    bool *result = new bool[tam];
    for (int i = 0; i < tam; i++)
    {
        result[i] = result1[i] && result2[i];
    }
    return result;
}

SistemaPassagem::SistemaPassagem()
{
}

SistemaPassagem::~SistemaPassagem()
{
}

void SistemaPassagem::run(std::string filename)
{
    org = new ArvoreB<std::string>();
    dst = new ArvoreB<std::string>();
    prc = new ArvoreB<double>();
    sea = new ArvoreB<int>();
    dep = new ArvoreB<time_t>();
    arr = new ArvoreB<time_t>();
    sto = new ArvoreB<int>();
    dur = new ArvoreB<long int>();
    readData(filename);
}
