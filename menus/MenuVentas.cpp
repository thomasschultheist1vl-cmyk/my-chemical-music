#include "MenuVentas.h"
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

void menuVentas() {
    VentaDAO ventaDAO;
    DetalleVentaDAO detalleVentaDAO;
    ClienteDAO clienteDAO;
    ProductoDAO productoDAO;
    MedioPagoDAO medioPagoDAO;

    Venta venta;
    DetalleVenta detalle;

    int opcion, id;

    do {
        cout << "\n--- GESTION DE VENTAS ---\n";
        cout << "1. Registrar venta\n";
        cout << "2. Listar ventas\n";
        cout << "3. Buscar venta por ID\n";
        cout << "4. Listar detalles de ventas\n";
        cout << "5. Listar detalles por ID de venta\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int idCliente, idMedioPago;
                string fecha;
                char agregarOtro;
                float totalVenta = 0;

                cout << "\nClientes disponibles:\n";
                clienteDAO.listar();

                cout << "\nID cliente: ";
                cin >> idCliente;

                if (!clienteDAO.existe(idCliente)) {
                    cout << "No existe un cliente con ese ID." << endl;
                    break;
                }

                cout << "\nMedios de pago disponibles:\n";
                medioPagoDAO.listar();

                cout << "\nID medio de pago: ";
                cin >> idMedioPago;

                if (!medioPagoDAO.existe(idMedioPago)) {
                    cout << "No existe un medio de pago con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Fecha (YYYY-MM-DD): ";
                getline(cin, fecha);

                venta.setIdCliente(idCliente);
                venta.setFecha(fecha);
                venta.setTotal(0);
                venta.setIdMedioPago(idMedioPago);

                ventaDAO.agregar(venta);

                int idVenta = ventaDAO.obtenerUltimoId();

                do {
                    int idProducto, cantidad;
                    float precioUnitario, subtotal;

                    cout << "\nProductos disponibles:\n";
                    productoDAO.listar();

                    cout << "\nID producto: ";
                    cin >> idProducto;

                    if (!productoDAO.existe(idProducto)) {
                        cout << "No existe un producto con ese ID." << endl;
                    } else {
                        cout << "Cantidad: ";
                        cin >> cantidad;

                        if (!detalleVentaDAO.hayStock(idProducto, cantidad)) {
                            cout << "No hay stock suficiente para ese producto." << endl;
                        } else {
                            precioUnitario = detalleVentaDAO.obtenerPrecioProducto(idProducto);
                            subtotal = precioUnitario * cantidad;

                            detalle.setIdVenta(idVenta);
                            detalle.setIdProducto(idProducto);
                            detalle.setCantidad(cantidad);
                            detalle.setPrecioUnitario(precioUnitario);
                            detalle.setSubtotal(subtotal);

                            detalleVentaDAO.agregar(detalle);
                            detalleVentaDAO.descontarStock(idProducto, cantidad);

                            totalVenta += subtotal;

                            cout << "Subtotal agregado: $" << subtotal << endl;
                        }
                    }

                    cout << "Desea agregar otro producto a la venta? (s/n): ";
                    cin >> agregarOtro;

                } while (agregarOtro == 's' || agregarOtro == 'S');

                ventaDAO.actualizarTotal(idVenta, totalVenta);

                cout << "\nVenta finalizada." << endl;
                cout << "ID venta: " << idVenta << endl;
                cout << "Total: $" << totalVenta << endl;

                break;
            }

            case 2:
                ventaDAO.listar();
                break;

            case 3:
                cout << "Ingrese ID de la venta: ";
                cin >> id;
                ventaDAO.buscarPorId(id);
                break;

            case 4:
                detalleVentaDAO.listar();
                break;

            case 5:
                cout << "Ingrese ID de la venta: ";
                cin >> id;

                if (!ventaDAO.existe(id)) {
                    cout << "No existe una venta con ese ID." << endl;
                    break;
                }

                detalleVentaDAO.listarPorVenta(id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
