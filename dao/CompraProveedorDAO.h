#ifndef COMPRAPROVEEDORDAO_H
#define COMPRAPROVEEDORDAO_H

#include "../models/CompraProveedor.h"
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

class CompraProveedorDAO {
public:
#ifdef MCM_QT_APP
    explicit CompraProveedorDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT cp.id_compra, p.nombre, DATE_FORMAT(cp.fecha, '%d/%m/%Y'), cp.total FROM compras_proveedores cp LEFT JOIN proveedores p ON cp.id_proveedor=p.id_proveedor ORDER BY cp.id_compra DESC"); return q; }
    bool agregarQt(CompraProveedor &c) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO compras_proveedores (id_proveedor, fecha, total) VALUES (?, ?, ?)"); q.addBindValue(c.getIdProveedor()); q.addBindValue(QString::fromStdString(c.getFecha())); q.addBindValue(c.getTotal()); return q.exec(); }
    int obtenerUltimoIdQt() const { QSqlQuery q(conexionQt); return q.exec("SELECT LAST_INSERT_ID()") && q.next() ? q.value(0).toInt() : 0; }
    bool actualizarTotalQt(int id, double total) const { QSqlQuery q(conexionQt); q.prepare("UPDATE compras_proveedores SET total=? WHERE id_compra=?"); q.addBindValue(total); q.addBindValue(id); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM compras_proveedores WHERE id_compra=?"); q.addBindValue(id); return q.exec(); }
    QSqlQuery ultimosQt(int limite=7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_compra, id_compra, fecha FROM compras_proveedores ORDER BY fecha DESC, id_compra DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM compras_proveedores WHERE id_compra = " + to_string(id);

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

    void agregar(CompraProveedor compra) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO compras_proveedores (id_proveedor, fecha, total) VALUES (" +
                to_string(compra.getIdProveedor()) + ", '" +
                compra.getFecha() + "', " +
                to_string(compra.getTotal()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Compra registrada correctamente." << endl;
            } else {
                cout << "Error al registrar compra." << endl;
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
            "SELECT c.id_compra, p.nombre, c.fecha, c.total "
            "FROM compras_proveedores c "
            "INNER JOIN proveedores p ON c.id_proveedor = p.id_proveedor";

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idCompra;
            char proveedor[80];
            char fecha[20];
            float total;
            bool hayCompras = false;

            cout << "\n--- LISTA DE COMPRAS A PROVEEDORES ---\n";

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                hayCompras = true;

                SQLGetData(hStmt, 1, SQL_C_LONG, &idCompra, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, proveedor, sizeof(proveedor), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                SQLGetData(hStmt, 4, SQL_C_FLOAT, &total, 0, NULL);

                cout << "ID compra: " << idCompra << endl;
                cout << "Proveedor: " << proveedor << endl;
                cout << "Fecha: " << fecha << endl;
                cout << "Total: $" << total << endl;
                cout << "-------------------------\n";
            }

            if (!hayCompras) {
                cout << "No hay compras registradas." << endl;
            }
        } else {
            cout << "Error al listar compras." << endl;
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
            "SELECT c.id_compra, p.nombre, c.fecha, c.total "
            "FROM compras_proveedores c "
            "INNER JOIN proveedores p ON c.id_proveedor = p.id_proveedor "
            "WHERE c.id_compra = " + to_string(id);

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idCompra;
            char proveedor[80];
            char fecha[20];
            float total;

            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &idCompra, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, proveedor, sizeof(proveedor), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                SQLGetData(hStmt, 4, SQL_C_FLOAT, &total, 0, NULL);

                cout << "\n--- COMPRA ENCONTRADA ---\n";
                cout << "ID compra: " << idCompra << endl;
                cout << "Proveedor: " << proveedor << endl;
                cout << "Fecha: " << fecha << endl;
                cout << "Total: $" << total << endl;
            } else {
                cout << "No se encontro una compra con ese ID." << endl;
            }
        } else {
            cout << "Error al buscar compra." << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }
}

    void actualizarTotal(int idCompra, float total) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE compras_proveedores SET total = " + to_string(total) +
                " WHERE id_compra = " + to_string(idCompra);

            SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    int obtenerUltimoId() {
        ConexionBD conexion;
        int ultimoId = 0;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "SELECT MAX(id_compra) FROM compras_proveedores";
            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &ultimoId, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }

        return ultimoId;
    }

    void eliminar(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "DELETE FROM compras_proveedores WHERE id_compra = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Compra eliminada correctamente." << endl;
            } else {
                cout << "Error al eliminar compra." << endl;
                cout << "Puede que la compra tenga detalles asociados." << endl;
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
