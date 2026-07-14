#ifndef SERVICIODAO_H
#define SERVICIODAO_H

#include "../models/Servicio.h"
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

class ServicioDAO {
public:
#ifdef MCM_QT_APP
    explicit ServicioDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT s.id_servicio, CONCAT(c.nombre, ' ', c.apellido), s.instrumento, s.descripcion, DATE_FORMAT(s.fecha_ingreso, '%d/%m/%Y'), DATE_FORMAT(s.fecha_entrega, '%d/%m/%Y'), s.precio, es.nombre FROM servicios s LEFT JOIN clientes c ON s.id_cliente=c.id_cliente LEFT JOIN estados_servicio es ON s.id_estado_servicio=es.id_estado_servicio ORDER BY s.id_servicio DESC"); return q; }
    QSqlQuery buscarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_cliente, instrumento, descripcion, fecha_ingreso, fecha_entrega, precio, id_estado_servicio FROM servicios WHERE id_servicio=?"); q.addBindValue(id); q.exec(); return q; }
    bool agregarQt(Servicio &s) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO servicios (id_cliente, instrumento, descripcion, fecha_ingreso, fecha_entrega, precio, id_estado_servicio) VALUES (?, ?, ?, ?, ?, ?, ?)"); q.addBindValue(s.getIdCliente()); q.addBindValue(QString::fromStdString(s.getInstrumento())); q.addBindValue(QString::fromStdString(s.getDescripcion())); q.addBindValue(QString::fromStdString(s.getFechaIngreso())); q.addBindValue(QString::fromStdString(s.getFechaEntrega())); q.addBindValue(s.getPrecio()); q.addBindValue(s.getIdEstadoServicio()); return q.exec(); }
    bool modificarQt(Servicio &s) const { QSqlQuery q(conexionQt); q.prepare("UPDATE servicios SET id_cliente=?, instrumento=?, descripcion=?, fecha_ingreso=?, fecha_entrega=?, precio=?, id_estado_servicio=? WHERE id_servicio=?"); q.addBindValue(s.getIdCliente()); q.addBindValue(QString::fromStdString(s.getInstrumento())); q.addBindValue(QString::fromStdString(s.getDescripcion())); q.addBindValue(QString::fromStdString(s.getFechaIngreso())); q.addBindValue(QString::fromStdString(s.getFechaEntrega())); q.addBindValue(s.getPrecio()); q.addBindValue(s.getIdEstadoServicio()); q.addBindValue(s.getIdServicio()); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM servicios WHERE id_servicio=?"); q.addBindValue(id); return q.exec(); }
    bool cambiarEstadoQt(int id, int estado) const { QSqlQuery q(conexionQt); q.prepare("UPDATE servicios SET id_estado_servicio=? WHERE id_servicio=?"); q.addBindValue(estado); q.addBindValue(id); return q.exec(); }
    QSqlQuery paraComboQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_servicio, CONCAT('Servicio ', id_servicio, ' - ', instrumento, ' - $', precio) FROM servicios ORDER BY id_servicio DESC"); return q; }
    QSqlQuery disponiblesParaFacturarQt() const {
        QSqlQuery q(conexionQt);
        q.exec(
            "SELECT s.id_servicio, CONCAT(c.nombre, ' ', c.apellido), "
            "s.instrumento, s.descripcion, s.precio "
            "FROM servicios s "
            "LEFT JOIN clientes c ON s.id_cliente = c.id_cliente "
            "LEFT JOIN facturas f ON s.id_servicio = f.id_servicio "
            "WHERE f.id_factura IS NULL "
            "ORDER BY s.id_servicio DESC"
        );
        return q;
    }
    QSqlQuery detalleParaFacturaQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT CONCAT(c.nombre, ' ', c.apellido), s.instrumento, s.descripcion, "
            "DATE_FORMAT(s.fecha_ingreso, '%d/%m/%Y'), "
            "DATE_FORMAT(s.fecha_entrega, '%d/%m/%Y'), es.nombre, s.precio "
            "FROM servicios s "
            "LEFT JOIN clientes c ON s.id_cliente = c.id_cliente "
            "LEFT JOIN estados_servicio es ON s.id_estado_servicio = es.id_estado_servicio "
            "WHERE s.id_servicio = ?"
        );
        q.addBindValue(id);
        q.exec();
        return q;
    }
    QSqlQuery precioQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT precio FROM servicios WHERE id_servicio=?"); q.addBindValue(id); q.exec(); return q; }
    QSqlQuery contarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT COUNT(*) FROM servicios"); return q; }
    QSqlQuery ultimosQt(int limite=7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_servicio, id_servicio, fecha_ingreso FROM servicios ORDER BY fecha_ingreso DESC, id_servicio DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
#endif
    bool existe(int id) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;
            int cantidad = 0;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT COUNT(*) FROM servicios WHERE id_servicio = " + to_string(id);

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

    void agregar(Servicio servicio) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "INSERT INTO servicios "
                "(id_cliente, instrumento, descripcion, fecha_ingreso, fecha_entrega, precio, id_estado_servicio) VALUES (" +
                to_string(servicio.getIdCliente()) + ", '" +
                servicio.getInstrumento() + "', '" +
                servicio.getDescripcion() + "', '" +
                servicio.getFechaIngreso() + "', '" +
                servicio.getFechaEntrega() + "', " +
                to_string(servicio.getPrecio()) + ", " +
                to_string(servicio.getIdEstadoServicio()) + ")";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Servicio registrado correctamente." << endl;
            } else {
                cout << "Error al registrar servicio." << endl;
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
            "SELECT s.id_servicio, s.id_cliente, s.instrumento, s.descripcion, "
            "s.fecha_ingreso, s.fecha_entrega, s.precio, e.nombre "
            "FROM servicios s "
            "INNER JOIN estados_servicio e ON s.id_estado_servicio = e.id_estado_servicio";

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idServicio, idCliente;
            char instrumento[80];
            char descripcion[250];
            char fechaIngreso[20];
            char fechaEntrega[20];
            char estado[60];
            float precio;
            bool hayServicios = false;

            cout << "\n--- LISTA DE SERVICIOS ---\n";

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                hayServicios = true;

                SQLGetData(hStmt, 1, SQL_C_LONG, &idServicio, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &idCliente, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, instrumento, sizeof(instrumento), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, fechaIngreso, sizeof(fechaIngreso), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, fechaEntrega, sizeof(fechaEntrega), NULL);
                SQLGetData(hStmt, 7, SQL_C_FLOAT, &precio, 0, NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, estado, sizeof(estado), NULL);

                cout << "ID servicio: " << idServicio << endl;
                cout << "ID cliente: " << idCliente << endl;
                cout << "Instrumento: " << instrumento << endl;
                cout << "Descripcion: " << descripcion << endl;
                cout << "Fecha ingreso: " << fechaIngreso << endl;
                cout << "Fecha entrega: " << fechaEntrega << endl;
                cout << "Precio: $" << precio << endl;
                cout << "Estado: " << estado << endl;
                cout << "-------------------------\n";
            }

            if (!hayServicios) {
                cout << "No hay servicios registrados." << endl;
            }
        } else {
            cout << "Error al listar servicios." << endl;
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
            "SELECT s.id_servicio, s.id_cliente, s.instrumento, s.descripcion, "
            "s.fecha_ingreso, s.fecha_entrega, s.precio, e.nombre "
            "FROM servicios s "
            "INNER JOIN estados_servicio e ON s.id_estado_servicio = e.id_estado_servicio "
            "WHERE s.id_servicio = " + to_string(id);

        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            int idServicio, idCliente;
            char instrumento[80];
            char descripcion[250];
            char fechaIngreso[20];
            char fechaEntrega[20];
            char estado[60];
            float precio;

            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &idServicio, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &idCliente, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, instrumento, sizeof(instrumento), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, fechaIngreso, sizeof(fechaIngreso), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, fechaEntrega, sizeof(fechaEntrega), NULL);
                SQLGetData(hStmt, 7, SQL_C_FLOAT, &precio, 0, NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, estado, sizeof(estado), NULL);

                cout << "\n--- SERVICIO ENCONTRADO ---\n";
                cout << "ID servicio: " << idServicio << endl;
                cout << "ID cliente: " << idCliente << endl;
                cout << "Instrumento: " << instrumento << endl;
                cout << "Descripcion: " << descripcion << endl;
                cout << "Fecha ingreso: " << fechaIngreso << endl;
                cout << "Fecha entrega: " << fechaEntrega << endl;
                cout << "Precio: $" << precio << endl;
                cout << "Estado: " << estado << endl;
            } else {
                cout << "No se encontro un servicio con ese ID." << endl;
            }
        } else {
            cout << "Error al buscar servicio." << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }
}

    void modificar(Servicio servicio) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE servicios SET "
                "id_cliente = " + to_string(servicio.getIdCliente()) + ", "
                "instrumento = '" + servicio.getInstrumento() + "', "
                "descripcion = '" + servicio.getDescripcion() + "', "
                "fecha_ingreso = '" + servicio.getFechaIngreso() + "', "
                "fecha_entrega = '" + servicio.getFechaEntrega() + "', "
                "precio = " + to_string(servicio.getPrecio()) + ", "
                "id_estado_servicio = " + to_string(servicio.getIdEstadoServicio()) + " "
                "WHERE id_servicio = " + to_string(servicio.getIdServicio());

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Servicio modificado correctamente." << endl;
            } else {
                cout << "Error al modificar servicio." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    void cambiarEstado(int idServicio, int idEstadoServicio) {
        ConexionBD conexion;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "UPDATE servicios SET id_estado_servicio = " + to_string(idEstadoServicio) +
                " WHERE id_servicio = " + to_string(idServicio);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Estado del servicio actualizado correctamente." << endl;
            } else {
                cout << "Error al cambiar estado del servicio." << endl;
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
                "DELETE FROM servicios WHERE id_servicio = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                cout << "Servicio eliminado correctamente." << endl;
            } else {
                cout << "Error al eliminar servicio." << endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }
    }

    float obtenerPrecio(int idServicio) {
        ConexionBD conexion;
        float precio = 0;

        if (conexion.conectar()) {
            SQLHDBC hDbc = conexion.getConexion();
            SQLHSTMT hStmt;

            SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

            string consulta =
                "SELECT precio FROM servicios WHERE id_servicio = " + to_string(idServicio);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_FLOAT, &precio, 0, NULL);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            conexion.desconectar();
        }

        return precio;
    }
#ifdef MCM_QT_APP
private: QSqlDatabase conexionQt;
#endif
};

#endif
