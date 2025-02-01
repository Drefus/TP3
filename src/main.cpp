#include <iostream>
#include "SistemaPassagem.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const char *filename = argv[1];
    SistemaPassagem sistema;
    sistema.executar(filename);

    return 0;
}