#ifndef VOO_HPP
#define VOO_HPP
#include <string>
#include <ctime>

class Voo
{
private:
    std::string origem;
    std::string destino;
    double preco;
    int assentos;
    time_t partida;
    time_t chegada;
    int paradas;
    int duracao;

public:
    Voo(std::string origem, std::string destino, double preco, int assentos, time_t partida, time_t chegada, int paradas);
    Voo();
    ~Voo();

    // Getters
    std::string getOrigem() const;
    std::string getDestino() const;
    double getPreco() const;
    int getAssentos() const;
    time_t getPartida() const;
    time_t getChegada() const;
    int getParadas() const;
    int getDuracao() const;
};
;

#endif // VOO_HPP