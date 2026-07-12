#include "MenuFacturacion.h"
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

void menuFacturacion() {
    FacturaDAO facturaDAO;
    VentaDAO ventaDAO;
    ServicioDAO servicioDAO;

    Factura factura;
    int opcion, id;

    do {
        cout << "\n--- GESTION DE FACTURACION ---\n";
        cout << "1. Generar factura\n";
        cout << "2. Listar facturas\n";
        cout << "3. Buscar factura por ID\n";
        cout << "4. Eliminar factura\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int tipoOrigen, idVenta = 0, idServicio = 0;
                string numeroFactura, tipoFactura, fecha;
                float total;

                cout << "\nQue queres facturar?\n";
                cout << "1. Venta\n";
                cout << "2. Servicio\n";
                cout << "Opcion: ";
                cin >> tipoOrigen;

                if (tipoOrigen == 1) {
                    cout << "\nVentas disponibles:\n";
                    ventaDAO.listar();

                    cout << "\nIngrese ID de la venta a facturar: ";
                    cin >> idVenta;

                    if (!ventaDAO.existe(idVenta)) {
                        cout << "No existe una venta con ese ID." << endl;
                        break;
                    }

                    total = ventaDAO.obtenerTotal(idVenta);
                } else if (tipoOrigen == 2) {
                    cout << "\nServicios disponibles:\n";
                    servicioDAO.listar();

                    cout << "\nIngrese ID del servicio a facturar: ";
                    cin >> idServicio;

                    if (!servicioDAO.existe(idServicio)) {
                        cout << "No existe un servicio con ese ID." << endl;
                        break;
                    }

                    total = servicioDAO.obtenerPrecio(idServicio);
                } else {
                    cout << "Opcion invalida." << endl;
                    break;
                }

                cin.ignore();

                cout << "Numero de factura: ";
                getline(cin, numeroFactura);

                cout << "Tipo de factura: ";
                getline(cin, tipoFactura);

                cout << "Fecha (YYYY-MM-DD): ";
                getline(cin, fecha);

                factura.setIdVenta(idVenta);
                factura.setIdServicio(idServicio);
                factura.setNumeroFactura(numeroFactura);
                factura.setTipoFactura(tipoFactura);
                factura.setFecha(fecha);
                factura.setTotal(total);

                facturaDAO.agregar(factura);
                break;
            }

            case 2:
                facturaDAO.listar();
                break;

            case 3:
                cout << "Ingrese ID de la factura: ";
                cin >> id;
                facturaDAO.buscarPorId(id);
                break;

            case 4:
                cout << "Ingrese ID de la factura a eliminar: ";
                cin >> id;

                if (!facturaDAO.existe(id)) {
                    cout << "No existe una factura con ese ID." << endl;
                    break;
                }

                facturaDAO.eliminar(id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
