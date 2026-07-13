#ifndef DETALLECOMPRADAO_H
#define DETALLECOMPRADAO_H

#include "../models/DetalleCompra.h"
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

class DetalleCompraDAO {
public:
#ifdef MCM_QT_APP
    explicit DetalleCompraDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    bool agregarQt(DetalleCompra &d) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO detalle_compras (id_compra, id_producto, cantidad, precio_compra, subtotal) VALUES (?, ?, ?, ?, ?)"); q.addBindValue(d.getIdCompra()); q.addBindValue(d.getIdProducto()); q.addBindValue(d.getCantidad()); q.addBindValue(d.getPrecioCompra()); q.addBindValue(d.getSubtotal()); return q.exec(); }
    QSqlQuery listarPorCompraQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT p.nombre, dc.cantidad, dc.precio_compra, dc.subtotal FROM detalle_compras dc INNER JOIN productos p ON dc.id_producto=p.id_producto WHERE dc.id_compra=? ORDER BY dc.id_detalle_compra"); q.addBindValue(id); q.exec(); return q; }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM detalle_compras WHERE id_detalle_compra = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();

            return cantidad > 0;
        }

        return false;
    }

    void aumentarStock(int idProducto, int cantidad) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE productos SET stock = stock + " + to_string(cantidad) +
                " WHERE id_producto = " + to_string(idProducto);

            SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void agregar(DetalleCompra detalle) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO detalle_compras "
                "(id_compra, id_producto, cantidad, precio_compra, subtotal) VALUES (" +
                to_string(detalle.getIdCompra()) + ", " +
                to_string(detalle.getIdProducto()) + ", " +
                to_string(detalle.getCantidad()) + ", " +
                to_string(detalle.getPrecioCompra()) + ", " +
                to_string(detalle.getSubtotal()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Detalle de compra agregado correctamente." << endl;
            } else {
                cout << "Error al agregar detalle de compra." << endl;
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

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT id_detalle_compra, id_compra, id_producto, cantidad, precio_compra, subtotal "
                "FROM detalle_compras";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idDetalleCompra, idCompra, idProducto, cantidad;
                float precioCompra, subtotal;
                bool hayDetalles = false;

                cout << "\n--- DETALLES DE COMPRAS ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayDetalles = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idDetalleCompra, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idCompra, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idProducto, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_LONG, &cantidad, 0, NULL);
                    SQLGetData(hStmt, 5, SQL_C_FLOAT, &precioCompra, 0, NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &subtotal, 0, NULL);

                    cout << "ID detalle: " << idDetalleCompra << endl;
                    cout << "ID compra: " << idCompra << endl;
                    cout << "ID producto: " << idProducto << endl;
                    cout << "Cantidad: " << cantidad << endl;
                    cout << "Precio compra: $" << precioCompra << endl;
                    cout << "Subtotal: $" << subtotal << endl;
                    cout << "-------------------------\n";
                }

                if (!hayDetalles) {
                    cout << "No hay detalles de compras registrados." << endl;
                }
            } else {
                cout << "Error al listar detalles de compras." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void listarPorCompra(int idCompraBuscada) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT id_detalle_compra, id_compra, id_producto, cantidad, precio_compra, subtotal "
                "FROM detalle_compras WHERE id_compra = " + to_string(idCompraBuscada);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idDetalleCompra, idCompra, idProducto, cantidad;
                float precioCompra, subtotal;
                bool hayDetalles = false;

                cout << "\n--- DETALLES DE LA COMPRA ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayDetalles = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idDetalleCompra, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idCompra, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idProducto, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_LONG, &cantidad, 0, NULL);
                    SQLGetData(hStmt, 5, SQL_C_FLOAT, &precioCompra, 0, NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &subtotal, 0, NULL);

                    cout << "ID detalle: " << idDetalleCompra << endl;
                    cout << "ID producto: " << idProducto << endl;
                    cout << "Cantidad: " << cantidad << endl;
                    cout << "Precio compra: $" << precioCompra << endl;
                    cout << "Subtotal: $" << subtotal << endl;
                    cout << "-------------------------\n";
                }

                if (!hayDetalles) {
                    cout << "No hay detalles para esa compra." << endl;
                }
            } else {
                cout << "Error al listar detalles de la compra." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
#ifdef MCM_QT_APP
private: QSqlDatabase conexionQt;
#endif
};

#endif
