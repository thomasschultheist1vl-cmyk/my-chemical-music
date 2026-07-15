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

void MainWindow::addFactura()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }

    int idVenta = -1;
    int idServicio = -1;
    QString numeroFactura, tipoFactura, fecha;
    double total = 0;

    if (!showFacturaDialog(idVenta, idServicio, numeroFactura, tipoFactura, fecha, total)) {
        return;
    }

    Factura factura(0, idVenta, idServicio, numeroFactura.toStdString(), tipoFactura.toStdString(), fecha.toStdString(), total);
    FacturaDAO dao(conexionBD.getConexionQt());
    if (!dao.agregarQt(factura, usuarioActual.idUsuario)) {
        QMessageBox::warning(this, "Facturacion", "No se pudo generar la factura.");
        return;
    }

    loadFacturas();
    actualizarDashboard();
}

void MainWindow::deleteFactura()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(facturasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar factura", "Selecciona una factura de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar factura", "Seguro que queres eliminar la factura seleccionada?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    FacturaDAO dao(conexionBD.getConexionQt());
    if (!dao.eliminarQt(id)) {
        QMessageBox::warning(this, "Facturacion", "No se pudo eliminar la factura.");
        return;
    }

    loadFacturas();
}

void MainWindow::anularFactura()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(facturasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Anular factura", "Selecciona una factura de la tabla.");
        return;
    }

    if (selectedRowIsAnulada(facturasTable)) {
        QMessageBox::information(this, "Anular factura", "La factura seleccionada ya esta anulada.");
        return;
    }
    if (usuarioActual.esVendedor() && !FacturaDAO(conexionBD.getConexionQt()).perteneceAUsuarioOSupervisorQt(id, usuarioActual.idUsuario)) {
        QMessageBox::warning(this, "Facturacion", "No podes anular una factura de otro usuario.");
        return;
    }

    QString motivo;
    if (!requestAnulacionMotivo("Anular factura", "Desea anular la factura N.º " + QString::number(id) + "?\n\nMotivo de anulacion:", motivo)) {
        return;
    }

    FacturaDAO dao(conexionBD.getConexionQt());
    QString error;
    if (!dao.anularFacturaQt(id, motivo, usuarioActual.idUsuario, &error)) {
        QMessageBox::warning(this, "Facturacion", "No se pudo anular la factura:\n" + error);
        return;
    }

    QMessageBox::information(this, "Facturacion", "Factura anulada correctamente.");
    loadFacturas();
    actualizarDashboard();
}

void MainWindow::marcarFacturaPagada()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }
    if (usuarioActual.esEncargadoCompras()) {
        QMessageBox::warning(this, "Facturacion", "Tu rol no puede marcar facturas como pagadas.");
        return;
    }

    const int id = selectedId(facturasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Marcar como pagada", "Selecciona una factura de la tabla.");
        return;
    }
    if (selectedRowIsAnulada(facturasTable)) {
        QMessageBox::information(this, "Marcar como pagada", "No se puede modificar el pago de una factura anulada.");
        return;
    }
    const int pagoCol = columnIndex(facturasTable, "Pago");
    if (pagoCol >= 0 && facturasTable->item(facturasTable->currentRow(), pagoCol)
        && facturasTable->item(facturasTable->currentRow(), pagoCol)->text() == "Pagada") {
        QMessageBox::information(this, "Marcar como pagada", "La factura ya esta pagada.");
        return;
    }
    FacturaDAO dao(conexionBD.getConexionQt());
    if (usuarioActual.esVendedor() && !dao.perteneceAUsuarioOSupervisorQt(id, usuarioActual.idUsuario)) {
        QMessageBox::warning(this, "Facturacion", "No podes marcar como pagada una factura de otro usuario.");
        return;
    }
    if (QMessageBox::question(this, "Marcar como pagada", "Confirmas marcar la factura N.º " + QString::number(id) + " como pagada?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QString error;
    if (!dao.marcarPagadaQt(id, usuarioActual.idUsuario, &error)) {
        QMessageBox::warning(this, "Facturacion", "No se pudo marcar como pagada:\n" + error);
        return;
    }
    QMessageBox::information(this, "Facturacion", "Factura marcada como pagada.");
    loadFacturas();
}



