#ifndef DETALLECOMPRA_H
#define DETALLECOMPRA_H

class DetalleCompra {
private:
    int idDetalleCompra;
    int idCompra;
    int idProducto;
    int cantidad;
    float precioCompra;
    float subtotal;

public:
    DetalleCompra() {}

    DetalleCompra(int idDetalleCompra, int idCompra, int idProducto, int cantidad, float precioCompra, float subtotal) {
        this->idDetalleCompra = idDetalleCompra;
        this->idCompra = idCompra;
        this->idProducto = idProducto;
        this->cantidad = cantidad;
        this->precioCompra = precioCompra;
        this->subtotal = subtotal;
    }

    int getIdDetalleCompra() { return idDetalleCompra; }
    int getIdCompra() { return idCompra; }
    int getIdProducto() { return idProducto; }
    int getCantidad() { return cantidad; }
    float getPrecioCompra() { return precioCompra; }
    float getSubtotal() { return subtotal; }

    void setIdDetalleCompra(int idDetalleCompra) { this->idDetalleCompra = idDetalleCompra; }
    void setIdCompra(int idCompra) { this->idCompra = idCompra; }
    void setIdProducto(int idProducto) { this->idProducto = idProducto; }
    void setCantidad(int cantidad) { this->cantidad = cantidad; }
    void setPrecioCompra(float precioCompra) { this->precioCompra = precioCompra; }
    void setSubtotal(float subtotal) { this->subtotal = subtotal; }
};

#endif