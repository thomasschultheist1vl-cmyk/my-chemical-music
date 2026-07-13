#ifndef CLIENTEDAO_H
#define CLIENTEDAO_H

#include "../models/Cliente.h"
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

class ClienteDAO {
public:
#ifdef MCM_QT_APP
    explicit ClienteDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const {
        QSqlQuery q(conexionQt); q.exec("SELECT id_cliente, nombre, apellido, telefono, email, direccion, dni FROM clientes ORDER BY id_cliente"); return q;
    }
    QSqlQuery paraComboQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_cliente, CONCAT(nombre, ' ', apellido) FROM clientes ORDER BY apellido, nombre"); return q; }
    QSqlQuery buscarQt(int id) const {
        QSqlQuery q(conexionQt); q.prepare("SELECT nombre, apellido, telefono, email, direccion, dni FROM clientes WHERE id_cliente = ?"); q.addBindValue(id); q.exec(); return q;
    }
    bool agregarQt(Cliente &c) const {
        QSqlQuery q(conexionQt); q.prepare("INSERT INTO clientes (nombre, apellido, telefono, email, direccion, dni) VALUES (?, ?, ?, ?, ?, ?)");
        q.addBindValue(QString::fromStdString(c.getNombre())); q.addBindValue(QString::fromStdString(c.getApellido())); q.addBindValue(QString::fromStdString(c.getTelefono())); q.addBindValue(QString::fromStdString(c.getEmail())); q.addBindValue(QString::fromStdString(c.getDireccion())); q.addBindValue(QString::fromStdString(c.getDni())); return q.exec();
    }
    bool modificarQt(Cliente &c) const {
        QSqlQuery q(conexionQt); q.prepare("UPDATE clientes SET nombre = ?, apellido = ?, telefono = ?, email = ?, direccion = ?, dni = ? WHERE id_cliente = ?");
        q.addBindValue(QString::fromStdString(c.getNombre())); q.addBindValue(QString::fromStdString(c.getApellido())); q.addBindValue(QString::fromStdString(c.getTelefono())); q.addBindValue(QString::fromStdString(c.getEmail())); q.addBindValue(QString::fromStdString(c.getDireccion())); q.addBindValue(QString::fromStdString(c.getDni())); q.addBindValue(c.getIdCliente()); return q.exec();
    }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM clientes WHERE id_cliente = ?"); q.addBindValue(id); return q.exec(); }
    QSqlQuery contarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT COUNT(*) FROM clientes"); return q; }
    QSqlQuery ultimosQt(int limite = 7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_cliente, CONCAT(nombre, ' ', apellido) FROM clientes ORDER BY id_cliente DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
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
                "SELECT COUNT(*) FROM clientes WHERE id_cliente = " + to_string(id);

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

    void agregar(Cliente cliente) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO clientes (nombre, apellido, telefono, email, direccion, dni) VALUES ('" +
                cliente.getNombre() + "', '" +
                cliente.getApellido() + "', '" +
                cliente.getTelefono() + "', '" +
                cliente.getEmail() + "', '" +
                cliente.getDireccion() + "', '" +
                cliente.getDni() + "')";

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Cliente agregado correctamente." << endl;
            } else {
                cout << "Error al agregar cliente." << endl;
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
            "SELECT id_cliente, nombre, apellido, telefono, email, direccion, dni FROM clientes";

        ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idCliente;
            char nombre[50];
            char apellido[50];
            char telefono[30];
            char email[80];
            char direccion[120];
            char dni[20];

            bool hayClientes = false;

            cout << "\n--- LISTA DE CLIENTES ---\n";

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                hayClientes = true;

                SQLGetData(hStmt, 1, SQL_C_LONG, &idCliente, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, apellido, sizeof(apellido), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, email, sizeof(email), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, direccion, sizeof(direccion), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, dni, sizeof(dni), NULL);

                cout << "ID: " << idCliente << endl;
                cout << "Nombre: " << nombre << " " << apellido << endl;
                cout << "Telefono: " << telefono << endl;
                cout << "Email: " << email << endl;
                cout << "Direccion: " << direccion << endl;
                cout << "DNI: " << dni << endl;
                cout << "-------------------------\n";
            }

            if (!hayClientes) {
                cout << "No hay clientes ingresados." << endl;
            }
        } else {
            cout << "Error al listar clientes." << endl;
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
                "SELECT id_cliente, nombre, apellido, telefono, email, direccion, dni "
                "FROM clientes WHERE id_cliente = " + to_string(id);

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idCliente;
                char nombre[50];
                char apellido[50];
                char telefono[30];
                char email[80];
                char direccion[120];
                char dni[20];

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idCliente, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, apellido, sizeof(apellido), NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, email, sizeof(email), NULL);
                    SQLGetData(hStmt, 6, SQL_C_CHAR, direccion, sizeof(direccion), NULL);
                    SQLGetData(hStmt, 7, SQL_C_CHAR, dni, sizeof(dni), NULL);

                    cout << "\n--- CLIENTE ENCONTRADO ---\n";
                    cout << "ID: " << idCliente << endl;
                    cout << "Nombre: " << nombre << " " << apellido << endl;
                    cout << "Telefono: " << telefono << endl;
                    cout << "Email: " << email << endl;
                    cout << "Direccion: " << direccion << endl;
                    cout << "DNI: " << dni << endl;
                } else {
                    cout << "No se encontro un cliente con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar cliente." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void modificar(Cliente cliente) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE clientes SET "
                "nombre = '" + cliente.getNombre() + "', "
                "apellido = '" + cliente.getApellido() + "', "
                "telefono = '" + cliente.getTelefono() + "', "
                "email = '" + cliente.getEmail() + "', "
                "direccion = '" + cliente.getDireccion() + "', "
                "dni = '" + cliente.getDni() + "' "
                "WHERE id_cliente = " + to_string(cliente.getIdCliente());

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Cliente modificado correctamente." << endl;
            } else {
                cout << "Error al modificar cliente." << endl;
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
            "DELETE FROM clientes WHERE id_cliente = " + to_string(id);

        ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            cout << "Cliente eliminado correctamente." << endl;
        } else {
            cout << "Error al eliminar cliente." << endl;
            cout << "Puede que el cliente tenga ventas o servicios asociados." << endl;
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
