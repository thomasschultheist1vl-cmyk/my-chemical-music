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

void MainWindow::addVenta()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Ventas", "No hay conexion con la base de datos.");
        return;
    }

    int idCliente = -1;
    int idMedioPago = -1;
    QString fecha;

    if (!showVentaHeaderDialog(idCliente, idMedioPago, fecha)) {
        return;
    }

    if (!conexionBD.iniciarTransaccionQt()) {
        QMessageBox::warning(this, "Ventas", "No se pudo iniciar la venta:\n" + conexionBD.getConexionQt().lastError().text());
        return;
    }

    VentaDAO ventaDao(conexionBD.getConexionQt());
    DetalleVentaDAO detalleDao(conexionBD.getConexionQt());
    ProductoDAO productoDao(conexionBD.getConexionQt());
    Venta ventaModelo(0, idCliente, fecha.toStdString(), 0, idMedioPago);
    if (!ventaDao.agregarQt(ventaModelo, usuarioActual.idUsuario)) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::warning(this, "Ventas", "No se pudo registrar la venta.");
        return;
    }

    const int idVenta = ventaDao.obtenerUltimoIdQt();

    double totalVenta = 0;
    int detalles = 0;

    while (true) {
        int idProducto = -1;
        int cantidad = 1;
        if (!showVentaDetalleDialog(idProducto, cantidad)) {
            break;
        }

        QSqlQuery producto = productoDao.precioStockQt(idProducto);
        if (!producto.next()) {
            conexionBD.cancelarTransaccionQt();
            QMessageBox::warning(this, "Ventas", "No se pudo cargar el producto seleccionado.");
            return;
        }

        const double precioUnitario = producto.value(0).toDouble();
        const int stockActual = producto.value(1).toInt();
        if (stockActual < cantidad) {
            QMessageBox::warning(this, "Ventas", "No hay stock suficiente para ese producto.");
            continue;
        }

        const double subtotal = precioUnitario * cantidad;

        DetalleVenta detalleModelo(0, idVenta, idProducto, cantidad, precioUnitario, subtotal);
        if (!detalleDao.agregarQt(detalleModelo) || !productoDao.ajustarStockQt(idProducto, -cantidad)) {
            conexionBD.cancelarTransaccionQt();
            QMessageBox::warning(this, "Ventas", "No se pudo agregar el detalle de venta.");
            return;
        }

        totalVenta += subtotal;
        ++detalles;

        if (QMessageBox::question(this, "Venta", "Producto agregado. Queres agregar otro producto?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            break;
        }
    }

    if (detalles == 0) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::information(this, "Ventas", "La venta se cancelo porque no agregaste productos.");
        return;
    }

    if (!ventaDao.actualizarTotalQt(idVenta, totalVenta) || !conexionBD.confirmarTransaccionQt()) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::warning(this, "Ventas", "No se pudo finalizar la venta.");
        return;
    }

    QMessageBox::information(this, "Ventas", "Venta registrada.\nID venta: " + QString::number(idVenta) + "\nTotal: $" + QString::number(totalVenta, 'f', 2));
    loadVentas();
    loadProductos();
    actualizarDashboard();
}

void MainWindow::anularVenta()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Ventas", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(ventasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Anular venta", "Selecciona una venta de la tabla.");
        return;
    }

    if (selectedRowIsAnulada(ventasTable)) {
        QMessageBox::information(this, "Anular venta", "La venta seleccionada ya esta anulada.");
        return;
    }
    if (usuarioActual.esVendedor() && !VentaDAO(conexionBD.getConexionQt()).perteneceAUsuarioOSupervisorQt(id, usuarioActual.idUsuario)) {
        QMessageBox::warning(this, "Ventas", "No podes anular una venta de otro usuario.");
        return;
    }

    QString motivo;
    if (!requestAnulacionMotivo("Anular venta", "Desea anular la venta N.º " + QString::number(id) + "?\n\nMotivo de anulacion:", motivo)) {
        return;
    }

    VentaDAO dao(conexionBD.getConexionQt());
    QString error;
    if (!dao.anularVentaQt(id, motivo, usuarioActual.idUsuario, &error)) {
        QMessageBox::warning(this, "Ventas", "No se pudo anular la venta:\n" + error);
        return;
    }

    QMessageBox::information(this, "Ventas", "Venta anulada correctamente.");
    loadVentas();
    loadProductos();
    actualizarDashboard();
}



