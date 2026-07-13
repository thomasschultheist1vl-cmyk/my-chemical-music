#include "../ventana_principal.h"
#include "ui_ventana_principal.h"
#include "../../dao/CategoriaDAO.h"
#include "../../dao/ClienteDAO.h"
#include "../../dao/CompraProveedorDAO.h"
#include "../../dao/DetalleCompraDAO.h"
#include "../../dao/DetalleVentaDAO.h"
#include "../../dao/EstadoServicioDAO.h"
#include "../../dao/FacturaDAO.h"
#include "../../dao/MarcaDAO.h"
#include "../../dao/MedioPagoDAO.h"
#include "../../dao/ProductoDAO.h"
#include "../../dao/ServicioDAO.h"
#include "../../dao/VentaDAO.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QDate>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStackedWidget>
#include <QSpinBox>
#include <QTabBar>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVariant>
#include <QVector>
#include <QVBoxLayout>

#include <algorithm>

void MainWindow::addProducto()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre, descripcion;
    double precioVenta = 0;
    int stock = 0;
    int stockMinimo = 0;
    int idCategoria = -1;
    int idMarca = -1;
    int idProveedor = -1;

    if (!showProductoDialog(nombre, descripcion, precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor, "Agregar producto")) {
        return;
    }

    Producto producto(0, nombre.toStdString(), descripcion.toStdString(), precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor);
    ProductoDAO dao(conexionBD.getConexionQt());
    if (!dao.agregarQt(producto)) {
        QMessageBox::warning(this, "Productos", "No se pudo agregar el producto.");
        return;
    }

    loadProductos();
    actualizarDashboard();
}

void MainWindow::editProducto()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(productosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar producto", "Selecciona un producto de la tabla.");
        return;
    }

    ProductoDAO dao(conexionBD.getConexionQt());
    QSqlQuery load = dao.buscarQt(id);
    if (!load.next()) {
        QMessageBox::warning(this, "Productos", "No se pudo cargar el producto seleccionado.");
        return;
    }

    QString nombre = load.value(0).toString();
    QString descripcion = load.value(1).toString();
    double precioVenta = load.value(2).toDouble();
    int stock = load.value(3).toInt();
    int stockMinimo = load.value(4).toInt();
    int idCategoria = load.value(5).toInt();
    int idMarca = load.value(6).toInt();
    int idProveedor = load.value(7).toInt();

    if (!showProductoDialog(nombre, descripcion, precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor, "Modificar producto")) {
        return;
    }

    Producto producto(id, nombre.toStdString(), descripcion.toStdString(), precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor);
    if (!dao.modificarQt(producto)) {
        QMessageBox::warning(this, "Productos", "No se pudo modificar el producto.");
        return;
    }

    loadProductos();
}

void MainWindow::deleteProducto()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(productosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar producto", "Selecciona un producto de la tabla.");
        return;
    }

    const QString nombre = productosTable->item(productosTable->currentRow(), 1)->text();
    if (QMessageBox::question(this, "Eliminar producto", "Seguro que queres eliminar \"" + nombre + "\"?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    ProductoDAO dao(conexionBD.getConexionQt());
    if (!dao.eliminarQt(id)) {
        QMessageBox::warning(this, "Productos", "No se pudo eliminar el producto.");
        return;
    }

    loadProductos();
}



