#include "SistemaPassagem.hpp"
#include "Voo.hpp"     // Certifique-se de que a classe Voo está incluída
#include "ArvoreB.hpp" // Certifique-se de que a classe ArvoreB está incluída
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>

// Função utilitária para dividir uma string em tokens
std::string *dividir(const std::string &str, const std::string &delimitador, int &numTokens)
{
    size_t pos = 0;
    size_t start = 0;
    numTokens = 0;

    while ((pos = str.find(delimitador, start)) != std::string::npos)
    {
        numTokens++;
        start = pos + delimitador.length();
    }
    numTokens++; // Para o último token

    std::string *tokens = new std::string[numTokens];
    start = 0;
    int index = 0;

    while ((pos = str.find(delimitador, start)) != std::string::npos)
    {
        tokens[index++] = str.substr(start, pos - start);
        start = pos + delimitador.length();
    }
    tokens[index] = str.substr(start);

    return tokens;
}

// Função para converter string para time_t
time_t converterStringParaTimeT(const std::string &dateTimeStr)
{
    struct tm tm;
    int ano, mes, dia, hora, min;
    sscanf(dateTimeStr.c_str(), "%d-%d-%dT%d:%d", &ano, &mes, &dia, &hora, &min);
    tm.tm_year = ano - 1900;
    tm.tm_mon = mes - 1;
    tm.tm_mday = dia;
    tm.tm_hour = hora;
    tm.tm_min = min;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    return mktime(&tm);
}

// Função para converter time_t para string
std::string converterTimeTParaString(time_t dateTime)
{
    struct tm *tm = localtime(&dateTime);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", tm);
    return std::string(buf);
}

// Funções de comparação
int compararPreco(Voo a, Voo b)
{
    if (a.getPreco() == b.getPreco())
    {
        return 0;
    }
    return a.getPreco() < b.getPreco() ? -1 : 1;
}

int compararDuracao(Voo a, Voo b)
{
    if (a.getDuracao() == b.getDuracao())
    {
        return 0;
    }
    return a.getDuracao() < b.getDuracao() ? -1 : 1;
}

int compararParadas(Voo a, Voo b)
{
    if (a.getParadas() == b.getParadas())
    {
        return 0;
    }
    return a.getParadas() < b.getParadas() ? -1 : 1;
}

// Função para encontrar o comparador baseado em um caractere
int (*encontrarComparador(char c))(Voo, Voo)
{
    switch (c)
    {
    case 'p':
        return compararPreco;
    case 'd':
        return compararDuracao;
    case 's':
        return compararParadas;
    }
    return nullptr;
}

// Função de ordenação estável
void ordenarEstavel(Voo *arr, int tamanho, int (*comp)(Voo, Voo), int (*desempate1)(Voo, Voo), int (*desempate2)(Voo, Voo))
{
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            if (comp(arr[i], arr[j]) > 0)
            {
                Voo temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
            else if (comp(arr[i], arr[j]) == 0)
            {
                if (desempate1(arr[i], arr[j]) > 0)
                {
                    Voo temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
                else if (desempate1(arr[i], arr[j]) == 0)
                {
                    if (desempate2(arr[i], arr[j]) > 0)
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

// Métodos da classe SistemaPassagem
void SistemaPassagem::lerDados(std::string nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return;
    }

    arquivo >> numVoos;
    voos = new Voo[numVoos];
    for (int i = 0; i < numVoos; i++)
    {
        std::string origem, destino;
        double preco = 0.0;
        int assentos = 0, paradas = 0;
        time_t partida = 0, chegada = 0;
        std::string partidaStr, chegadaStr;
        arquivo >> origem >> destino >> preco >> assentos >> partidaStr >> chegadaStr >> paradas;
        partida = converterStringParaTimeT(partidaStr);
        chegada = converterStringParaTimeT(chegadaStr);
        voos[i] = Voo(origem, destino, preco, assentos, partida, chegada, paradas);
        arvoreOrigem->inserir(origem, i);
        arvoreDestino->inserir(destino, i);
        arvorePreco->inserir(preco, i);
        arvoreAssentos->inserir(assentos, i);
        arvorePartida->inserir(partida, i);
        arvoreChegada->inserir(chegada, i);
        arvoreParadas->inserir(paradas, i);
        arvoreDuracao->inserir(difftime(chegada, partida), i);
    }

    arquivo >> numOperacoes;
    for (int i = 0; i < numOperacoes; i++)
    {
        LinhaOperacao linha;
        arquivo >> linha.numResultados >> linha.criterioOrdenacao >> linha.operacao;
        executarOperacao(linha);
    }

    arquivo.close();
}

// Função para limpar a string de operação
std::string limparStringOperacao(const std::string &operacao)
{
    std::string operacaoLimpa = "";
    for (char c : operacao)
    {
        if (c != '(' && c != ')')
        {
            operacaoLimpa += c;
        }
    }
    return operacaoLimpa;
}

// Função para executar uma operação
void SistemaPassagem::executarOperacao(LinhaOperacao linha)
{
    std::string operacaoLimpa = limparStringOperacao(linha.operacao);

    int numOp;

    std::string *ops = dividir(operacaoLimpa, "&&", numOp);

    bool *resultado = processarOperacoes(ops, 0, numOp);

    int countResultado = 0;
    for (int i = 0; i < numVoos; i++)
    {
        if (resultado[i])
        {
            countResultado++;
        }
    }

    Voo *resultados = new Voo[countResultado];
    int indexResultado = 0;
    for (int i = 0; i < numVoos; i++)
    {
        if (resultado[i])
        {
            resultados[indexResultado] = voos[i];
            indexResultado++;
        }
    }

    ordenarEstavel(resultados, countResultado, encontrarComparador(linha.criterioOrdenacao[0]), encontrarComparador(linha.criterioOrdenacao[1]), encontrarComparador(linha.criterioOrdenacao[2]));

    std::cout << linha.numResultados << " " << linha.criterioOrdenacao << " " << linha.operacao << std::endl;
    for (int i = 0; i < linha.numResultados; i++)
    {
        std::cout << resultados[i].getOrigem() << " " << resultados[i].getDestino() << " " << resultados[i].getPreco() << " " << resultados[i].getAssentos() << " " << converterTimeTParaString(resultados[i].getPartida()) << " " << converterTimeTParaString(resultados[i].getChegada()) << " " << resultados[i].getParadas() << std::endl;
    }

    delete[] resultados;
    delete[] resultado;
    delete[] ops;
}

// Função para processar operações
bool *SistemaPassagem::processarOperacoes(std::string *operacoes, int indice, int maxIndice)
{
    if (indice == maxIndice)
    {
        bool *resultado = new bool[numVoos];
        for (int i = 0; i < numVoos; i++)
        {
            resultado[i] = true;
        }
        return resultado;
    }
    std::string op = operacoes[indice];
    std::string tipo = op.substr(0, 3);
    std::string boolOp = op.substr(3, 2);
    std::string valor = op.substr(5, op.size() - 5);
    if (tipo == "org")
    {
        return combinarResultados(arvoreOrigem->buscarComOperacao(valor, boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "dst")
    {
        return combinarResultados(arvoreDestino->buscarComOperacao(valor, boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "prc")
    {
        return combinarResultados(arvorePreco->buscarComOperacao(std::stod(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "sea")
    {
        return combinarResultados(arvoreAssentos->buscarComOperacao(std::stoi(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "dep")
    {
        return combinarResultados(arvorePartida->buscarComOperacao(std::stoi(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "arr")
    {
        return combinarResultados(arvoreChegada->buscarComOperacao(std::stoi(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "sto")
    {
        return combinarResultados(arvoreParadas->buscarComOperacao(std::stoi(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }
    else if (tipo == "dur")
    {
        return combinarResultados(arvoreDuracao->buscarComOperacao(std::stoi(valor), boolOp, numVoos), processarOperacoes(operacoes, indice + 1, maxIndice), numVoos);
    }

    return nullptr;
}

// Função para combinar resultados de duas operações
bool *SistemaPassagem::combinarResultados(bool *resultado1, bool *resultado2, int tamanho)
{
    bool *resultado = new bool[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        resultado[i] = resultado1[i] && resultado2[i];
    }
    delete[] resultado1;
    delete[] resultado2;
    return resultado;
}

// Construtor e Destrutor
SistemaPassagem::SistemaPassagem()
    : arvoreOrigem(nullptr), arvoreDestino(nullptr), arvorePreco(nullptr), arvoreAssentos(nullptr), arvorePartida(nullptr), arvoreChegada(nullptr), arvoreParadas(nullptr), arvoreDuracao(nullptr), voos(nullptr)
{
}

SistemaPassagem::~SistemaPassagem()
{
    delete arvoreOrigem;
    delete arvoreDestino;
    delete arvorePreco;
    delete arvoreAssentos;
    delete arvorePartida;
    delete arvoreChegada;
    delete arvoreParadas;
    delete arvoreDuracao;
    delete[] voos;
}

// Função para executar o sistema de passagem
void SistemaPassagem::executar(std::string nomeArquivo)
{
    arvoreOrigem = new ArvoreB<std::string>();
    arvoreDestino = new ArvoreB<std::string>();
    arvorePreco = new ArvoreB<double>();
    arvoreAssentos = new ArvoreB<int>();
    arvorePartida = new ArvoreB<time_t>();
    arvoreChegada = new ArvoreB<time_t>();
    arvoreParadas = new ArvoreB<int>();
    arvoreDuracao = new ArvoreB<long int>();
    lerDados(nomeArquivo);
}
