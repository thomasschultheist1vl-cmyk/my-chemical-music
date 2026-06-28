#ifndef DETALLEVENTA_H
#define DETALLEVENTA_H

class DetalleVenta {
private:
    int idDetalleVenta;
    int idVenta;
    int idProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;

public:
    DetalleVenta() {}

    DetalleVenta(int idDetalleVenta, int idVenta, int idProducto, int cantidad, float precioUnitario, float subtotal) {
        this->idDetalleVenta = idDetalleVenta;
        this->idVenta = idVenta;
        this->idProducto = idProducto;
        this->cantidad = cantidad;
        this->precioUnitario = precioUnitario;
        this->subtotal = subtotal;
    }

    int getIdDetalleVenta() { return idDetalleVenta; }
    int getIdVenta() { return idVenta; }
    int getIdProducto() { return idProducto; }
    int getCantidad() { return cantidad; }
    float getPrecioUnitario() { return precioUnitario; }
    float getSubtotal() { return subtotal; }

    void setIdDetalleVenta(int idDetalleVenta) { this->idDetalleVenta = idDetalleVenta; }
    void setIdVenta(int idVenta) { this->idVenta = idVenta; }
    void setIdProducto(int idProducto) { this->idProducto = idProducto; }
    void setCantidad(int cantidad) { this->cantidad = cantidad; }
    void setPrecioUnitario(float precioUnitario) { this->precioUnitario = precioUnitario; }
    void setSubtotal(float subtotal) { this->subtotal = subtotal; }
};

#endif