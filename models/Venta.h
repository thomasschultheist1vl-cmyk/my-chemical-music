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
    string estado;
    string motivoAnulacion;
    string fechaAnulacion;

public:
    Venta() {}

    Venta(int idVenta, int idCliente, string fecha, float total, int idMedioPago) {
        this->idVenta = idVenta;
        this->idCliente = idCliente;
        this->fecha = fecha;
        this->total = total;
        this->idMedioPago = idMedioPago;
        this->estado = "Confirmada";
        this->motivoAnulacion = "";
        this->fechaAnulacion = "";
    }

    Venta(int idVenta, int idCliente, string fecha, float total, int idMedioPago, string estado, string motivoAnulacion, string fechaAnulacion) {
        this->idVenta = idVenta;
        this->idCliente = idCliente;
        this->fecha = fecha;
        this->total = total;
        this->idMedioPago = idMedioPago;
        this->estado = estado;
        this->motivoAnulacion = motivoAnulacion;
        this->fechaAnulacion = fechaAnulacion;
    }

    int getIdVenta() { return idVenta; }
    int getIdCliente() { return idCliente; }
    string getFecha() { return fecha; }
    float getTotal() { return total; }
    int getIdMedioPago() { return idMedioPago; }
    string getEstado() { return estado; }
    string getMotivoAnulacion() { return motivoAnulacion; }
    string getFechaAnulacion() { return fechaAnulacion; }

    void setIdVenta(int idVenta) { this->idVenta = idVenta; }
    void setIdCliente(int idCliente) { this->idCliente = idCliente; }
    void setFecha(string fecha) { this->fecha = fecha; }
    void setTotal(float total) { this->total = total; }
    void setIdMedioPago(int idMedioPago) { this->idMedioPago = idMedioPago; }
    void setEstado(string estado) { this->estado = estado; }
    void setMotivoAnulacion(string motivoAnulacion) { this->motivoAnulacion = motivoAnulacion; }
    void setFechaAnulacion(string fechaAnulacion) { this->fechaAnulacion = fechaAnulacion; }
};

#endif
