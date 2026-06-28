#ifndef MARCA_H
#define MARCA_H

#include <string>
using namespace std;

class Marca {
private:
    int idMarca;
    string nombre;

public:
    Marca() {}

    Marca(int idMarca, string nombre) {
        this->idMarca = idMarca;
        this->nombre = nombre;
    }

    int getIdMarca() { return idMarca; }
    string getNombre() { return nombre; }

    void setIdMarca(int idMarca) { this->idMarca = idMarca; }
    void setNombre(string nombre) { this->nombre = nombre; }
};

#endif