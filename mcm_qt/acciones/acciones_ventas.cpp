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
    if (!ventaDao.agregarQt(ventaModelo)) {
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

void MainWindow::showVentaDetails()
{
    const int id = selectedId(ventasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalles de venta", "Selecciona una venta de la tabla.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Detalles de venta " + QString::number(id));
    dialog.resize(760, 420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTableWidget *table = new QTableWidget(&dialog);
    layout->addWidget(table);

    DetalleVentaDAO dao(conexionBD.getConexionQt());
    fillTable(table, {"Producto", "Cantidad", "Precio unitario", "Subtotal"}, dao.listarPorVentaQt(id));

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}



