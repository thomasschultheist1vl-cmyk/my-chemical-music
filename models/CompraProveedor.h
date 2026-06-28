#ifndef COMPRAPROVEEDOR_H
#define COMPRAPROVEEDOR_H

#include <string>
using namespace std;

class CompraProveedor {
private:
    int idCompra;
    int idProveedor;
    string fecha;
    float total;

public:
    CompraProveedor() {}

    CompraProveedor(int idCompra, int idProveedor, string fecha, float total) {
        this->idCompra = idCompra;
        this->idProveedor = idProveedor;
        this->fecha = fecha;
        this->total = total;
    }

    int getIdCompra() { return idCompra; }
    int getIdProveedor() { return idProveedor; }
    string getFecha() { return fecha; }
    float getTotal() { return total; }

    void setIdCompra(int idCompra) { this->idCompra = idCompra; }
    void setIdProveedor(int idProveedor) { this->idProveedor = idProveedor; }
    void setFecha(string fecha) { this->fecha = fecha; }
    void setTotal(float total) { this->total = total; }
};

#endif