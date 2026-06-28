#ifndef PROVEEDOR_H
#define PROVEEDOR_H

#include <string>
using namespace std;

class Proveedor {
private:
    int idProveedor;
    string nombre;
    string telefono;
    string email;
    string direccion;
    string cuit;

public:
    Proveedor() {}

    Proveedor(int idProveedor, string nombre, string telefono, string email, string direccion, string cuit) {
        this->idProveedor = idProveedor;
        this->nombre = nombre;
        this->telefono = telefono;
        this->email = email;
        this->direccion = direccion;
        this->cuit = cuit;
    }

    int getIdProveedor() { return idProveedor; }
    string getNombre() { return nombre; }
    string getTelefono() { return telefono; }
    string getEmail() { return email; }
    string getDireccion() { return direccion; }
    string getCuit() { return cuit; }

    void setIdProveedor(int idProveedor) { this->idProveedor = idProveedor; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setTelefono(string telefono) { this->telefono = telefono; }
    void setEmail(string email) { this->email = email; }
    void setDireccion(string direccion) { this->direccion = direccion; }
    void setCuit(string cuit) { this->cuit = cuit; }
};

#endif