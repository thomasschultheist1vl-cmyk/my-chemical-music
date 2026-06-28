#ifndef FACTURADAO_H
#define FACTURADAO_H

#include "../models/Factura.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>

using namespace std;

class FacturaDAO {
public:
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "SELECT COUNT(*) FROM facturas WHERE id_factura = " + to_string(id);
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

    void agregar(Factura factura) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO facturas (id_venta, numero_factura, tipo_factura, fecha, total) VALUES (" +
                to_string(factura.getIdVenta()) + ", '" +
                factura.getNumeroFactura() + "', '" +
                factura.getTipoFactura() + "', '" +
                factura.getFecha() + "', " +
                to_string(factura.getTotal()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Factura generada correctamente." << endl;
            } else {
                cout << "Error al generar factura." << endl;
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
                "SELECT id_factura, id_venta, numero_factura, tipo_factura, fecha, total FROM facturas";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idFactura, idVenta;
                char numeroFactura[30];
                char tipoFactura[10];
                char fecha[20];
                float total;
                bool hayFacturas = false;

                cout << "\n--- LISTA DE FACTURAS ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayFacturas = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &total, 0, NULL);

                    cout << "ID factura: " << idFactura << endl;
                    cout << "ID venta: " << idVenta << endl;
                    cout << "Numero: " << numeroFactura << endl;
                    cout << "Tipo: " << tipoFactura << endl;
                    cout << "Fecha: " << fecha << endl;
                    cout << "Total: $" << total << endl;
                    cout << "-------------------------\n";
                }

                if (!hayFacturas) {
                    cout << "No hay facturas registradas." << endl;
                }
            } else {
                cout << "Error al listar facturas." << endl;
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

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT id_factura, id_venta, numero_factura, tipo_factura, fecha, total "
                "FROM facturas WHERE id_factura = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idFactura, idVenta;
                char numeroFactura[30];
                char tipoFactura[10];
                char fecha[20];
                float total;

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                    SQLGetData(hStmt, 6, SQL_C_FLOAT, &total, 0, NULL);

                    cout << "\n--- FACTURA ENCONTRADA ---\n";
                    cout << "ID factura: " << idFactura << endl;
                    cout << "ID venta: " << idVenta << endl;
                    cout << "Numero: " << numeroFactura << endl;
                    cout << "Tipo: " << tipoFactura << endl;
                    cout << "Fecha: " << fecha << endl;
                    cout << "Total: $" << total << endl;
                } else {
                    cout << "No se encontro una factura con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar factura." << endl;
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

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "DELETE FROM facturas WHERE id_factura = " + to_string(id);
            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Factura eliminada correctamente." << endl;
            } else {
                cout << "Error al eliminar factura." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
};

#endif