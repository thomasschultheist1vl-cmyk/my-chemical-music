#ifndef VENTADAO_H
#define VENTADAO_H

#include "../models/Venta.h"
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

class VentaDAO {
public:
#ifdef MCM_QT_APP
    explicit VentaDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT v.id_venta, CONCAT(c.nombre, ' ', c.apellido), DATE_FORMAT(v.fecha, '%d/%m/%Y'), v.total, mp.nombre FROM ventas v LEFT JOIN clientes c ON v.id_cliente=c.id_cliente LEFT JOIN medios_pago mp ON v.id_medio_pago=mp.id_medio_pago ORDER BY v.id_venta DESC"); return q; }
    bool agregarQt(Venta &v) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO ventas (id_cliente, fecha, total, id_medio_pago) VALUES (?, ?, ?, ?)"); q.addBindValue(v.getIdCliente()); q.addBindValue(QString::fromStdString(v.getFecha())); q.addBindValue(v.getTotal()); q.addBindValue(v.getIdMedioPago()); return q.exec(); }
    int obtenerUltimoIdQt() const { QSqlQuery q(conexionQt); return q.exec("SELECT LAST_INSERT_ID()") && q.next() ? q.value(0).toInt() : 0; }
    bool actualizarTotalQt(int id, double total) const { QSqlQuery q(conexionQt); q.prepare("UPDATE ventas SET total=? WHERE id_venta=?"); q.addBindValue(total); q.addBindValue(id); return q.exec(); }
    QSqlQuery paraComboQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_venta, CONCAT('Venta ', id_venta, ' - $', total) FROM ventas ORDER BY id_venta DESC"); return q; }
    QSqlQuery totalQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT total FROM ventas WHERE id_venta=?"); q.addBindValue(id); q.exec(); return q; }
    QSqlQuery contarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT COUNT(*) FROM ventas"); return q; }
    QSqlQuery ultimosQt(int limite=7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_venta, id_venta, fecha FROM ventas ORDER BY fecha DESC, id_venta DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "SELECT COUNT(*) FROM ventas WHERE id_venta = " + to_string(id);
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

    void agregar(Venta venta) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO ventas (id_cliente, fecha, total, id_medio_pago) VALUES (" +
                to_string(venta.getIdCliente()) + ", '" +
                venta.getFecha() + "', " +
                to_string(venta.getTotal()) + ", " +
                to_string(venta.getIdMedioPago()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Venta registrada correctamente." << endl;
            } else {
                cout << "Error al registrar venta." << endl;
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
            "SELECT v.id_venta, c.nombre, c.apellido, v.fecha, v.total, m.nombre "
            "FROM ventas v "
            "INNER JOIN clientes c ON v.id_cliente = c.id_cliente "
            "INNER JOIN medios_pago m ON v.id_medio_pago = m.id_medio_pago";

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idVenta;
            char nombreCliente[50];
            char apellidoCliente[50];
            char fecha[20];
            char medioPago[60];
            float total;
            bool hayVentas = false;

            cout << "\n--- LISTA DE VENTAS ---\n";

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                hayVentas = true;

                SQLGetData(hStmt, 1, SQL_C_LONG, &idVenta, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombreCliente, sizeof(nombreCliente), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, apellidoCliente, sizeof(apellidoCliente), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                SQLGetData(hStmt, 5, SQL_C_FLOAT, &total, 0, NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, medioPago, sizeof(medioPago), NULL);

                cout << "ID venta: " << idVenta << endl;
                cout << "Cliente: " << nombreCliente << " " << apellidoCliente << endl;
                cout << "Fecha: " << fecha << endl;
                cout << "Total: $" << total << endl;
                cout << "Medio de pago: " << medioPago << endl;
                cout << "-------------------------\n";
            }

            if (!hayVentas) {
                cout << "No hay ventas registradas." << endl;
            }
        } else {
            cout << "Error al listar ventas." << endl;
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
            "SELECT v.id_venta, c.nombre, c.apellido, v.fecha, v.total, m.nombre "
            "FROM ventas v "
            "INNER JOIN clientes c ON v.id_cliente = c.id_cliente "
            "INNER JOIN medios_pago m ON v.id_medio_pago = m.id_medio_pago "
            "WHERE v.id_venta = " + to_string(id);

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idVenta;
            char nombreCliente[50];
            char apellidoCliente[50];
            char fecha[20];
            char medioPago[60];
            float total;

            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &idVenta, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombreCliente, sizeof(nombreCliente), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, apellidoCliente, sizeof(apellidoCliente), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                SQLGetData(hStmt, 5, SQL_C_FLOAT, &total, 0, NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, medioPago, sizeof(medioPago), NULL);

                cout << "\n--- VENTA ENCONTRADA ---\n";
                cout << "ID venta: " << idVenta << endl;
                cout << "Cliente: " << nombreCliente << " " << apellidoCliente << endl;
                cout << "Fecha: " << fecha << endl;
                cout << "Total: $" << total << endl;
                cout << "Medio de pago: " << medioPago << endl;
            } else {
                cout << "No se encontro una venta con ese ID." << endl;
            }
        } else {
            cout << "Error al buscar venta." << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }
}

    void modificar(Venta venta) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE ventas SET "
                "id_cliente = " + to_string(venta.getIdCliente()) + ", "
                "fecha = '" + venta.getFecha() + "', "
                "total = " + to_string(venta.getTotal()) + ", "
                "id_medio_pago = " + to_string(venta.getIdMedioPago()) + " "
                "WHERE id_venta = " + to_string(venta.getIdVenta());

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Venta modificada correctamente." << endl;
            } else {
                cout << "Error al modificar venta." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void actualizarTotal(int idVenta, float total) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE ventas SET total = " + to_string(total) +
                " WHERE id_venta = " + to_string(idVenta);

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

            string consulta = "SELECT MAX(id_venta) FROM ventas";
            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &ultimoId, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }

        return ultimoId;
    }


float obtenerTotal(int idVenta) {
    ConexionBD conexion;
    float total = 0;

    if (conexion.conectar()) {
        SQLHDBC hDbc = conexion.getConexion();
        SQLHSTMT hStmt;

        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        string consulta =
            "SELECT total FROM ventas WHERE id_venta = " + to_string(idVenta);

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_FLOAT, &total, 0, NULL);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }

    return total;
}



    void eliminar(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "DELETE FROM ventas WHERE id_venta = " + to_string(id);
            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Venta eliminada correctamente." << endl;
            } else {
                cout << "Error al eliminar venta." << endl;
                cout << "Puede que la venta tenga detalles o factura asociados." << endl;
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
