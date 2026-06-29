#include "MenuClientes.h"
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

void menuClientes() {
    ClienteDAO dao;
    Cliente cliente;
    int opcion, id;

    do {
        cout << "\n--- GESTION DE CLIENTES ---\n";
        cout << "1. Agregar cliente\n";
        cout << "2. Listar clientes\n";
        cout << "3. Buscar cliente por ID\n";
        cout << "4. Modificar cliente\n";
        cout << "5. Eliminar cliente\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, apellido, telefono, email, direccion, dni;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                cout << "Apellido: ";
                getline(cin, apellido);

                cout << "Telefono: ";
                getline(cin, telefono);

                cout << "Email: ";
                getline(cin, email);

                cout << "Direccion: ";
                getline(cin, direccion);

                cout << "DNI: ";
                getline(cin, dni);

                cliente.setNombre(nombre);
                cliente.setApellido(apellido);
                cliente.setTelefono(telefono);
                cliente.setEmail(email);
                cliente.setDireccion(direccion);
                cliente.setDni(dni);

                dao.agregar(cliente);
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
                string nombre, apellido, telefono, email, direccion, dni;

                cout << "Ingrese ID del cliente a modificar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un cliente con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                cout << "Nuevo apellido: ";
                getline(cin, apellido);

                cout << "Nuevo telefono: ";
                getline(cin, telefono);

                cout << "Nuevo email: ";
                getline(cin, email);

                cout << "Nueva direccion: ";
                getline(cin, direccion);

                cout << "Nuevo DNI: ";
                getline(cin, dni);

                cliente.setIdCliente(id);
                cliente.setNombre(nombre);
                cliente.setApellido(apellido);
                cliente.setTelefono(telefono);
                cliente.setEmail(email);
                cliente.setDireccion(direccion);
                cliente.setDni(dni);

                dao.modificar(cliente);
                break;
            }

            case 5: {
                cout << "Ingrese ID del cliente a eliminar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un cliente con ese ID." << endl;
                    break;
                }

                dao.eliminar(id);
                break;
            }

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
