# My Chemical Music

Sistema de gestion de escritorio para una casa de musica. Permite administrar clientes, productos, proveedores, ventas, compras, servicios de reparacion/arreglo de instrumentos y facturacion.

El proyecto tiene dos versiones:

- Version consola en C++.
- Version grafica en Qt Widgets.

## Tecnologias usadas

- C++
- Qt 6.11.1 Widgets
- MinGW
- MySQL con XAMPP
- ODBC
- phpMyAdmin
- Git y GitHub

## Base de datos

Base de datos: `my_chemical_music`

Tablas principales:

- `clientes`
- `productos`
- `categorias`
- `marcas`
- `proveedores`
- `ventas`
- `detalle_ventas`
- `facturas`
- `servicios`
- `estados_servicio`
- `medios_pago`
- `compras_proveedores`
- `detalle_compras`

Para la version consola se uso el DSN ODBC 32 bits:

```txt
my_chemical_music32
```

Para la version Qt se uso el DSN ODBC 64 bits:

```txt
my_chemical_music64
```

Usuario MySQL:

```txt
root
```

Contrasena:

```txt
vacia
```

## Estructura del proyecto

```txt
mcm/
├── dao/
├── database/
├── menus/
├── models/
├── mcm_qt/
├── main.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Carpetas importantes

`models/`

Contiene las clases del sistema, por ejemplo `Cliente`, `Producto`, `Venta`, `Servicio`, `Factura`, etc.

`dao/`

Contiene las clases DAO que se encargan de trabajar con la base de datos desde la version consola.

`database/`

Contiene la conexion a la base de datos por ODBC.

`menus/`

Contiene los menus separados de la version consola.

`mcm_qt/`

Contiene la interfaz grafica hecha con Qt.

## Version consola

Archivo principal:

```txt
main.cpp
```

Compilacion:

```powershell
C:\MinGW\bin\g++.exe main.cpp menus\*.cpp -o mcm.exe -lodbc32
```

## Version Qt

Para abrir el proyecto en Qt Creator, abrir este archivo:

```txt
C:\Users\Admin\OneDrive\Desktop\mcm\CMakeLists.txt
```

Archivos principales de Qt:

- `mcm_qt/main_qt.cpp`
- `mcm_qt/mainwindow.h`
- `mcm_qt/mainwindow.cpp`
- `mcm_qt/mainwindow.ui`

La interfaz grafica permite:

- Gestionar clientes.
- Gestionar productos.
- Registrar ventas y descontar stock.
- Registrar compras a proveedores y aumentar stock.
- Gestionar servicios/arreglos de instrumentos.
- Cambiar el estado de un servicio.
- Generar facturas a partir de ventas.
- Administrar configuracion general: categorias, marcas, medios de pago y estados de servicio.

## Estado del proyecto

El sistema esta preparado como aplicacion de escritorio para presentar en clase. La version consola mantiene la logica original y la version Qt agrega una interfaz grafica con la misma gestion principal.
