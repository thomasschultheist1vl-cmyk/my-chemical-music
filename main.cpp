#include <iostream>
#include <string>

#include "dao/CategoriaDAO.h"
#include "dao/MarcaDAO.h"
#include "dao/ProveedorDAO.h"
#include "dao/ProductoDAO.h"
#include "dao/ClienteDAO.h"
#include "dao/VentaDAO.h"
#include "dao/DetalleVentaDAO.h"
#include "dao/FacturaDAO.h"
#include "dao/ServicioDAO.h"
#include "dao/EstadoServicioDAO.h"
#include "dao/MedioPagoDAO.h"
#include "dao/CompraProveedorDAO.h"
#include "dao/DetalleCompraDAO.h"

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

void menuFacturacion() {
    FacturaDAO facturaDAO;
    VentaDAO ventaDAO;

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
                int idVenta;
                string numeroFactura, tipoFactura, fecha;
                float total;

                cout << "\nVentas disponibles:\n";
                ventaDAO.listar();

                cout << "\nIngrese ID de la venta a facturar: ";
                cin >> idVenta;

                if (!ventaDAO.existe(idVenta)) {
                    cout << "No existe una venta con ese ID." << endl;
                    break;
                }

                total = ventaDAO.obtenerTotal(idVenta);

                cin.ignore();

                cout << "Numero de factura: ";
                getline(cin, numeroFactura);

                cout << "Tipo de factura: ";
                getline(cin, tipoFactura);

                cout << "Fecha (YYYY-MM-DD): ";
                getline(cin, fecha);

                factura.setIdVenta(idVenta);
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

int main() {
    int opcion;

    do {
        cout << "\n====================================\n";
        cout << "        MY CHEMICAL MUSIC\n";
        cout << "====================================\n";
        cout << "1. Gestionar productos\n";
        cout << "2. Gestionar clientes\n";
        cout << "3. Gestionar proveedores\n";
        cout << "4. Gestionar ventas\n";
        cout << "5. Gestionar servicios\n";
        cout << "6. Gestionar facturacion\n";
        cout << "7. Compras a proveedores\n";
        cout << "8. Configuracion general\n";
        cout << "0. Salir\n";
        cout << "====================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: menuProductos(); break;
            case 2: menuClientes(); break;
            case 3: menuProveedores(); break;
            case 4: menuVentas(); break;
            case 5: menuServicios(); break;
            case 6: menuFacturacion(); break;
            case 7: menuComprasProveedores(); break;
            case 8: menuConfiguracion(); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}