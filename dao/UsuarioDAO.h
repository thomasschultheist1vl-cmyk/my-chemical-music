#ifndef USUARIODAO_H
#define USUARIODAO_H

#include "../models/Usuario.h"

#ifdef MCM_QT_APP
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#endif

class UsuarioDAO {
public:
#ifdef MCM_QT_APP
    explicit UsuarioDAO(const QSqlDatabase &conexion) : conexionQt(conexion) {}

    bool validarLoginQt(const QString &nombreUsuario, const QString &passwordHash, Usuario &usuario, QString *error = nullptr) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT u.id_usuario, u.id_rol, r.nombre, u.nombre, u.apellido, "
            "u.nombre_usuario, u.activo, DATE_FORMAT(u.fecha_creacion, '%d/%m/%Y %H:%i') "
            "FROM usuarios u "
            "INNER JOIN roles r ON u.id_rol = r.id_rol "
            "WHERE u.nombre_usuario = ? AND u.password_hash = ?"
        );
        q.addBindValue(nombreUsuario);
        q.addBindValue(passwordHash);
        if (!q.exec()) {
            if (error) *error = q.lastError().text();
            return false;
        }
        if (!q.next()) {
            if (error) *error = "Usuario o contraseña incorrectos";
            return false;
        }
        if (!q.value(6).toBool()) {
            if (error) *error = "Esta cuenta se encuentra desactivada";
            return false;
        }
        usuario = desdeQuery(q);
        return true;
    }

    QSqlQuery listarUsuariosQt() const {
        QSqlQuery q(conexionQt);
        q.exec(
            "SELECT u.id_usuario, u.nombre, u.apellido, u.nombre_usuario, r.nombre, "
            "CASE WHEN u.activo = 1 THEN 'Activo' ELSE 'Inactivo' END, "
            "DATE_FORMAT(u.fecha_creacion, '%d/%m/%Y %H:%i') "
            "FROM usuarios u INNER JOIN roles r ON u.id_rol = r.id_rol "
            "ORDER BY u.id_usuario"
        );
        return q;
    }

    QSqlQuery rolesQt() const {
        QSqlQuery q(conexionQt);
        q.exec("SELECT id_rol, nombre FROM roles ORDER BY nombre");
        return q;
    }

    QSqlQuery obtenerUsuarioQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT u.id_usuario, u.id_rol, r.nombre, u.nombre, u.apellido, "
            "u.nombre_usuario, u.activo, DATE_FORMAT(u.fecha_creacion, '%d/%m/%Y %H:%i') "
            "FROM usuarios u INNER JOIN roles r ON u.id_rol = r.id_rol "
            "WHERE u.id_usuario = ?"
        );
        q.addBindValue(id);
        q.exec();
        return q;
    }

    QSqlQuery detalleUsuarioQt(int id) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT u.id_usuario, u.nombre, u.apellido, u.nombre_usuario, r.nombre, "
            "CASE WHEN u.activo = 1 THEN 'Activo' ELSE 'Inactivo' END, "
            "DATE_FORMAT(u.fecha_creacion, '%d/%m/%Y %H:%i') "
            "FROM usuarios u INNER JOIN roles r ON u.id_rol = r.id_rol "
            "WHERE u.id_usuario = ?"
        );
        q.addBindValue(id);
        q.exec();
        return q;
    }

    bool existeNombreUsuarioQt(const QString &nombreUsuario, int ignorarId = 0) const {
        QSqlQuery q(conexionQt);
        q.prepare("SELECT COUNT(*) FROM usuarios WHERE nombre_usuario = ? AND id_usuario <> ?");
        q.addBindValue(nombreUsuario);
        q.addBindValue(ignorarId);
        return q.exec() && q.next() && q.value(0).toInt() > 0;
    }

    bool agregarUsuarioQt(const Usuario &u, const QString &passwordHash) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "INSERT INTO usuarios (id_rol, nombre, apellido, nombre_usuario, password_hash, activo) "
            "VALUES (?, ?, ?, ?, ?, ?)"
        );
        q.addBindValue(u.idRol);
        q.addBindValue(u.nombre);
        q.addBindValue(u.apellido);
        q.addBindValue(u.nombreUsuario);
        q.addBindValue(passwordHash);
        q.addBindValue(u.activo ? 1 : 0);
        return q.exec();
    }

    bool modificarUsuarioQt(const Usuario &u) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "UPDATE usuarios SET id_rol=?, nombre=?, apellido=?, nombre_usuario=?, activo=? "
            "WHERE id_usuario=?"
        );
        q.addBindValue(u.idRol);
        q.addBindValue(u.nombre);
        q.addBindValue(u.apellido);
        q.addBindValue(u.nombreUsuario);
        q.addBindValue(u.activo ? 1 : 0);
        q.addBindValue(u.idUsuario);
        return q.exec();
    }

    bool cambiarPasswordQt(int idUsuario, const QString &passwordHash) const {
        QSqlQuery q(conexionQt);
        q.prepare("UPDATE usuarios SET password_hash=? WHERE id_usuario=?");
        q.addBindValue(passwordHash);
        q.addBindValue(idUsuario);
        return q.exec();
    }

    bool activarUsuarioQt(int idUsuario, bool activo) const {
        QSqlQuery q(conexionQt);
        q.prepare("UPDATE usuarios SET activo=? WHERE id_usuario=?");
        q.addBindValue(activo ? 1 : 0);
        q.addBindValue(idUsuario);
        return q.exec();
    }

    int contarSupervisoresActivosQt(int ignorarId = 0) const {
        QSqlQuery q(conexionQt);
        q.prepare(
            "SELECT COUNT(*) FROM usuarios u "
            "INNER JOIN roles r ON u.id_rol = r.id_rol "
            "WHERE r.nombre = 'Supervisor' AND u.activo = 1 AND u.id_usuario <> ?"
        );
        q.addBindValue(ignorarId);
        return q.exec() && q.next() ? q.value(0).toInt() : 0;
    }

private:
    QSqlDatabase conexionQt;

    static Usuario desdeQuery(const QSqlQuery &q) {
        Usuario u;
        u.idUsuario = q.value(0).toInt();
        u.idRol = q.value(1).toInt();
        u.nombreRol = q.value(2).toString();
        u.nombre = q.value(3).toString();
        u.apellido = q.value(4).toString();
        u.nombreUsuario = q.value(5).toString();
        u.activo = q.value(6).toBool();
        u.fechaCreacion = q.value(7).toString();
        return u;
    }
#endif
};

#endif // USUARIODAO_H
