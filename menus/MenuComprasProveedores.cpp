#include "MenuComprasProveedores.h"
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

void menuComprasProveedores() {
    CompraProveedorDAO compraDAO;
    DetalleCompraDAO detalleDAO;
    ProveedorDAO proveedorDAO;
    ProductoDAO productoDAO;

    CompraProveedor compra;
    DetalleCompra detalle;

    int opcion, id;

    do {
        cout << "\n--- COMPRAS A PROVEEDORES ---\n";
        cout << "1. Registrar compra\n";
        cout << "2. Listar compras\n";
        cout << "3. Buscar compra por ID\n";
        cout << "4. Listar detalles de compras\n";
        cout << "5. Listar detalles por ID de compra\n";
        cout << "6. Eliminar compra\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int idProveedor;
                string fecha;
                char agregarOtro;
                float totalCompra = 0;

                cout << "\nProveedores disponibles:\n";
                proveedorDAO.listar();

                cout << "\nID proveedor: ";
                cin >> idProveedor;

                if (!proveedorDAO.existe(idProveedor)) {
                    cout << "No existe un proveedor con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Fecha (YYYY-MM-DD): ";
                getline(cin, fecha);

                compra.setIdProveedor(idProveedor);
                compra.setFecha(fecha);
                compra.setTotal(0);

                compraDAO.agregar(compra);

                int idCompra = compraDAO.obtenerUltimoId();

                do {
                    int idProducto, cantidad;
                    float precioCompra, subtotal;

                    cout << "\nProductos disponibles:\n";
                    productoDAO.listar();

                    cout << "\nID producto: ";
                    cin >> idProducto;

                    if (!productoDAO.existe(idProducto)) {
                        cout << "No existe un producto con ese ID." << endl;
                    } else {
                        cout << "Cantidad comprada: ";
                        cin >> cantidad;

                        cout << "Precio de compra unitario: ";
                        cin >> precioCompra;

                        subtotal = precioCompra * cantidad;

                        detalle.setIdCompra(idCompra);
                        detalle.setIdProducto(idProducto);
                        detalle.setCantidad(cantidad);
                        detalle.setPrecioCompra(precioCompra);
                        detalle.setSubtotal(subtotal);

                        detalleDAO.agregar(detalle);
                        detalleDAO.aumentarStock(idProducto, cantidad);

                        totalCompra += subtotal;

                        cout << "Subtotal agregado: $" << subtotal << endl;
                    }

                    cout << "Desea agregar otro producto a la compra? (s/n): ";
                    cin >> agregarOtro;

                } while (agregarOtro == 's' || agregarOtro == 'S');

                compraDAO.actualizarTotal(idCompra, totalCompra);

                cout << "\nCompra finalizada." << endl;
                cout << "ID compra: " << idCompra << endl;
                cout << "Total: $" << totalCompra << endl;

                break;
            }

            case 2:
                compraDAO.listar();
                break;

            case 3:
                cout << "Ingrese ID de la compra: ";
                cin >> id;
                compraDAO.buscarPorId(id);
                break;

            case 4:
                detalleDAO.listar();
                break;

            case 5:
                cout << "Ingrese ID de la compra: ";
                cin >> id;

                if (!compraDAO.existe(id)) {
                    cout << "No existe una compra con ese ID." << endl;
                    break;
                }

                detalleDAO.listarPorCompra(id);
                break;

            case 6:
                cout << "Ingrese ID de la compra a eliminar: ";
                cin >> id;

                if (!compraDAO.existe(id)) {
                    cout << "No existe una compra con ese ID." << endl;
                    break;
                }

                compraDAO.eliminar(id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
