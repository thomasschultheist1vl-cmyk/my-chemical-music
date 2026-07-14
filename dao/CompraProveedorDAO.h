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
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QPair>
#include <QVariant>
#include <QVector>
#endif

using namespace std;

class CompraProveedorDAO {
public:
#ifdef MCM_QT_APP
    explicit CompraProveedorDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}
    QSqlQuery listarQt() const { QSqlQuery q(conexionQt); q.exec("SELECT cp.id_compra, p.nombre, DATE_FORMAT(cp.fecha, '%d/%m/%Y'), cp.total, cp.estado, COALESCE(cp.motivo_anulacion, ''), COALESCE(DATE_FORMAT(cp.fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM compras_proveedores cp LEFT JOIN proveedores p ON cp.id_proveedor=p.id_proveedor ORDER BY cp.id_compra DESC"); return q; }
    bool agregarQt(CompraProveedor &c) const { QSqlQuery q(conexionQt); q.prepare("INSERT INTO compras_proveedores (id_proveedor, fecha, total) VALUES (?, ?, ?)"); q.addBindValue(c.getIdProveedor()); q.addBindValue(QString::fromStdString(c.getFecha())); q.addBindValue(c.getTotal()); return q.exec(); }
    int obtenerUltimoIdQt() const { QSqlQuery q(conexionQt); return q.exec("SELECT LAST_INSERT_ID()") && q.next() ? q.value(0).toInt() : 0; }
    bool actualizarTotalQt(int id, double total) const { QSqlQuery q(conexionQt); q.prepare("UPDATE compras_proveedores SET total=? WHERE id_compra=?"); q.addBindValue(total); q.addBindValue(id); return q.exec(); }
    bool eliminarQt(int id) const { QSqlQuery q(conexionQt); q.prepare("DELETE FROM compras_proveedores WHERE id_compra=?"); q.addBindValue(id); return q.exec(); }
    QSqlQuery detalleQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT cp.id_compra, p.nombre, DATE_FORMAT(cp.fecha, '%d/%m/%Y'), cp.total, cp.estado, COALESCE(cp.motivo_anulacion, ''), COALESCE(DATE_FORMAT(cp.fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM compras_proveedores cp LEFT JOIN proveedores p ON cp.id_proveedor=p.id_proveedor WHERE cp.id_compra=?"); q.addBindValue(id); q.exec(); return q; }
    QSqlQuery anulacionQt(int id) const { QSqlQuery q(conexionQt); q.prepare("SELECT estado, COALESCE(motivo_anulacion, ''), COALESCE(DATE_FORMAT(fecha_anulacion, '%d/%m/%Y %H:%i'), '') FROM compras_proveedores WHERE id_compra=?"); q.addBindValue(id); q.exec(); return q; }
    bool estaAnuladaQt(int id) const { QSqlQuery q = anulacionQt(id); return q.next() && q.value(0).toString() == "Anulada"; }
    bool anularCompraQt(int id, const QString &motivo, QString *error = nullptr) {
        auto setError = [error](const QString &texto) { if (error) *error = texto; };
        if (!conexionQt.transaction()) {
            setError(conexionQt.lastError().text());
            return false;
        }

        QSqlQuery estado(conexionQt);
        estado.prepare("SELECT estado FROM compras_proveedores WHERE id_compra=? FOR UPDATE");
        estado.addBindValue(id);
        if (!estado.exec() || !estado.next()) {
            setError(estado.lastError().isValid() ? estado.lastError().text() : "No se encontro la compra.");
            conexionQt.rollback();
            return false;
        }
        if (estado.value(0).toString() == "Anulada") {
            setError("La compra ya esta anulada.");
            conexionQt.rollback();
            return false;
        }

        struct DetalleStock { int idProducto; QString nombre; int cantidad; int stock; };
        QVector<DetalleStock> detalles;
        QSqlQuery detalle(conexionQt);
        detalle.prepare(
            "SELECT dc.id_producto, p.nombre, dc.cantidad, p.stock "
            "FROM detalle_compras dc "
            "INNER JOIN productos p ON dc.id_producto = p.id_producto "
            "WHERE dc.id_compra=? FOR UPDATE"
        );
        detalle.addBindValue(id);
        if (!detalle.exec()) {
            setError(detalle.lastError().text());
            conexionQt.rollback();
            return false;
        }
        while (detalle.next()) {
            DetalleStock item{detalle.value(0).toInt(), detalle.value(1).toString(), detalle.value(2).toInt(), detalle.value(3).toInt()};
            if (item.stock < item.cantidad) {
                setError("No se puede anular: el producto \"" + item.nombre + "\" quedaria con stock negativo.");
                conexionQt.rollback();
                return false;
            }
            detalles.append(item);
        }

        for (const DetalleStock &item : detalles) {
            QSqlQuery stock(conexionQt);
            stock.prepare("UPDATE productos SET stock = stock - ? WHERE id_producto = ?");
            stock.addBindValue(item.cantidad);
            stock.addBindValue(item.idProducto);
            if (!stock.exec()) {
                setError(stock.lastError().text());
                conexionQt.rollback();
                return false;
            }
        }

        QSqlQuery update(conexionQt);
        update.prepare("UPDATE compras_proveedores SET estado='Anulada', motivo_anulacion=?, fecha_anulacion=NOW() WHERE id_compra=? AND estado <> 'Anulada'");
        update.addBindValue(motivo);
        update.addBindValue(id);
        if (!update.exec() || update.numRowsAffected() == 0) {
            setError(update.lastError().isValid() ? update.lastError().text() : "No se pudo anular la compra.");
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
