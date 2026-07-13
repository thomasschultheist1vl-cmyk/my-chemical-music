cmake_minimum_required(VERSION 3.16)

project(my_chemical_music VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Archivos de la version consola: se muestran en Qt Creator, pero no se compilan
# dentro del ejecutable grafico para evitar otro main().
set_source_files_properties(
    main.cpp
    menus/MenuClientes.cpp
    menus/MenuComprasProveedores.cpp
    menus/MenuConfiguracion.cpp
    menus/MenuFacturacion.cpp
    menus/MenuProductos.cpp
    menus/MenuProveedores.cpp
    menus/MenuServicios.cpp
    menus/MenuVentas.cpp
    PROPERTIES HEADER_FILE_ONLY TRUE
)

set(PROJECT_SOURCES
    mcm_qt/inicio_aplicacion.cpp
    mcm_qt/ventana_principal_interfaz.cpp
    mcm_qt/ventana_principal_datos.cpp
    mcm_qt/acciones/acciones_clientes.cpp
    mcm_qt/acciones/acciones_productos.cpp
    mcm_qt/acciones/acciones_ventas.cpp
    mcm_qt/acciones/acciones_servicios.cpp
    mcm_qt/acciones/acciones_compras.cpp
    mcm_qt/acciones/acciones_facturacion.cpp
    mcm_qt/acciones/acciones_datos_generales.cpp
    mcm_qt/formularios/formularios_clientes.cpp
    mcm_qt/formularios/formularios_productos.cpp
    mcm_qt/formularios/formularios_ventas.cpp
    mcm_qt/formularios/formularios_servicios.cpp
    mcm_qt/formularios/formularios_compras.cpp
    mcm_qt/formularios/formularios_facturacion.cpp
    mcm_qt/formularios/formularios_datos_generales.cpp
    mcm_qt/utilidades/utilidades_seleccion.cpp
    mcm_qt/utilidades/utilidades_fecha.h
    mcm_qt/ventana_principal.h
    mcm_qt/ventana_principal.ui

    main.cpp

    dao/CategoriaDAO.h
    dao/ClienteDAO.h
    dao/CompraProveedorDAO.h
    dao/DetalleCompraDAO.h
    dao/DetalleVentaDAO.h
    dao/EstadoServicioDAO.h
    dao/FacturaDAO.h
    dao/MarcaDAO.h
    dao/MedioPagoDAO.h
    dao/ProductoDAO.h
    dao/ProveedorDAO.h
    dao/ServicioDAO.h
    dao/VentaDAO.h

    database/ConexionBD.h
    database/my_chemical_music.sql

    menus/MenuClientes.cpp
    menus/MenuClientes.h
    menus/MenuComprasProveedores.cpp
    menus/MenuComprasProveedores.h
    menus/MenuConfiguracion.cpp
    menus/MenuConfiguracion.h
    menus/MenuFacturacion.cpp
    menus/MenuFacturacion.h
    menus/MenuProductos.cpp
    menus/MenuProductos.h
    menus/MenuProveedores.cpp
    menus/MenuProveedores.h
    menus/MenuServicios.cpp
    menus/MenuServicios.h
    menus/MenuVentas.cpp
    menus/MenuVentas.h
    menus/Menus.h

    models/Categoria.h
    models/Cliente.h
    models/CompraProveedor.h
    models/DetalleCompra.h
    models/DetalleVenta.h
    models/EstadoServicio.h
    models/Factura.h
    models/Marca.h
    models/MedioPago.h
    models/Producto.h
    models/Proveedor.h
    models/Servicio.h
    models/Venta.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mcm_qt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(mcm_qt
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(mcm_qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
target_compile_definitions(mcm_qt PRIVATE MCM_QT_APP)
set_target_properties(mcm_qt PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/mcm_qt"
)

set_target_properties(mcm_qt PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mcm_qt)
endif()
