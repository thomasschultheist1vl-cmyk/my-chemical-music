#ifndef PROVEEDORDAO_H
#define PROVEEDORDAO_H

#include "../models/Proveedor.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>

using namespace std;

class ProveedorDAO {
public:
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta = "SELECT COUNT(*) FROM proveedores WHERE id_proveedor = " + to_string(id);
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

    void agregar(Proveedor proveedor) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO proveedores (nombre, telefono, email, direccion, cuit) VALUES ('" +
                proveedor.getNombre() + "', '" +
                proveedor.getTelefono() + "', '" +
                proveedor.getEmail() + "', '" +
                proveedor.getDireccion() + "', '" +
                proveedor.getCuit() + "')";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Proveedor agregado correctamente." << endl;
            } else {
                cout << "Error al agregar proveedor." << endl;
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
                "SELECT id_proveedor, nombre, telefono, email, direccion, cuit FROM proveedores";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idProveedor;
                char nombre[80];
                char telefono[30];
                char email[80];
                char direccion[120];
                char cuit[30];

                bool hayProveedores = false;

                cout << "\n--- LISTA DE PROVEEDORES ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayProveedores = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idProveedor, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, email, sizeof(email), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, direccion, sizeof(direccion), NULL);
                    SQLGetData(hStmt, 6, SQL_C_CHAR, cuit, sizeof(cuit), NULL);

                    cout << "ID: " << idProveedor << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "Telefono: " << telefono << endl;
                    cout << "Email: " << email << endl;
                    cout << "Direccion: " << direccion << endl;
                    cout << "CUIT: " << cuit << endl;
                    cout << "-------------------------\n";
                }

                if (!hayProveedores) {
                    cout << "No hay proveedores ingresados." << endl;
                }
            } else {
                cout << "Error al listar proveedores." << endl;
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
                "SELECT id_proveedor, nombre, telefono, email, direccion, cuit "
                "FROM proveedores WHERE id_proveedor = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idProveedor;
                char nombre[80];
                char telefono[30];
                char email[80];
                char direccion[120];
                char cuit[30];

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idProveedor, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, email, sizeof(email), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, direccion, sizeof(direccion), NULL);
                    SQLGetData(hStmt, 6, SQL_C_CHAR, cuit, sizeof(cuit), NULL);

                    cout << "\n--- PROVEEDOR ENCONTRADO ---\n";
                    cout << "ID: " << idProveedor << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "Telefono: " << telefono << endl;
                    cout << "Email: " << email << endl;
                    cout << "Direccion: " << direccion << endl;
                    cout << "CUIT: " << cuit << endl;
                } else {
                    cout << "No se encontro un proveedor con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar proveedor." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void modificar(Proveedor proveedor) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE proveedores SET "
                "nombre = '" + proveedor.getNombre() + "', "
                "telefono = '" + proveedor.getTelefono() + "', "
                "email = '" + proveedor.getEmail() + "', "
                "direccion = '" + proveedor.getDireccion() + "', "
                "cuit = '" + proveedor.getCuit() + "' "
                "WHERE id_proveedor = " + to_string(proveedor.getIdProveedor());

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Proveedor modificado correctamente." << endl;
            } else {
                cout << "Error al modificar proveedor." << endl;
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
                "DELETE FROM proveedores WHERE id_proveedor = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Proveedor eliminado correctamente." << endl;
            } else {
                cout << "Error al eliminar proveedor." << endl;
                cout << "Puede que el proveedor tenga productos o compras asociadas." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }
};

#endif