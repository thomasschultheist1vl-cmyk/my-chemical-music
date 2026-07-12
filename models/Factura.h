#ifndef FACTURA_H
#define FACTURA_H

#include <string>
using namespace std;

class Factura {
private:
    int idFactura;
    int idVenta;
    int idServicio;
    string numeroFactura;
    string tipoFactura;
    string fecha;
    float total;

public:
    Factura() {}

    Factura(int idFactura, int idVenta, int idServicio, string numeroFactura, string tipoFactura, string fecha, float total) {
        this->idFactura = idFactura;
        this->idVenta = idVenta;
        this->idServicio = idServicio;
        this->numeroFactura = numeroFactura;
        this->tipoFactura = tipoFactura;
        this->fecha = fecha;
        this->total = total;
    }

    int getIdFactura() { return idFactura; }
    int getIdVenta() { return idVenta; }
    int getIdServicio() { return idServicio; }
    string getNumeroFactura() { return numeroFactura; }
    string getTipoFactura() { return tipoFactura; }
    string getFecha() { return fecha; }
    float getTotal() { return total; }

    void setIdFactura(int idFactura) { this->idFactura = idFactura; }
    void setIdVenta(int idVenta) { this->idVenta = idVenta; }
    void setIdServicio(int idServicio) { this->idServicio = idServicio; }
    void setNumeroFactura(string numeroFactura) { this->numeroFactura = numeroFactura; }
    void setTipoFactura(string tipoFactura) { this->tipoFactura = tipoFactura; }
    void setFecha(string fecha) { this->fecha = fecha; }
    void setTotal(float total) { this->total = total; }
};

#endif
