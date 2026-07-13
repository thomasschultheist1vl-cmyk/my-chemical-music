#ifndef CATEGORIADAO_H
#define CATEGORIADAO_H

#include "../models/Categoria.h"
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

class CategoriaDAO {
public:
#ifdef MCM_QT_APP
    explicit CategoriaDAO(const QSqlDatabase &db) : conexionQt(db) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_categoria, nombre, descripcion FROM categorias ORDER BY id_categoria"); return q; }
    QSqlQuery paraComboQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_categoria, nombre FROM categorias ORDER BY nombre"); return q; }
    bool agregarQt(Categoria &c) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO categorias (nombre, descripcion) VALUES (?, ?)"); q.addBindValue(QString::fromStdString(c.getNombre())); q.addBindValue(QString::fromStdString(c.getDescripcion())); return q.exec(); }
    bool modificarQt(Categoria &c) const { QSqlQuery q(conexionQt); q.prepare("UPDATE categorias SET nombre=?, descripcion=? WHERE id_categoria=?"); q.addBindValue(QString::fromStdString(c.getNombre())); q.addBindValue(QString::fromStdString(c.getDescripcion())); q.addBindValue(c.getIdCategoria()); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM categorias WHERE id_categoria=?"); q.addBindValue(id); return q.exec(); }
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
                "SELECT COUNT(*) FROM categorias WHERE id_categoria = " + to_string(id);

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

    void agregar(Categoria categoria) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO categorias (nombre, descripcion) VALUES ('" +
                categoria.getNombre() + "', '" +
                categoria.getDescripcion() + "')";

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Categoria agregada correctamente." << endl;
            } else {
                cout << "Error al agregar categoria." << endl;
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
                "SELECT id_categoria, nombre, descripcion FROM categorias";

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idCategoria;
                char nombre[60];
                char descripcion[150];

                bool hayCategorias = false;

                cout << "\n--- LISTA DE CATEGORIAS ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayCategorias = true;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idCategoria, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);

                    cout << "ID: " << idCategoria << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "Descripcion: " << descripcion << endl;
                    cout << "-------------------------\n";
                }

                if (!hayCategorias) {
                    cout << "No hay categorias ingresadas." << endl;
                }
            } else {
                cout << "Error al listar categorias." << endl;
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
                "SELECT id_categoria, nombre, descripcion FROM categorias WHERE id_categoria = " +
                to_string(id);

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idCategoria;
                char nombre[60];
                char descripcion[150];

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &idCategoria, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                    SQLGetData(hStmt, 3, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);

                    cout << "\n--- CATEGORIA ENCONTRADA ---\n";
                    cout << "ID: " << idCategoria << endl;
                    cout << "Nombre: " << nombre << endl;
                    cout << "Descripcion: " << descripcion << endl;
                } else {
                    cout << "No se encontro una categoria con ese ID." << endl;
                }
            } else {
                cout << "Error al buscar categoria." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void modificar(Categoria categoria) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            SQLRETURN ret;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE categorias SET "
                "nombre = '" + categoria.getNombre() + "', "
                "descripcion = '" + categoria.getDescripcion() + "' "
                "WHERE id_categoria = " + to_string(categoria.getIdCategoria());

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Categoria modificada correctamente." << endl;
            } else {
                cout << "Error al modificar categoria." << endl;
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
                "DELETE FROM categorias WHERE id_categoria = " + to_string(id);

            ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Categoria eliminada correctamente." << endl;
            } else {
                cout << "Error al eliminar categoria." << endl;
                cout << "Puede que la categoria tenga productos asociados." << endl;
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
