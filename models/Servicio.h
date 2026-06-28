#ifndef SERVICIO_H
#define SERVICIO_H

#include <string>
using namespace std;

class Servicio {
private:
    int idServicio;
    int idCliente;
    string instrumento;
    string descripcion;
    string fechaIngreso;
    string fechaEntrega;
    float precio;
    int idEstadoServicio;

public:
    Servicio() {}

    Servicio(int idServicio, int idCliente, string instrumento, string descripcion, string fechaIngreso, string fechaEntrega, float precio, int idEstadoServicio) {
        this->idServicio = idServicio;
        this->idCliente = idCliente;
        this->instrumento = instrumento;
        this->descripcion = descripcion;
        this->fechaIngreso = fechaIngreso;
        this->fechaEntrega = fechaEntrega;
        this->precio = precio;
        this->idEstadoServicio = idEstadoServicio;
    }

    int getIdServicio() { return idServicio; }
    int getIdCliente() { return idCliente; }
    string getInstrumento() { return instrumento; }
    string getDescripcion() { return descripcion; }
    string getFechaIngreso() { return fechaIngreso; }
    string getFechaEntrega() { return fechaEntrega; }
    float getPrecio() { return precio; }
    int getIdEstadoServicio() { return idEstadoServicio; }

    void setIdServicio(int idServicio) { this->idServicio = idServicio; }
    void setIdCliente(int idCliente) { this->idCliente = idCliente; }
    void setInstrumento(string instrumento) { this->instrumento = instrumento; }
    void setDescripcion(string descripcion) { this->descripcion = descripcion; }
    void setFechaIngreso(string fechaIngreso) { this->fechaIngreso = fechaIngreso; }
    void setFechaEntrega(string fechaEntrega) { this->fechaEntrega = fechaEntrega; }
    void setPrecio(float precio) { this->precio = precio; }
    void setIdEstadoServicio(int idEstadoServicio) { this->idEstadoServicio = idEstadoServicio; }
};

#endif