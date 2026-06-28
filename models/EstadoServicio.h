#ifndef ESTADOSERVICIO_H
#define ESTADOSERVICIO_H

#include <string>
using namespace std;

class EstadoServicio {
private:
    int idEstadoServicio;
    string nombre;

public:
    EstadoServicio() {}

    EstadoServicio(int idEstadoServicio, string nombre) {
        this->idEstadoServicio = idEstadoServicio;
        this->nombre = nombre;
    }

    int getIdEstadoServicio() { return idEstadoServicio; }
    string getNombre() { return nombre; }

    void setIdEstadoServicio(int idEstadoServicio) { this->idEstadoServicio = idEstadoServicio; }
    void setNombre(string nombre) { this->nombre = nombre; }
};

#endif