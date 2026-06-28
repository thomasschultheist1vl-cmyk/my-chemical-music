#ifndef VENTA_H
#define VENTA_H

#include <string>
using namespace std;

class Venta {
private:
    int idVenta;
    int idCliente;
    string fecha;
    float total;
    int idMedioPago;

public:
    Venta() {}

    Venta(int idVenta, int idCliente, string fecha, float total, int idMedioPago) {
        this->idVenta = idVenta;
        this->idCliente = idCliente;
        this->fecha = fecha;
        this->total = total;
        this->idMedioPago = idMedioPago;
    }

    int getIdVenta() { return idVenta; }
    int getIdCliente() { return idCliente; }
    string getFecha() { return fecha; }
    float getTotal() { return total; }
    int getIdMedioPago() { return idMedioPago; }

    void setIdVenta(int idVenta) { this->idVenta = idVenta; }
    void setIdCliente(int idCliente) { this->idCliente = idCliente; }
    void setFecha(string fecha) { this->fecha = fecha; }
    void setTotal(float total) { this->total = total; }
    void setIdMedioPago(int idMedioPago) { this->idMedioPago = idMedioPago; }
};

#endif