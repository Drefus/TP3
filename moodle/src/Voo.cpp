#include "Voo.hpp"

Voo::Voo(std::string origem, std::string destino, double preco, int assentos, time_t partida, time_t chegada, int paradas)
{
    this->origem = origem;
    this->destino = destino;
    this->preco = preco;
    this->assentos = assentos;
    this->partida = partida;
    this->chegada = chegada;
    this->paradas = paradas;
    this->duracao = difftime(chegada, partida);
}

Voo::Voo()
{
}

Voo::~Voo()
{
}

std::string Voo::getOrigem() const
{
    return origem;
}

std::string Voo::getDestino() const
{
    return destino;
}

double Voo::getPreco() const
{
    return preco;
}

int Voo::getAssentos() const
{
    return assentos;
}

time_t Voo::getPartida() const
{
    return partida;
}

time_t Voo::getChegada() const
{
    return chegada;
}

int Voo::getParadas() const
{
    return paradas;
}

int Voo::getDuracao() const
{
    return duracao;
}
