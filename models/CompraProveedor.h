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
    string estado;
    string motivoAnulacion;
    string fechaAnulacion;

public:
    CompraProveedor() {}

    CompraProveedor(int idCompra, int idProveedor, string fecha, float total) {
        this->idCompra = idCompra;
        this->idProveedor = idProveedor;
        this->fecha = fecha;
        this->total = total;
        this->estado = "Confirmada";
        this->motivoAnulacion = "";
        this->fechaAnulacion = "";
    }

    CompraProveedor(int idCompra, int idProveedor, string fecha, float total, string estado, string motivoAnulacion, string fechaAnulacion) {
        this->idCompra = idCompra;
        this->idProveedor = idProveedor;
        this->fecha = fecha;
        this->total = total;
        this->estado = estado;
        this->motivoAnulacion = motivoAnulacion;
        this->fechaAnulacion = fechaAnulacion;
    }

    int getIdCompra() { return idCompra; }
    int getIdProveedor() { return idProveedor; }
    string getFecha() { return fecha; }
    float getTotal() { return total; }
    string getEstado() { return estado; }
    string getMotivoAnulacion() { return motivoAnulacion; }
    string getFechaAnulacion() { return fechaAnulacion; }

    void setIdCompra(int idCompra) { this->idCompra = idCompra; }
    void setIdProveedor(int idProveedor) { this->idProveedor = idProveedor; }
    void setFecha(string fecha) { this->fecha = fecha; }
    void setTotal(float total) { this->total = total; }
    void setEstado(string estado) { this->estado = estado; }
    void setMotivoAnulacion(string motivoAnulacion) { this->motivoAnulacion = motivoAnulacion; }
    void setFechaAnulacion(string fechaAnulacion) { this->fechaAnulacion = fechaAnulacion; }
};

#endif
