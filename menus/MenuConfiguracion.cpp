#include "MenuConfiguracion.h"
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

void menuConfiguracion() {
    CategoriaDAO categoriaDAO;
    MarcaDAO marcaDAO;
    MedioPagoDAO medioPagoDAO;
    EstadoServicioDAO estadoDAO;

    Categoria categoria;
    Marca marca;
    MedioPago medioPago;
    EstadoServicio estadoServicio;

    int opcion, id;

    do {
        cout << "\n--- CONFIGURACION GENERAL ---\n";
        cout << "1. Agregar categoria\n";
        cout << "2. Listar categorias\n";
        cout << "3. Buscar categoria por ID\n";
        cout << "4. Modificar categoria\n";
        cout << "5. Eliminar categoria\n";
        cout << "6. Agregar marca\n";
        cout << "7. Listar marcas\n";
        cout << "8. Buscar marca por ID\n";
        cout << "9. Modificar marca\n";
        cout << "10. Eliminar marca\n";
        cout << "11. Agregar medio de pago\n";
        cout << "12. Listar medios de pago\n";
        cout << "13. Buscar medio de pago por ID\n";
        cout << "14. Modificar medio de pago\n";
        cout << "15. Eliminar medio de pago\n";
        cout << "16. Agregar estado de servicio\n";
        cout << "17. Listar estados de servicio\n";
        cout << "18. Buscar estado de servicio por ID\n";
        cout << "19. Modificar estado de servicio\n";
        cout << "20. Eliminar estado de servicio\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, descripcion;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                cout << "Descripcion: ";
                getline(cin, descripcion);

                categoria.setNombre(nombre);
                categoria.setDescripcion(descripcion);

                categoriaDAO.agregar(categoria);
                break;
            }

            case 2:
                categoriaDAO.listar();
                break;

            case 3:
                cout << "Ingrese ID de la categoria: ";
                cin >> id;
                categoriaDAO.buscarPorId(id);
                break;

            case 4: {
                string nombre, descripcion;

                cout << "Ingrese ID de la categoria a modificar: ";
                cin >> id;

                if (!categoriaDAO.existe(id)) {
                    cout << "No existe una categoria con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                cout << "Nueva descripcion: ";
                getline(cin, descripcion);

                categoria.setIdCategoria(id);
                categoria.setNombre(nombre);
                categoria.setDescripcion(descripcion);

                categoriaDAO.modificar(categoria);
                break;
            }

            case 5:
                cout << "Ingrese ID de la categoria a eliminar: ";
                cin >> id;

                if (!categoriaDAO.existe(id)) {
                    cout << "No existe una categoria con ese ID." << endl;
                    break;
                }

                categoriaDAO.eliminar(id);
                break;

            case 6: {
                string nombre;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                marca.setNombre(nombre);

                marcaDAO.agregar(marca);
                break;
            }

            case 7:
                marcaDAO.listar();
                break;

            case 8:
                cout << "Ingrese ID de la marca: ";
                cin >> id;
                marcaDAO.buscarPorId(id);
                break;

            case 9: {
                string nombre;

                cout << "Ingrese ID de la marca a modificar: ";
                cin >> id;

                if (!marcaDAO.existe(id)) {
                    cout << "No existe una marca con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                marca.setIdMarca(id);
                marca.setNombre(nombre);

                marcaDAO.modificar(marca);
                break;
            }

            case 10:
                cout << "Ingrese ID de la marca a eliminar: ";
                cin >> id;

                if (!marcaDAO.existe(id)) {
                    cout << "No existe una marca con ese ID." << endl;
                    break;
                }

                marcaDAO.eliminar(id);
                break;

            case 11: {
                string nombre;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                medioPago.setNombre(nombre);

                medioPagoDAO.agregar(medioPago);
                break;
            }

            case 12:
                medioPagoDAO.listar();
                break;

            case 13:
                cout << "Ingrese ID del medio de pago: ";
                cin >> id;
                medioPagoDAO.buscarPorId(id);
                break;

            case 14: {
                string nombre;

                cout << "Ingrese ID del medio de pago a modificar: ";
                cin >> id;

                if (!medioPagoDAO.existe(id)) {
                    cout << "No existe un medio de pago con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                medioPago.setIdMedioPago(id);
                medioPago.setNombre(nombre);

                medioPagoDAO.modificar(medioPago);
                break;
            }

            case 15:
                cout << "Ingrese ID del medio de pago a eliminar: ";
                cin >> id;

                if (!medioPagoDAO.existe(id)) {
                    cout << "No existe un medio de pago con ese ID." << endl;
                    break;
                }

                medioPagoDAO.eliminar(id);
                break;

            case 16: {
                string nombre;

                cin.ignore();

                cout << "Nombre: ";
                getline(cin, nombre);

                estadoServicio.setNombre(nombre);

                estadoDAO.agregar(estadoServicio);
                break;
            }

            case 17:
                estadoDAO.listar();
                break;

            case 18:
                cout << "Ingrese ID del estado de servicio: ";
                cin >> id;
                estadoDAO.buscarPorId(id);
                break;

            case 19: {
                string nombre;

                cout << "Ingrese ID del estado de servicio a modificar: ";
                cin >> id;

                if (!estadoDAO.existe(id)) {
                    cout << "No existe un estado de servicio con ese ID." << endl;
                    break;
                }

                cin.ignore();

                cout << "Nuevo nombre: ";
                getline(cin, nombre);

                estadoServicio.setIdEstadoServicio(id);
                estadoServicio.setNombre(nombre);

                estadoDAO.modificar(estadoServicio);
                break;
            }

            case 20:
                cout << "Ingrese ID del estado de servicio a eliminar: ";
                cin >> id;

                if (!estadoDAO.existe(id)) {
                    cout << "No existe un estado de servicio con ese ID." << endl;
                    break;
                }

                estadoDAO.eliminar(id);
                break;

            case 0:
                break;

            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}
