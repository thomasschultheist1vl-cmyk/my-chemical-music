#include "MenuProductos.h"
#include <iostream>
#include <string>

#include "../dao/CategoriaDAO.h"
#include "../dao/MarcaDAO.h"
#include "../dao/ProveedorDAO.h"
#include "../dao/ProductoDAO.h"
#include "../dao/ClienteDAO.h"
#include "../dao/VentaDAO.h"
#include "../dao/DetalleVentaDAO.h"
#include "../dao/FacturaDAO.h"
#include "../dao/ServicioDAO.h"
#include "../dao/EstadoServicioDAO.h"
#include "../dao/MedioPagoDAO.h"
#include "../dao/CompraProveedorDAO.h"
#include "../dao/DetalleCompraDAO.h"

using namespace std;

void menuProductos() {
    ProductoDAO dao;
    CategoriaDAO categoriaDAO;
    MarcaDAO marcaDAO;
    ProveedorDAO proveedorDAO;

    Producto producto;
    int opcion, id;

    do {
        cout << "\n--- GESTION DE PRODUCTOS ---\n";
        cout << "1. Agregar producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Buscar producto por ID\n";
        cout << "4. Modificar producto\n";
        cout << "5. Eliminar producto\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, descripcion;
                float precioVenta;
                int stock, stockMinimo, idCategoria, idMarca, idProveedor;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                cout << "Descripcion: ";
                getline(cin, descripcion);

                cout << "Precio de venta: ";
                cin >> precioVenta;

                cout << "Stock: ";
                cin >> stock;

                cout << "Stock minimo: ";
                cin >> stockMinimo;

                cout << "\nCategorias disponibles:\n";
                categoriaDAO.listar();

                cout << "\nID categoria: ";
                cin >> idCategoria;

                cout << "\nMarcas disponibles:\n";
                marcaDAO.listar();

                cout << "\nID marca: ";
                cin >> idMarca;

                cout << "\nProveedores disponibles:\n";
                proveedorDAO.listar();

                cout << "\nID proveedor: ";
                cin >> idProveedor;

                producto.setNombre(nombre);
                producto.setDescripcion(descripcion);
                producto.setPrecioVenta(precioVenta);
                producto.setStock(stock);
                producto.setStockMinimo(stockMinimo);
                producto.setIdCategoria(idCategoria);
                producto.setIdMarca(idMarca);
                producto.setIdProveedor(idProveedor);

                dao.agregar(producto);
                break;
            }

            case 2:
                dao.listar();
                break;

            case 3:
                cout << "Ingrese ID: ";
                cin >> id;
                dao.buscarPorId(id);
                break;

            case 4: {
                string nombre, descripcion;
                float precioVenta;
                int stock, stockMinimo, idCategoria, idMarca, idProveedor;

                cout << "Ingrese ID del producto a modificar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un producto con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                cout << "Nueva descripcion: ";
                getline(cin, descripcion);

                cout << "Nuevo precio de venta: ";
                cin >> precioVenta;

                cout << "Nuevo stock: ";
                cin >> stock;

                cout << "Nuevo stock minimo: ";
                cin >> stockMinimo;

                cout << "\nCategorias disponibles:\n";
                categoriaDAO.listar();

                cout << "\nNuevo ID categoria: ";
                cin >> idCategoria;

                cout << "\nMarcas disponibles:\n";
                marcaDAO.listar();

                cout << "\nNuevo ID marca: ";
                cin >> idMarca;

                cout << "\nProveedores disponibles:\n";
                proveedorDAO.listar();

                cout << "\nNuevo ID proveedor: ";
                cin >> idProveedor;

                producto.setIdProducto(id);
                producto.setNombre(nombre);
                producto.setDescripcion(descripcion);
                producto.setPrecioVenta(precioVenta);
                producto.setStock(stock);
                producto.setStockMinimo(stockMinimo);
                producto.setIdCategoria(idCategoria);
                producto.setIdMarca(idMarca);
                producto.setIdProveedor(idProveedor);

                dao.modificar(producto);
                break;
            }

            case 5:
                cout << "Ingrese ID del producto a eliminar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un producto con ese ID." << endl;
                    break;
                }

                dao.eliminar(id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
