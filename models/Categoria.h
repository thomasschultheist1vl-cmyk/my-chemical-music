#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <string>
using namespace std;

class Categoria {
private:
    int idCategoria;
    string nombre;
    string descripcion;

public:
    Categoria() {}

    Categoria(int idCategoria, string nombre, string descripcion) {
        this->idCategoria = idCategoria;
        this->nombre = nombre;
        this->descripcion = descripcion;
    }

    int getIdCategoria() { return idCategoria; }
    string getNombre() { return nombre; }
    string getDescripcion() { return descripcion; }

    void setIdCategoria(int idCategoria) { this->idCategoria = idCategoria; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setDescripcion(string descripcion) { this->descripcion = descripcion; }
};

#endif