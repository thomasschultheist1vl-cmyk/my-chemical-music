#ifndef FACTURADAO_H
#define FACTURADAO_H

#include "../models/Factura.h"
#include "../database/ConexionBD.h"
#include <iostream>
#include <string>
#include <sql.h>
#include <sqlext.h>
#ifdef MCM_QT_APP
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#endif

using namespace std;

class FacturaDAO {
public:
#ifdef MCM_QT_APP
    explicit FacturaDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT id_factura, CASE WHEN id_venta IS NOT NULL THEN 'Venta' ELSE 'Servicio' END, CASE WHEN id_venta IS NOT NULL THEN CONCAT('Venta N.º ', id_venta) ELSE CONCAT('Servicio N.º ', id_servicio) END, numero_factura, tipo_factura, DATE_FORMAT(fecha, '%d/%m/%Y'), total, estado, COALESCE(motivo_anulacion, ''), COALESCE(DATE_FORMAT(fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM facturas ORDER BY id_factura DESC"); return q; }
    bool agregarQt(Factura &f) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO facturas (id_venta, id_servicio, numero_factura, tipo_factura, fecha, total) VALUES (?, ?, ?, ?, ?, ?)"); q.addBindValue(f.getIdVenta() > 0 ? QVariant(f.getIdVenta()) : QVariant()); q.addBindValue(f.getIdServicio() > 0 ? QVariant(f.getIdServicio()) : QVariant()); q.addBindValue(QString::fromStdString(f.getNumeroFactura())); q.addBindValue(QString::fromStdString(f.getTipoFactura())); q.addBindValue(QString::fromStdString(f.getFecha())); q.addBindValue(f.getTotal()); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM facturas WHERE id_factura=?"); q.addBindValue(id); return q.exec(); }
    QSqlQuery detalleQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT f.id_factura, f.numero_factura, f.tipo_factura, DATE_FORMAT(f.fecha, '%d/%m/%Y'), f.estado, CASE WHEN f.id_venta IS NOT NULL THEN 'Venta' ELSE 'Servicio' END, COALESCE(f.id_venta, f.id_servicio), CASE WHEN f.id_venta IS NOT NULL THEN CONCAT('Venta N.º ', f.id_venta) ELSE CONCAT('Servicio N.º ', f.id_servicio) END, CASE WHEN f.id_venta IS NOT NULL THEN CONCAT(cv.nombre, ' ', cv.apellido) ELSE CONCAT(cs.nombre, ' ', cs.apellido) END, f.total, COALESCE(f.motivo_anulacion, ''), COALESCE(DATE_FORMAT(f.fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM facturas f LEFT JOIN ventas v ON f.id_venta=v.id_venta LEFT JOIN clientes cv ON v.id_cliente=cv.id_cliente LEFT JOIN servicios s ON f.id_servicio=s.id_servicio LEFT JOIN clientes cs ON s.id_cliente=cs.id_cliente WHERE f.id_factura=?"); q.addBindValue(id); q.exec(); return q; }
    QSqlQuery anulacionQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT estado, COALESCE(motivo_anulacion, ''), COALESCE(DATE_FORMAT(fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM facturas WHERE id_factura=?"); q.addBindValue(id); q.exec(); return q; }
    bool estaAnuladaQt(int id) const { QSqlQuery q = anulacionQt(id); return q.next() && q.value(0).toString() == "Anulada"; }
    bool anularFacturaQt(int id, const QString &motivo, QString *error = nullptr) {
        auto setError = [error](const QString &texto) { if (error) *error = texto; };
        if (!conexionQt.transaction()) {
            setError(conexionQt.lastError().text());
            return false;
        }

        QSqlQuery estado(conexionQt);
        estado.prepare("SELECT estado FROM facturas WHERE id_factura=? FOR UPDATE");
        estado.addBindValue(id);
        if (!estado.exec() || !estado.next()) {
            setError(estado.lastError().isValid() ? estado.lastError().text() : "No se encontro la factura.");
            conexionQt.rollback();
            return false;
        }
        if (estado.value(0).toString() == "Anulada") {
            setError("La factura ya esta anulada.");
            conexionQt.rollback();
            return false;
        }

        QSqlQuery update(conexionQt);
        update.prepare("UPDATE facturas SET estado='Anulada', motivo_anulacion=?, fecha_anulacion=NOW() WHERE id_factura=? AND estado <> 'Anulada'");
        update.addBindValue(motivo);
        update.addBindValue(id);
        if (!update.exec() || update.numRowsAffected() == 0) {
            setError(update.lastError().isValid() ? update.lastError().text() : "No se pudo anular la factura.");
            conexionQt.rollback();
            return false;
        }

        if (!conexionQt.commit()) {
            setError(conexionQt.lastError().text());
            conexionQt.rollback();
            return false;
        }
        return true;
    }
    QSqlQuery ultimosQt(int limite=7) const { QSqlQuery q(conexionQt); q.prepare("SELECT id_factura, id_factura, fecha FROM facturas ORDER BY fecha DESC, id_factura DESC LIMIT ?"); q.addBindValue(limite); q.exec(); return q; }
#endif
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

            string idVenta = factura.getIdVenta() > 0 ? to_string(factura.getIdVenta()) : "NULL";
            string idServicio = factura.getIdServicio() > 0 ? to_string(factura.getIdServicio()) : "NULL";
            string consulta =
                "INSERT INTO facturas (id_venta, id_servicio, numero_factura, tipo_factura, fecha, total) VALUES (" +
                idVenta + ", " +
                idServicio + ", '" +
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
                "SELECT id_factura, id_venta, id_servicio, numero_factura, tipo_factura, fecha, total FROM facturas";

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idFactura, idVenta, idServicio;
                char numeroFactura[30];
                char tipoFactura[10];
                char fecha[20];
                float total;
                bool hayFacturas = false;

                cout << "\n--- LISTA DE FACTURAS ---\n";

                while (SQLFetch(hStmt) == SQL_SUCCESS) {
                    hayFacturas = true;

                    idVenta = 0;
                    idServicio = 0;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idServicio, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
                    SQLGetData(hStmt, 6, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                    SQLGetData(hStmt, 7, SQL_C_FLOAT, &total, 0, NULL);

                    cout << "ID factura: " << idFactura << endl;
                    if (idVenta > 0) {
                        cout << "Origen: Venta " << idVenta << endl;
                    } else {
                        cout << "Origen: Servicio " << idServicio << endl;
                    }
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
                "SELECT id_factura, id_venta, id_servicio, numero_factura, tipo_factura, fecha, total "
                "FROM facturas WHERE id_factura = " + to_string(id);

            SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);

            if (SQL_SUCCEEDED(ret)) {
                int idFactura, idVenta, idServicio;
                char numeroFactura[30];
                char tipoFactura[10];
                char fecha[20];
                float total;

                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    idVenta = 0;
                    idServicio = 0;

                    SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                    SQLGetData(hStmt, 3, SQL_C_LONG, &idServicio, 0, NULL);
                    SQLGetData(hStmt, 4, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
                    SQLGetData(hStmt, 5, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
                    SQLGetData(hStmt, 6, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                    SQLGetData(hStmt, 7, SQL_C_FLOAT, &total, 0, NULL);

                    cout << "\n--- FACTURA ENCONTRADA ---\n";
                    cout << "ID factura: " << idFactura << endl;
                    if (idVenta > 0) {
                        cout << "Origen: Venta " << idVenta << endl;
                    } else {
                        cout << "Origen: Servicio " << idServicio << endl;
                    }
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
#ifdef MCM_QT_APP
private: QSqlDatabase conexionQt;
#endif
};

#endif
