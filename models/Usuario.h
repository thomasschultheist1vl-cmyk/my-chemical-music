#ifndef USUARIO_H
#define USUARIO_H

#include <QString>

class Usuario {
public:
    int idUsuario = 0;
    int idRol = 0;
    QString nombreRol;
    QString nombre;
    QString apellido;
    QString nombreUsuario;
    bool activo = false;
    QString fechaCreacion;

    QString nombreCompleto() const {
        return (nombre + " " + apellido).trimmed();
    }

    bool esSupervisor() const { return nombreRol == "Supervisor"; }
    bool esVendedor() const { return nombreRol == "Vendedor"; }
    bool esEncargadoCompras() const { return nombreRol == "Encargado de compras"; }
    bool estaAutenticado() const { return idUsuario > 0 && activo; }
};

#endif // USUARIO_H
