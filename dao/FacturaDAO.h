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

    QSqlQuery listarQt(const QString &rol = "Supervisor", int idUsuario = 0) const {
        QSqlQuery q(conexionQt);
        QString sql =
            "SELECT f.id_factura, CASE WHEN f.id_venta IS NOT NULL THEN 'Venta' ELSE 'Servicio' END, "
            "CASE WHEN f.id_venta IS NOT NULL THEN CONCAT('Venta Nro ', f.id_venta) ELSE CONCAT('Servicio Nro ', f.id_servicio) END, "
            "f.numero_factura, f.tipo_factura, DATE_FORMAT(f.fecha, '%d/%m/%Y'), f.total, f.estado, f.estado_pago, "
            "COALESCE(f.motivo_anulacion, ''), COALESCE(DATE_FORMAT(f.fecha_anulacion, '%d/%m/%Y %H:%i'), ''), "
            "COALESCE(CONCAT(u.nombre, ' ', u.apellido, ' - ', r.nombre), 'Sin usuario') "
            "FROM facturas f LEFT JOIN usuarios u ON f.id_usuario=u.id_usuario LEFT JOIN roles r ON u.id_rol=r.id_rol ";
        if (rol == "Vendedor") sql += "WHERE f.id_usuario=? OR r.nombre='Supervisor' ";
        sql += "ORDER BY f.id_factura DESC";
        q.prepare(sql);
        if (rol == "Vendedor") q.addBindValue(idUsuario);
        q.exec();
        return q;
    }

    bool agregarQt(Factura &f, int idUsuario = 0) const {
        QSqlQuery q(conexionQt);
        q.prepare("INSERT INTO facturas (id_venta, id_servicio, numero_factura, tipo_factura, fecha, total, id_usuario) VALUES (?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(f.getIdVenta() > 0 ? QVariant(f.getIdVenta()) : QVariant());
        q.addBindValue(f.getIdServicio() > 0 ? QVariant(f.getIdServicio()) : QVariant());
        q.addBindValue(QString::fromStdString(f.getNumeroFactura()));
        q.addBindValue(QString::fromStdString(f.getTipoFactura()));
        q.addBindValue(QString::fromStdString(f.getFecha()));
        q.addBindValue(f.getTotal());
        q.addBindValue(idUsuario > 0 ? QVariant(idUsuario) : QVariant());
        return q.exec();
    }

    bool eliminarQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare("DELETE FROM facturas WHERE id_factura=?");
        q.addBindValue(id);
        return q.exec();
    }

    QSqlQuery detalleQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT f.id_factura, f.numero_factura, f.tipo_factura, DATE_FORMAT(f.fecha, '%d/%m/%Y'), f.estado, "
            "CASE WHEN f.id_venta IS NOT NULL THEN 'Venta' ELSE 'Servicio' END, COALESCE(f.id_venta, f.id_servicio), "
            "CASE WHEN f.id_venta IS NOT NULL THEN CONCAT('Venta Nro ', f.id_venta) ELSE CONCAT('Servicio Nro ', f.id_servicio) END, "
            "CASE WHEN f.id_venta IS NOT NULL THEN CONCAT(cv.nombre, ' ', cv.apellido) ELSE CONCAT(cs.nombre, ' ', cs.apellido) END, "
            "f.total, COALESCE(f.motivo_anulacion, ''), COALESCE(DATE_FORMAT(f.fecha_anulacion, '%d/%m/%Y %H:%i'), ''), "
            "f.estado_pago, COALESCE(DATE_FORMAT(f.fecha_pago, '%d/%m/%Y %H:%i'), ''), COALESCE(CONCAT(up.nombre, ' ', up.apellido), ''), "
            "COALESCE(CONCAT(uc.nombre, ' ', uc.apellido, ' - ', rc.nombre), 'Sin usuario'), COALESCE(CONCAT(ua.nombre, ' ', ua.apellido), '') "
            "FROM facturas f LEFT JOIN ventas v ON f.id_venta=v.id_venta LEFT JOIN clientes cv ON v.id_cliente=cv.id_cliente "
            "LEFT JOIN servicios s ON f.id_servicio=s.id_servicio LEFT JOIN clientes cs ON s.id_cliente=cs.id_cliente "
            "LEFT JOIN usuarios up ON f.id_usuario_pago=up.id_usuario LEFT JOIN usuarios uc ON f.id_usuario=uc.id_usuario "
            "LEFT JOIN roles rc ON uc.id_rol=rc.id_rol LEFT JOIN usuarios ua ON f.id_usuario_anulacion=ua.id_usuario WHERE f.id_factura=?"
        );
        q.addBindValue(id);
        q.exec();
        return q;
    }

    QSqlQuery anulacionQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare("SELECT estado, COALESCE(motivo_anulacion, ''), COALESCE(DATE_FORMAT(fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM facturas WHERE id_factura=?");
        q.addBindValue(id);
        q.exec();
        return q;
    }

    bool estaAnuladaQt(int id) const {
        QSqlQuery q = anulacionQt(id);
        return q.next() && q.value(0).toString() == "Anulada";
    }

    bool anularFacturaQt(int id, const QString &motivo, int idUsuarioAnulacion = 0, QString *error = nullptr) {
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
        update.prepare("UPDATE facturas SET estado='Anulada', motivo_anulacion=?, fecha_anulacion=NOW(), id_usuario_anulacion=? WHERE id_factura=? AND estado <> 'Anulada'");
        update.addBindValue(motivo);
        update.addBindValue(idUsuarioAnulacion > 0 ? QVariant(idUsuarioAnulacion) : QVariant());
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

    bool marcarPagadaQt(int id, int idUsuarioPago, QString *error = nullptr) const {
        QSqlQuery q(conexionQt);
        q.prepare("UPDATE facturas SET estado_pago='Pagada', fecha_pago=NOW(), id_usuario_pago=? WHERE id_factura=? AND estado <> 'Anulada' AND estado_pago='Pendiente'");
        q.addBindValue(idUsuarioPago);
        q.addBindValue(id);
        if (!q.exec()) {
            if (error) *error = q.lastError().text();
            return false;
        }
        if (q.numRowsAffected() == 0) {
            if (error) *error = "La factura no esta pendiente o esta anulada.";
            return false;
        }
        return true;
    }

    bool perteneceAUsuarioOSupervisorQt(int id, int idUsuario) const {
        QSqlQuery q(conexionQt);
        q.prepare("SELECT COUNT(*) FROM facturas f LEFT JOIN usuarios u ON f.id_usuario=u.id_usuario LEFT JOIN roles r ON u.id_rol=r.id_rol WHERE f.id_factura=? AND (f.id_usuario=? OR r.nombre='Supervisor')");
        q.addBindValue(id);
        q.addBindValue(idUsuario);
        return q.exec() && q.next() && q.value(0).toInt() > 0;
    }

    QSqlQuery ultimosQt(int limite=7) const {
        QSqlQuery q(conexionQt);
        q.prepare("SELECT id_factura, id_factura, fecha FROM facturas ORDER BY fecha DESC, id_factura DESC LIMIT ?");
        q.addBindValue(limite);
        q.exec();
        return q;
    }
#endif

    bool existe(int id) {
        ConexionBD conexion;
        if (!conexion.conectar()) return false;
        SQLHSTMT hStmt;
        int cantidad = 0;
        SQLAllocHandle(SQL_HANDLE_STMT, conexion.getConexion(), &hStmt);
        string consulta = "SELECT COUNT(*) FROM facturas WHERE id_factura = " + to_string(id);
        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);
        if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, NULL);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
        return cantidad > 0;
    }

    void agregar(Factura factura) {
        ConexionBD conexion;
        if (!conexion.conectar()) return;
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, conexion.getConexion(), &hStmt);
        string idVenta = factura.getIdVenta() > 0 ? to_string(factura.getIdVenta()) : "NULL";
        string idServicio = factura.getIdServicio() > 0 ? to_string(factura.getIdServicio()) : "NULL";
        string consulta = "INSERT INTO facturas (id_venta, id_servicio, numero_factura, tipo_factura, fecha, total) VALUES (" +
            idVenta + ", " + idServicio + ", '" + factura.getNumeroFactura() + "', '" + factura.getTipoFactura() + "', '" + factura.getFecha() + "', " + to_string(factura.getTotal()) + ")";
        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);
        cout << (SQL_SUCCEEDED(ret) ? "Factura generada correctamente." : "Error al generar factura.") << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }

    void listar() {
        ConexionBD conexion;
        if (!conexion.conectar()) return;
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, conexion.getConexion(), &hStmt);
        string consulta = "SELECT id_factura, id_venta, id_servicio, numero_factura, tipo_factura, fecha, total FROM facturas";
        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            int idFactura, idVenta, idServicio;
            char numeroFactura[30], tipoFactura[10], fecha[20];
            float total;
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                idVenta = 0; idServicio = 0;
                SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &idVenta, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_LONG, &idServicio, 0, NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
                SQLGetData(hStmt, 7, SQL_C_FLOAT, &total, 0, NULL);
                cout << "ID factura: " << idFactura << endl;
                cout << "Origen: " << (idVenta > 0 ? "Venta " + to_string(idVenta) : "Servicio " + to_string(idServicio)) << endl;
                cout << "Numero: " << numeroFactura << endl;
                cout << "Tipo: " << tipoFactura << endl;
                cout << "Fecha: " << fecha << endl;
                cout << "Total: $" << total << endl;
                cout << "-------------------------\n";
            }
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }

    void buscarPorId(int id) {
        ConexionBD conexion;
        if (!conexion.conectar()) return;
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, conexion.getConexion(), &hStmt);
        string consulta = "SELECT id_factura, numero_factura, tipo_factura, fecha, total FROM facturas WHERE id_factura = " + to_string(id);
        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);
        if (SQL_SUCCEEDED(ret) && SQLFetch(hStmt) == SQL_SUCCESS) {
            int idFactura;
            char numeroFactura[30], tipoFactura[10], fecha[20];
            float total;
            SQLGetData(hStmt, 1, SQL_C_LONG, &idFactura, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, numeroFactura, sizeof(numeroFactura), NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, tipoFactura, sizeof(tipoFactura), NULL);
            SQLGetData(hStmt, 4, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
            SQLGetData(hStmt, 5, SQL_C_FLOAT, &total, 0, NULL);
            cout << "ID factura: " << idFactura << endl;
            cout << "Numero: " << numeroFactura << endl;
            cout << "Tipo: " << tipoFactura << endl;
            cout << "Fecha: " << fecha << endl;
            cout << "Total: $" << total << endl;
        } else {
            cout << "No existe una factura con ese ID." << endl;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }

    void eliminar(int id) {
        ConexionBD conexion;
        if (!conexion.conectar()) return;
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, conexion.getConexion(), &hStmt);
        string consulta = "DELETE FROM facturas WHERE id_factura = " + to_string(id);
        SQLRETURN ret = SQLExecDirectA(hStmt, (SQLCHAR*)consulta.c_str(), SQL_NTS);
        cout << (SQL_SUCCEEDED(ret) ? "Factura eliminada correctamente." : "Error al eliminar factura.") << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        conexion.desconectar();
    }

private:
#ifdef MCM_QT_APP
    QSqlDatabase conexionQt;
#endif
};

#endif
