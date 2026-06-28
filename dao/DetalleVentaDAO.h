#ifndef DETALLEVENTADAO_H
#define DETALLEVENTADAO_H

#include "../models/DetalleVenta.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>

using namespace std;

class DetalleVentaDAO {
public:
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM detalle_ventas WHERE id_detalle_venta = " + to_string(id);

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

    float obtenerPrecioProducto(int idProducto) {
        ConexionBD conexion;
        float precio = 0;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT precio_venta FROM productos WHERE id_producto = " + to_string(idProducto);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_FLOAT, &precio, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }

        return precio;
    }

    int obtenerStockProducto(int idProducto) {
        ConexionBD conexion;
        int stock = 0;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT stock FROM productos WHERE id_producto = " + to_string(idProducto);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &stock, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }

        return stock;
    }

    bool hayStock(int idProducto, int cantidad) {
        return obtenerStockProducto(idProducto) >= cantidad;
    }

    void descontarStock(int idProducto, int cantidad) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE productos SET stock = stock - " + to_string(cantidad) +
                " WHERE id_producto = " + to_string(idProducto);

            SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void agregar(DetalleVenta detalle) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO detalle_ventas "
                "(id_venta, id_producto, cantidad, precio_unitario, subtotal) VALUES (" +
                to_string(detalle.getIdVenta()) + ", " +
                to_string(detalle.getIdProducto()) + ", " +
                to_string(detalle.getCantidad()) + ", " +
                to_string(detalle.getPrecioUnitario()) + ", " +
                to_string(detalle.getSubtotal()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Detalle de venta agregado correctamente." << endl;
            } else {
                cout << "Error al agregar detalle de venta." << endl;
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
                "SELECT id_detalle_venta, id_venta, id_producto, cantidad, precio_unitario, subtotal "
                "FROM detalle_ventas";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idDetalleVenta, idVenta, idProducto, cantidad;
                float precioUnitario, subtotal;
                bool hayDetalles = false;

                cout << "\n--- DETALLES DE VENTAS ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayDetalles = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idDetalleVenta, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idProducto, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_LONG, &cantidad, 0, NULL);
                    SQLGetData(hStmt, 5, SQL_C_FLOAT, &precioUnitario, 0, NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &subtotal, 0, NULL);

                    cout << "ID detalle: " << idDetalleVenta << endl;
                    cout << "ID venta: " << idVenta << endl;
                    cout << "ID producto: " << idProducto << endl;
                    cout << "Cantidad: " << cantidad << endl;
                    cout << "Precio unitario: $" << precioUnitario << endl;
                    cout << "Subtotal: $" << subtotal << endl;
                    cout << "-------------------------\n";
                }

                if (!hayDetalles) {
                    cout << "No hay detalles de ventas registrados." << endl;
                }
            } else {
                cout << "Error al listar detalles de ventas." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void listarPorVenta(int idVentaBuscada) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT id_detalle_venta, id_venta, id_producto, cantidad, precio_unitario, subtotal "
                "FROM detalle_ventas WHERE id_venta = " + to_string(idVentaBuscada);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idDetalleVenta, idVenta, idProducto, cantidad;
                float precioUnitario, subtotal;
                bool hayDetalles = false;

                cout << "\n--- DETALLES DE LA VENTA ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayDetalles = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idDetalleVenta, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idProducto, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_LONG, &cantidad, 0, NULL);
                    SQLGetData(hStmt, 5, SQL_C_FLOAT, &precioUnitario, 0, NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &subtotal, 0, NULL);

                    cout << "ID detalle: " << idDetalleVenta << endl;
                    cout << "ID producto: " << idProducto << endl;
                    cout << "Cantidad: " << cantidad << endl;
                    cout << "Precio unitario: $" << precioUnitario << endl;
                    cout << "Subtotal: $" << subtotal << endl;
                    cout << "-------------------------\n";
                }

                if (!hayDetalles) {
                    cout << "No hay detalles para esa venta." << endl;
                }
            } else {
                cout << "Error al listar detalles de la venta." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
};

#endif