#ifndef MEDIOPAGO_H
#define MEDIOPAGO_H

#include <string>
using namespace std;

class MedioPago {
private:
    int idMedioPago;
    string nombre;

public:
    MedioPago() {}

    MedioPago(int idMedioPago, string nombre) {
        this->idMedioPago = idMedioPago;
        this->nombre = nombre;
    }

    int getIdMedioPago() { return idMedioPago; }
    string getNombre() { return nombre; }

    void setIdMedioPago(int idMedioPago) { this->idMedioPago = idMedioPago; }
    void setNombre(string nombre) { this->nombre = nombre; }
};

#endif