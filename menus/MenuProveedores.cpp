#include "MenuProveedores.h"
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

void menuProveedores() {
    ProveedorDAO dao;
    Proveedor proveedor;
    int opcion, id;

    do {
        cout << "\n--- GESTION DE PROVEEDORES ---\n";
        cout << "1. Agregar proveedor\n";
        cout << "2. Listar proveedores\n";
        cout << "3. Buscar proveedor por ID\n";
        cout << "4. Modificar proveedor\n";
        cout << "5. Eliminar proveedor\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, telefono, email, direccion, cuit;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                cout << "Telefono: ";
                getline(cin, telefono);

                cout << "Email: ";
                getline(cin, email);

                cout << "Direccion: ";
                getline(cin, direccion);

                cout << "CUIT: ";
                getline(cin, cuit);

                proveedor.setNombre(nombre);
                proveedor.setTelefono(telefono);
                proveedor.setEmail(email);
                proveedor.setDireccion(direccion);
                proveedor.setCuit(cuit);

                dao.agregar(proveedor);
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
                string nombre, telefono, email, direccion, cuit;

                cout << "Ingrese ID del proveedor a modificar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un proveedor con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                cout << "Nuevo telefono: ";
                getline(cin, telefono);

                cout << "Nuevo email: ";
                getline(cin, email);

                cout << "Nueva direccion: ";
                getline(cin, direccion);

                cout << "Nuevo CUIT: ";
                getline(cin, cuit);

                proveedor.setIdProveedor(id);
                proveedor.setNombre(nombre);
                proveedor.setTelefono(telefono);
                proveedor.setEmail(email);
                proveedor.setDireccion(direccion);
                proveedor.setCuit(cuit);

                dao.modificar(proveedor);
                break;
            }

            case 5:
                cout << "Ingrese ID del proveedor a eliminar: ";
                cin >> id;

                if (!dao.existe(id)) {
                    cout << "No existe un proveedor con ese ID." << endl;
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
