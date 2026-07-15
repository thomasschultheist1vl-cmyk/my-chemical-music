# My Chemical Music

Sistema de escritorio para una casa de musica.

El proyecto permite administrar clientes, productos, ventas, servicios/arreglos, compras a proveedores, facturacion, datos generales y usuarios del sistema.

Hay dos partes:

- version original por consola;
- version grafica hecha con Qt Widgets.

La version principal para usar en la entrega es la de Qt.

## Tecnologias

- C++
- Qt 6.11.1 Widgets
- MySQL / MariaDB
- XAMPP
- ODBC
- CMake
- Ninja

## Base de datos

La base se llama:

```txt
my_chemical_music
```

El archivo actualizado para importar la base esta en:

```txt
database/my_chemical_music.sql
```

Tablas principales:

- clientes
- productos
- categorias
- marcas
- proveedores
- ventas
- detalle_ventas
- servicios
- estados_servicio
- compras_proveedores
- detalle_compras
- facturas
- medios_pago
- roles
- usuarios

La base ya incluye campos para:

- anulaciones;
- usuario que registra operaciones;
- usuario que anula operaciones;
- estado de pago de facturas;
- usuario que registra el pago.

## Conexion

Para la version Qt se usa ODBC de 64 bits.

DSN local usado durante el desarrollo:

```txt
my_chemical_music64
```

Usuario local:

```txt
root
```

Contrasena local:

```txt
vacia
```

Tambien se puede usar un archivo de configuracion en la carpeta del ejecutable:

```txt
MCM_EXE/conexion.ini
```

Ese archivo no se sube al repositorio porque puede tener datos privados.

Hay un ejemplo en:

```txt
MCM_EXE/conexion.ini.example
```

## Cuenta inicial

La base tiene una cuenta inicial de Supervisor para entrar al sistema:

```txt
usuario: supervisor
contrasena: 1234
```

La contrasena no se guarda en texto plano. Se guarda con SHA-256.

## Estructura del proyecto

```txt
dao/        consultas y operaciones contra la base de datos
database/   conexion ODBC y archivo SQL de la base
menus/      version original por consola
models/     clases del sistema
mcm_qt/     interfaz grafica en Qt
MCM_EXE/    ejecutable final y DLLs necesarias para correrlo
```

## Archivos importantes de Qt

Entrada de la aplicacion:

```txt
mcm_qt/inicio_aplicacion.cpp
```

Login:

```txt
mcm_qt/login_dialog.h
mcm_qt/login_dialog.cpp
```

Ventana principal:

```txt
mcm_qt/ventana_principal.h
mcm_qt/ventana_principal_interfaz.cpp
mcm_qt/ventana_principal_datos.cpp
mcm_qt/ventana_principal.ui
```

Acciones de botones:

```txt
mcm_qt/acciones/
```

Formularios:

```txt
mcm_qt/formularios/
```

Utilidades:

```txt
mcm_qt/utilidades/
```

## DAO principales

Los DAO estan en la carpeta:

```txt
dao/
```

Algunos de los mas importantes:

- ClienteDAO
- ProductoDAO
- VentaDAO
- ServicioDAO
- CompraProveedorDAO
- FacturaDAO
- UsuarioDAO
- CategoriaDAO
- MarcaDAO
- MedioPagoDAO
- EstadoServicioDAO

La idea es que la interfaz Qt no haga todo directamente contra la base, sino que use los DAO para listar, agregar, modificar, anular y consultar datos.

## Roles del sistema

El sistema maneja tres roles:

- Supervisor
- Vendedor
- Encargado de compras

El Supervisor puede ver y administrar todo.

El Vendedor trabaja con clientes, ventas, servicios, facturacion y consulta de productos.

El Encargado de compras trabaja principalmente con productos, stock y compras a proveedores.

## Ejecutable

El ejecutable final esta en:

```txt
MCM_EXE/mcm_qt.exe
```

Esa carpeta tambien contiene las DLLs y plugins de Qt necesarios para abrir el programa fuera de Qt Creator.

## Compilar desde Qt Creator

Abrir este archivo:

```txt
CMakeLists.txt
```

Elegir el kit de Qt 6.11.1 de 64 bits y compilar en Release.

## Compilar desde PowerShell

Ejemplo usado durante el desarrollo:

```powershell
C:\Qt\Tools\CMake_64\bin\cmake.exe -S C:\xampp\htdocs\my-chemical-music -B C:\xampp\htdocs\my-chemical-music\build\mcm_qt_release_deploy -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:\Qt\6.11.1\llvm-mingw_64 -DCMAKE_MAKE_PROGRAM=C:\Qt\Tools\Ninja\ninja.exe -DCMAKE_C_COMPILER=C:\Qt\Tools\llvm-mingw1706_64\bin\gcc.exe -DCMAKE_CXX_COMPILER=C:\Qt\Tools\llvm-mingw1706_64\bin\g++.exe

C:\Qt\Tools\Ninja\ninja.exe -C C:\xampp\htdocs\my-chemical-music\build\mcm_qt_release_deploy
```

Despues de compilar, el ejecutable se copia a:

```txt
MCM_EXE/mcm_qt.exe
```

## Version consola

La version de consola se conserva como parte del proyecto original.

Archivo principal:

```txt
main.cpp
```

Menus:

```txt
menus/
```

Esta parte sirve para mostrar de donde salio la logica inicial del sistema.

## Estado actual

La version Qt ya tiene:

- login;
- usuarios;
- roles;
- permisos por rol;
- altas, modificaciones y eliminaciones segun permisos;
- anulaciones;
- facturacion;
- estado de pago de facturas;
- detalles de registros;
- dashboard de inicio;
- conexion ODBC reutilizada.
