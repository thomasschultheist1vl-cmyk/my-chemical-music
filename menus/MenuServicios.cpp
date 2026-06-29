#include "MenuServicios.h"
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

void menuServicios() {
    ServicioDAO servicioDAO;
    ClienteDAO clienteDAO;
    EstadoServicioDAO estadoDAO;

    Servicio servicio;
    int opcion, id;

    do {
        cout << "\n--- GESTION DE SERVICIOS ---\n";
        cout << "1. Registrar servicio\n";
        cout << "2. Listar servicios\n";
        cout << "3. Buscar servicio por ID\n";
        cout << "4. Modificar servicio\n";
        cout << "5. Eliminar servicio\n";
        cout << "6. Cambiar estado del servicio\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int idCliente, idEstadoServicio;
                string instrumento, descripcion, fechaIngreso, fechaEntrega;
                float precio;

                cout << "\nClientes disponibles:\n";
                clienteDAO.listar();

                cout << "\nID cliente: ";
                cin >> idCliente;

                if (!clienteDAO.existe(idCliente)) {
                    cout << "No existe un cliente con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Instrumento: ";
                getline(cin, instrumento);

                cout << "Descripcion: ";
                getline(cin, descripcion);

                cout << "Fecha ingreso (YYYY-MM-DD): ";
                getline(cin, fechaIngreso);

                cout << "Fecha entrega (YYYY-MM-DD): ";
                getline(cin, fechaEntrega);

                cout << "Precio: ";
                cin >> precio;

                cout << "\nEstados de servicio disponibles:\n";
                estadoDAO.listar();

                cout << "\nID estado de servicio: ";
                cin >> idEstadoServicio;

                if (!estadoDAO.existe(idEstadoServicio)) {
                    cout << "No existe un estado de servicio con ese ID." << endl;
                    break;
                }

                servicio.setIdCliente(idCliente);
                servicio.setInstrumento(instrumento);
                servicio.setDescripcion(descripcion);
                servicio.setFechaIngreso(fechaIngreso);
                servicio.setFechaEntrega(fechaEntrega);
                servicio.setPrecio(precio);
                servicio.setIdEstadoServicio(idEstadoServicio);

                servicioDAO.agregar(servicio);
                break;
            }

            case 2:
                servicioDAO.listar();
                break;

            case 3:
                cout << "Ingrese ID del servicio: ";
                cin >> id;
                servicioDAO.buscarPorId(id);
                break;

            case 4: {
                int idCliente, idEstadoServicio;
                string instrumento, descripcion, fechaIngreso, fechaEntrega;
                float precio;

                cout << "Ingrese ID del servicio a modificar: ";
                cin >> id;

                if (!servicioDAO.existe(id)) {
                    cout << "No existe un servicio con ese ID." << endl;
                    break;
                }

                cout << "\nClientes disponibles:\n";
                clienteDAO.listar();

                cout << "\nNuevo ID cliente: ";
                cin >> idCliente;

                if (!clienteDAO.existe(idCliente)) {
                    cout << "No existe un cliente con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo instrumento: ";
                getline(cin, instrumento);

                cout << "Nueva descripcion: ";
                getline(cin, descripcion);

                cout << "Nueva fecha ingreso (YYYY-MM-DD): ";
                getline(cin, fechaIngreso);

                cout << "Nueva fecha entrega (YYYY-MM-DD): ";
                getline(cin, fechaEntrega);

                cout << "Nuevo precio: ";
                cin >> precio;

                cout << "\nEstados de servicio disponibles:\n";
                estadoDAO.listar();

                cout << "\nNuevo ID estado de servicio: ";
                cin >> idEstadoServicio;

                if (!estadoDAO.existe(idEstadoServicio)) {
                    cout << "No existe un estado de servicio con ese ID." << endl;
                    break;
                }

                servicio.setIdServicio(id);
                servicio.setIdCliente(idCliente);
                servicio.setInstrumento(instrumento);
                servicio.setDescripcion(descripcion);
                servicio.setFechaIngreso(fechaIngreso);
                servicio.setFechaEntrega(fechaEntrega);
                servicio.setPrecio(precio);
                servicio.setIdEstadoServicio(idEstadoServicio);

                servicioDAO.modificar(servicio);
                break;
            }

            case 5:
                cout << "Ingrese ID del servicio a eliminar: ";
                cin >> id;

                if (!servicioDAO.existe(id)) {
                    cout << "No existe un servicio con ese ID." << endl;
                    break;
                }

                servicioDAO.eliminar(id);
                break;

            case 6: {
                int idEstadoServicio;

                cout << "Ingrese ID del servicio: ";
                cin >> id;

                if (!servicioDAO.existe(id)) {
                    cout << "No existe un servicio con ese ID." << endl;
                    break;
                }

                cout << "\nEstados de servicio disponibles:\n";
                estadoDAO.listar();

                cout << "\nIngrese ID del nuevo estado: ";
                cin >> idEstadoServicio;

                if (!estadoDAO.existe(idEstadoServicio)) {
                    cout << "No existe un estado de servicio con ese ID." << endl;
                    break;
                }

                servicioDAO.cambiarEstado(id, idEstadoServicio);
                break;
            }

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
