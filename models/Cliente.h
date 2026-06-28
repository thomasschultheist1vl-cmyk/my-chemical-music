#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
using namespace std;

class Cliente {
private:
    int idCliente;
    string nombre;
    string apellido;
    string telefono;
    string email;
    string direccion;
    string dni;

public:
    Cliente() {}

    Cliente(int idCliente, string nombre, string apellido, string telefono, string email, string direccion, string dni) {
        this->idCliente = idCliente;
        this->nombre = nombre;
        this->apellido = apellido;
        this->telefono = telefono;
        this->email = email;
        this->direccion = direccion;
        this->dni = dni;
    }

    int getIdCliente() { return idCliente; }
    string getNombre() { return nombre; }
    string getApellido() { return apellido; }
    string getTelefono() { return telefono; }
    string getEmail() { return email; }
    string getDireccion() { return direccion; }
    string getDni() { return dni; }

    void setIdCliente(int idCliente) { this->idCliente = idCliente; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setApellido(string apellido) { this->apellido = apellido; }
    void setTelefono(string telefono) { this->telefono = telefono; }
    void setEmail(string email) { this->email = email; }
    void setDireccion(string direccion) { this->direccion = direccion; }
    void setDni(string dni) { this->dni = dni; }
};

#endif