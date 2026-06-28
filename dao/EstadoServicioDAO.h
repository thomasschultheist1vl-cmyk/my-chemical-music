#ifndef ESTADOSERVICIODAO_H
#define ESTADOSERVICIODAO_H

#include "../models/EstadoServicio.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>

using namespace std;

class EstadoServicioDAO {
public:
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM estados_servicio WHERE id_estado_servicio = " + to_string(id);

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

    void agregar(EstadoServicio estadoServicio) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO estados_servicio (nombre) VALUES ('" +
                estadoServicio.getNombre() + "')";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Estado de servicio agregado correctamente." << endl;
            } else {
                cout << "Error al agregar estado de servicio." << endl;
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
                "SELECT id_estado_servicio, nombre FROM estados_servicio";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idEstadoServicio;
                char nombre[60];
                bool hayEstados = false;

                cout << "\n--- LISTA DE ESTADOS DE SERVICIO ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayEstados = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idEstadoServicio, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);

                    cout << "ID: " << idEstadoServicio << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "-------------------------\n";
                }

                if (!hayEstados) {
                    cout << "No hay estados de servicio ingresados." << endl;
                }
            } else {
                cout << "Error al listar estados de servicio." << endl;
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
                "SELECT id_estado_servicio, nombre FROM estados_servicio "
                "WHERE id_estado_servicio = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idEstadoServicio;
                char nombre[60];

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idEstadoServicio, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);

                    cout << "\n--- ESTADO DE SERVICIO ENCONTRADO ---\n";
                    cout << "ID: " << idEstadoServicio << endl;
                    cout << "Nombre: " << nombre << endl;
                } else {
                    cout << "No se encontro un estado de servicio con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar estado de servicio." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void modificar(EstadoServicio estadoServicio) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE estados_servicio SET nombre = '" + estadoServicio.getNombre() + "' "
                "WHERE id_estado_servicio = " + to_string(estadoServicio.getIdEstadoServicio());

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Estado de servicio modificado correctamente." << endl;
            } else {
                cout << "Error al modificar estado de servicio." << endl;
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
                "DELETE FROM estados_servicio WHERE id_estado_servicio = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Estado de servicio eliminado correctamente." << endl;
            } else {
                cout << "Error al eliminar estado de servicio." << endl;
                cout << "Puede que el estado tenga servicios asociados." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
};

#endif