#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>
using namespace std;

class Producto {
private:
    int idProducto;
    string nombre;
    string descripcion;
    float precioVenta;
    int stock;
    int stockMinimo;
    int idCategoria;
    int idMarca;
    int idProveedor;

public:
    Producto() {}

    Producto(int idProducto, string nombre, string descripcion, float precioVenta, int stock, int stockMinimo, int idCategoria, int idMarca, int idProveedor) {
        this->idProducto = idProducto;
        this->nombre = nombre;
        this->descripcion = descripcion;
        this->precioVenta = precioVenta;
        this->stock = stock;
        this->stockMinimo = stockMinimo;
        this->idCategoria = idCategoria;
        this->idMarca = idMarca;
        this->idProveedor = idProveedor;
    }

    int getIdProducto() { return idProducto; }
    string getNombre() { return nombre; }
    string getDescripcion() { return descripcion; }
    float getPrecioVenta() { return precioVenta; }
    int getStock() { return stock; }
    int getStockMinimo() { return stockMinimo; }
    int getIdCategoria() { return idCategoria; }
    int getIdMarca() { return idMarca; }
    int getIdProveedor() { return idProveedor; }

    void setIdProducto(int idProducto) { this->idProducto = idProducto; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setDescripcion(string descripcion) { this->descripcion = descripcion; }
    void setPrecioVenta(float precioVenta) { this->precioVenta = precioVenta; }
    void setStock(int stock) { this->stock = stock; }
    void setStockMinimo(int stockMinimo) { this->stockMinimo = stockMinimo; }
    void setIdCategoria(int idCategoria) { this->idCategoria = idCategoria; }
    void setIdMarca(int idMarca) { this->idMarca = idMarca; }
    void setIdProveedor(int idProveedor) { this->idProveedor = idProveedor; }
};

#endif