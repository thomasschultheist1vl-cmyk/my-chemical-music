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

void MainWindow::addCompra()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Compras", "No hay conexion con la base de datos.");
        return;
    }

    int idProveedor = -1;
    QString fecha;

    if (!showCompraHeaderDialog(idProveedor, fecha)) {
        return;
    }

    if (!conexionBD.iniciarTransaccionQt()) {
        QMessageBox::warning(this, "Compras", "No se pudo iniciar la compra:\n" + conexionBD.getConexionQt().lastError().text());
        return;
    }

    CompraProveedorDAO compraDao(conexionBD.getConexionQt());
    DetalleCompraDAO detalleDao(conexionBD.getConexionQt());
    ProductoDAO productoDao(conexionBD.getConexionQt());
    CompraProveedor compraModelo(0, idProveedor, fecha.toStdString(), 0);
    if (!compraDao.agregarQt(compraModelo)) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::warning(this, "Compras", "No se pudo registrar la compra.");
        return;
    }

    const int idCompra = compraDao.obtenerUltimoIdQt();

    double totalCompra = 0;
    int detalles = 0;

    while (true) {
        int idProducto = -1;
        int cantidad = 1;
        double precioCompra = 0;
        if (!showCompraDetalleDialog(idProducto, cantidad, precioCompra)) {
            break;
        }

        const double subtotal = precioCompra * cantidad;

        DetalleCompra detalleModelo(0, idCompra, idProducto, cantidad, precioCompra, subtotal);
        if (!detalleDao.agregarQt(detalleModelo) || !productoDao.ajustarStockQt(idProducto, cantidad)) {
            conexionBD.cancelarTransaccionQt();
            QMessageBox::warning(this, "Compras", "No se pudo agregar el detalle de compra.");
            return;
        }

        totalCompra += subtotal;
        ++detalles;

        if (QMessageBox::question(this, "Compra", "Producto agregado. Queres agregar otro producto?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            break;
        }
    }

    if (detalles == 0) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::information(this, "Compras", "La compra se cancelo porque no agregaste productos.");
        return;
    }

    if (!compraDao.actualizarTotalQt(idCompra, totalCompra) || !conexionBD.confirmarTransaccionQt()) {
        conexionBD.cancelarTransaccionQt();
        QMessageBox::warning(this, "Compras", "No se pudo finalizar la compra.");
        return;
    }

    QMessageBox::information(this, "Compras", "Compra registrada.\nID compra: " + QString::number(idCompra) + "\nTotal: $" + QString::number(totalCompra, 'f', 2));
    loadCompras();
    loadProductos();
    actualizarDashboard();
}

void MainWindow::deleteCompra()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Compras", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(comprasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar compra", "Selecciona una compra de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar compra", "Seguro que queres eliminar la compra seleccionada?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    CompraProveedorDAO dao(conexionBD.getConexionQt());
    if (!dao.eliminarQt(id)) {
        QMessageBox::warning(this, "Compras", "No se pudo eliminar la compra.");
        return;
    }

    loadCompras();
}

void MainWindow::anularCompra()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Compras", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(comprasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Anular compra", "Selecciona una compra de la tabla.");
        return;
    }

    if (selectedRowIsAnulada(comprasTable)) {
        QMessageBox::information(this, "Anular compra", "La compra seleccionada ya esta anulada.");
        return;
    }

    QString motivo;
    if (!requestAnulacionMotivo("Anular compra", "Desea anular la compra N.º " + QString::number(id) + "?\n\nMotivo de anulacion:", motivo)) {
        return;
    }

    CompraProveedorDAO dao(conexionBD.getConexionQt());
    QString error;
    if (!dao.anularCompraQt(id, motivo, &error)) {
        QMessageBox::warning(this, "Compras", "No se pudo anular la compra:\n" + error);
        return;
    }

    QMessageBox::information(this, "Compras", "Compra anulada correctamente.");
    loadCompras();
    loadProductos();
    actualizarDashboard();
}



