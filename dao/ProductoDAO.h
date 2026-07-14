#ifndef PRODUCTODAO_H
#define PRODUCTODAO_H

#include "../models/Producto.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>
#ifdef MCM_QT_APP
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#endif

using namespace std;

class ProductoDAO {
public:
#ifdef MCM_QT_APP
    explicit ProductoDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT p.id_producto, p.nombre, p.descripcion, p.precio_venta, p.stock, p.stock_minimo, c.nombre, m.nombre, pr.nombre FROM productos p LEFT JOIN categorias c ON p.id_categoria=c.id_categoria LEFT JOIN marcas m ON p.id_marca=m.id_marca LEFT JOIN proveedores pr ON p.id_proveedor=pr.id_proveedor ORDER BY p.id_producto"); return q; }
    QSqlQuery buscarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT nombre, descripcion, precio_venta, stock, stock_minimo, id_categoria, id_marca, id_proveedor FROM productos WHERE id_producto = ?"); q.addBindValue(id); q.exec(); return q; }
    QSqlQuery detalleQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT p.id_producto, p.nombre, p.descripcion, p.precio_venta, p.stock, p.stock_minimo, c.nombre, m.nombre, pr.nombre FROM productos p LEFT JOIN categorias c ON p.id_categoria=c.id_categoria LEFT JOIN marcas m ON p.id_marca=m.id_marca LEFT JOIN proveedores pr ON p.id_proveedor=pr.id_proveedor WHERE p.id_producto=?"); q.addBindValue(id); q.exec(); return q; }
    bool agregarQt(Producto &p) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO productos (nombre, descripcion, precio_venta, stock, stock_minimo, id_categoria, id_marca, id_proveedor) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"); q.addBindValue(QString::fromStdString(p.getNombre())); q.addBindValue(QString::fromStdString(p.getDescripcion())); q.addBindValue(p.getPrecioVenta()); q.addBindValue(p.getStock()); q.addBindValue(p.getStockMinimo()); q.addBindValue(p.getIdCategoria()); q.addBindValue(p.getIdMarca()); q.addBindValue(p.getIdProveedor()); return q.exec(); }
    bool modificarQt(Producto &p) const { QSqlQuery q(conexionQt); q.prepare("UPDATE productos SET nombre=?, descripcion=?, precio_venta=?, stock=?, stock_minimo=?, id_categoria=?, id_marca=?, id_proveedor=? WHERE id_producto=?"); q.addBindValue(QString::fromStdString(p.getNombre())); q.addBindValue(QString::fromStdString(p.getDescripcion())); q.addBindValue(p.getPrecioVenta()); q.addBindValue(p.getStock()); q.addBindValue(p.getStockMinimo()); q.addBindValue(p.getIdCategoria()); q.addBindValue(p.getIdMarca()); q.addBindValue(p.getIdProveedor()); q.addBindValue(p.getIdProducto()); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM productos WHERE id_producto=?"); q.addBindValue(id); return q.exec(); }
    QSqlQuery contarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT COUNT(*) FROM productos"); return q; }
    QSqlQuery stockBajoQt(int maximo=3) const { QSqlQuery q(conexionQt); q.prepare("SELECT nombre, stock FROM productos WHERE stock <= ? ORDER BY stock ASC, nombre ASC"); q.addBindValue(maximo); q.exec(); return q; }
    QSqlQuery paraComboQt(bool conStockPrecio=false) const { QSqlQuery q(conexionQt); q.exec(conStockPrecio ? "SELECT id_producto, CONCAT(nombre, ' - Stock: ', stock, ' - $', precio_venta) FROM productos ORDER BY nombre" : "SELECT id_producto, nombre FROM productos ORDER BY nombre"); return q; }
    QSqlQuery precioStockQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT precio_venta, stock FROM productos WHERE id_producto=?"); q.addBindValue(id); q.exec(); return q; }
    bool ajustarStockQt(int id, int cantidad) const { QSqlQuery q(conexionQt); q.prepare("UPDATE productos SET stock=stock+? WHERE id_producto=?"); q.addBindValue(cantidad); q.addBindValue(id); return q.exec(); }
    QSqlQuery ultimosQt(int limite=7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_producto, nombre FROM productos ORDER BY id_producto DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM productos WHERE id_producto = " + to_string(id);

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();

            return cantidad > 0;
        }

        return false;
    }

    void agregar(Producto producto) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO productos "
                "(nombre, descripcion, precio_venta, stock, stock_minimo, id_categoria, id_marca, id_proveedor) VALUES ('" +
                producto.getNombre() + "', '" +
                producto.getDescripcion() + "', " +
                to_string(producto.getPrecioVenta()) + ", " +
                to_string(producto.getStock()) + ", " +
                to_string(producto.getStockMinimo()) + ", " +
                to_string(producto.getIdCategoria()) + ", " +
                to_string(producto.getIdMarca()) + ", " +
                to_string(producto.getIdProveedor()) + ")";

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Producto agregado correctamente." << endl;
            } else {
                cout << "Error al agregar producto." << endl;
                cout << "Verifique que existan la categoria, marca y proveedor indicados." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void listar() {
    ConexionBD conexion;

    if (conexion.conectar()) {
        SQLHDBC hDbc = conexion.getConexion();
        SQLHSTMT hStmt;
        SQLRETURN ret;

        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        string consulta =
            "SELECT p.id_producto, p.nombre, p.descripcion, p.precio_venta, p.stock, p.stock_minimo, "
            "c.nombre, m.nombre, pr.nombre "
            "FROM productos p "
            "INNER JOIN categorias c ON p.id_categoria = c.id_categoria "
            "INNER JOIN marcas m ON p.id_marca = m.id_marca "
            "INNER JOIN proveedores pr ON p.id_proveedor = pr.id_proveedor";

        ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idProducto, stock, stockMinimo;
            double precioVenta;
            char nombre[80];
            char descripcion[200];
            char categoria[60];
            char marca[60];
            char proveedor[80];

            bool hayProductos = false;

            cout << "\n--- LISTA DE PRODUCTOS ---\n";

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                hayProductos = true;

                SQLGetData(hStmt, 1, SQL_C_LONG, &idProducto, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);
                SQLGetData(hStmt, 4, SQL_C_DOUBLE, &precioVenta, 0, NULL);
                SQLGetData(hStmt, 5, SQL_C_LONG, &stock, 0, NULL);
                SQLGetData(hStmt, 6, SQL_C_LONG, &stockMinimo, 0, NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, categoria, sizeof(categoria), NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, marca, sizeof(marca), NULL);
                SQLGetData(hStmt, 9, SQL_C_CHAR, proveedor, sizeof(proveedor), NULL);

                cout << "ID: " << idProducto << endl;
                cout << "Nombre: " << nombre << endl;
                cout << "Descripcion: " << descripcion << endl;
                cout << "Precio venta: $" << precioVenta << endl;
                cout << "Stock: " << stock << endl;
                cout << "Stock minimo: " << stockMinimo << endl;
                cout << "Categoria: " << categoria << endl;
                cout << "Marca: " << marca << endl;
                cout << "Proveedor: " << proveedor << endl;
                cout << "-------------------------\n";
            }

            if (!hayProductos) {
                cout << "No hay productos ingresados." << endl;
            }
        } else {
            cout << "Error al listar productos." << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }
}

    void buscarPorId(int id) {
    ConexionBD conexion;

    if (conexion.conectar()) {
        SQLHDBC hDbc = conexion.getConexion();
        SQLHSTMT hStmt;
        SQLRETURN ret;

        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        string consulta =
            "SELECT p.id_producto, p.nombre, p.descripcion, p.precio_venta, p.stock, p.stock_minimo, "
            "c.nombre, m.nombre, pr.nombre "
            "FROM productos p "
            "INNER JOIN categorias c ON p.id_categoria = c.id_categoria "
            "INNER JOIN marcas m ON p.id_marca = m.id_marca "
            "INNER JOIN proveedores pr ON p.id_proveedor = pr.id_proveedor "
            "WHERE p.id_producto = " + to_string(id);

        ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idProducto, stock, stockMinimo;
            double precioVenta;
            char nombre[80];
            char descripcion[200];
            char categoria[60];
            char marca[60];
            char proveedor[80];

            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &idProducto, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);
                SQLGetData(hStmt, 4, SQL_C_DOUBLE, &precioVenta, 0, NULL);
                SQLGetData(hStmt, 5, SQL_C_LONG, &stock, 0, NULL);
                SQLGetData(hStmt, 6, SQL_C_LONG, &stockMinimo, 0, NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, categoria, sizeof(categoria), NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, marca, sizeof(marca), NULL);
                SQLGetData(hStmt, 9, SQL_C_CHAR, proveedor, sizeof(proveedor), NULL);

                cout << "\n--- PRODUCTO ENCONTRADO ---\n";
                cout << "ID: " << idProducto << endl;
                cout << "Nombre: " << nombre << endl;
                cout << "Descripcion: " << descripcion << endl;
                cout << "Precio venta: $" << precioVenta << endl;
                cout << "Stock: " << stock << endl;
                cout << "Stock minimo: " << stockMinimo << endl;
                cout << "Categoria: " << categoria << endl;
                cout << "Marca: " << marca << endl;
                cout << "Proveedor: " << proveedor << endl;
            } else {
                cout << "No se encontro un producto con ese ID." << endl;
            }
        } else {
            cout << "Error al buscar producto." << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }
}

    void modificar(Producto producto) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE productos SET "
                "nombre = '" + producto.getNombre() + "', "
                "descripcion = '" + producto.getDescripcion() + "', "
                "precio_venta = " + to_string(producto.getPrecioVenta()) + ", "
                "stock = " + to_string(producto.getStock()) + ", "
                "stock_minimo = " + to_string(producto.getStockMinimo()) + ", "
                "id_categoria = " + to_string(producto.getIdCategoria()) + ", "
                "id_marca = " + to_string(producto.getIdMarca()) + ", "
                "id_proveedor = " + to_string(producto.getIdProveedor()) + " "
                "WHERE id_producto = " + to_string(producto.getIdProducto());

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Producto modificado correctamente." << endl;
            } else {
                cout << "Error al modificar producto." << endl;
                cout << "Verifique que existan la categoria, marca y proveedor indicados." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void eliminar(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "DELETE FROM productos WHERE id_producto = " + to_string(id);

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Producto eliminado correctamente." << endl;
            } else {
                cout << "Error al eliminar producto." << endl;
                cout << "Puede que el producto tenga ventas o compras asociadas." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
#ifdef MCM_QT_APP
private:
    QSqlDatabase conexionQt;
#endif
};

#endif
