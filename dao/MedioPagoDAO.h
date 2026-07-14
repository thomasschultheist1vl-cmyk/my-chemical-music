#ifndef MEDIOPAGODAO_H
#define MEDIOPAGODAO_H

#include "../models/MedioPago.h"
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

class MedioPagoDAO {
public:
#ifdef MCM_QT_APP
    explicit MedioPagoDAO(const QSqlDatabase &db) : conexionQt(db) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_medio_pago, nombre FROM medios_pago ORDER BY id_medio_pago"); return q; }
    QSqlQuery paraComboQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_medio_pago, nombre FROM medios_pago ORDER BY id_medio_pago"); return q; }
    QSqlQuery detalleQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_medio_pago, nombre FROM medios_pago WHERE id_medio_pago=?"); q.addBindValue(id); q.exec(); return q; }
    bool agregarQt(MedioPago &m) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO medios_pago (nombre) VALUES (?)"); q.addBindValue(QString::fromStdString(m.getNombre())); return q.exec(); }
    bool modificarQt(MedioPago &m) const { QSqlQuery q(conexionQt); q.prepare("UPDATE medios_pago SET nombre=? WHERE id_medio_pago=?"); q.addBindValue(QString::fromStdString(m.getNombre())); q.addBindValue(m.getIdMedioPago()); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM medios_pago WHERE id_medio_pago=?"); q.addBindValue(id); return q.exec(); }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "SELECT COUNT(*) FROM medios_pago WHERE id_medio_pago = " + to_string(id);
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

    void agregar(MedioPago medioPago) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO medios_pago (nombre) VALUES ('" +
                medioPago.getNombre() + "')";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Medio de pago agregado correctamente." << endl;
            } else {
                cout << "Error al agregar medio de pago." << endl;
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

            string consulta = "SELECT id_medio_pago, nombre FROM medios_pago";
            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idMedioPago;
                char nombre[60];
                bool hayMediosPago = false;

                cout << "\n--- LISTA DE MEDIOS DE PAGO ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayMediosPago = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idMedioPago, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);

                    cout << "ID: " << idMedioPago << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "-------------------------\n";
                }

                if (!hayMediosPago) {
                    cout << "No hay medios de pago ingresados." << endl;
                }
            } else {
                cout << "Error al listar medios de pago." << endl;
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
                "SELECT id_medio_pago, nombre FROM medios_pago WHERE id_medio_pago = " +
                to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idMedioPago;
                char nombre[60];

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idMedioPago, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);

                    cout << "\n--- MEDIO DE PAGO ENCONTRADO ---\n";
                    cout << "ID: " << idMedioPago << endl;
                    cout << "Nombre: " << nombre << endl;
                } else {
                    cout << "No se encontro un medio de pago con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar medio de pago." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void modificar(MedioPago medioPago) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE medios_pago SET nombre = '" + medioPago.getNombre() + "' "
                "WHERE id_medio_pago = " + to_string(medioPago.getIdMedioPago());

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Medio de pago modificado correctamente." << endl;
            } else {
                cout << "Error al modificar medio de pago." << endl;
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

            string consulta =
                "DELETE FROM medios_pago WHERE id_medio_pago = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Medio de pago eliminado correctamente." << endl;
            } else {
                cout << "Error al eliminar medio de pago." << endl;
                cout << "Puede que el medio de pago tenga ventas asociadas." << endl;
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
