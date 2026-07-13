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

void MainWindow::addServicio()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    int idCliente = -1;
    QString instrumento, descripcion, fechaIngreso, fechaEntrega;
    double precio = 0;
    int idEstadoServicio = -1;

    if (!showServicioDialog(idCliente, instrumento, descripcion, fechaIngreso, fechaEntrega, precio, idEstadoServicio, "Registrar servicio")) {
        return;
    }

    Servicio servicio(0, idCliente, instrumento.toStdString(), descripcion.toStdString(), fechaIngreso.toStdString(), fechaEntrega.toStdString(), precio, idEstadoServicio);
    ServicioDAO dao(conexionBD.getConexionQt());
    if (!dao.agregarQt(servicio)) {
        QMessageBox::warning(this, "Servicios", "No se pudo registrar el servicio.");
        return;
    }

    loadServicios();
    actualizarDashboard();
}

void MainWindow::editServicio()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar servicio", "Selecciona un servicio de la tabla.");
        return;
    }

    ServicioDAO dao(conexionBD.getConexionQt());
    QSqlQuery load = dao.buscarQt(id);
    if (!load.next()) {
        QMessageBox::warning(this, "Servicios", "No se pudo cargar el servicio seleccionado.");
        return;
    }

    int idCliente = load.value(0).toInt();
    QString instrumento = load.value(1).toString();
    QString descripcion = load.value(2).toString();
    QString fechaIngreso = load.value(3).toString();
    QString fechaEntrega = load.value(4).toString();
    double precio = load.value(5).toDouble();
    int idEstadoServicio = load.value(6).toInt();

    if (!showServicioDialog(idCliente, instrumento, descripcion, fechaIngreso, fechaEntrega, precio, idEstadoServicio, "Modificar servicio")) {
        return;
    }

    Servicio servicio(id, idCliente, instrumento.toStdString(), descripcion.toStdString(), fechaIngreso.toStdString(), fechaEntrega.toStdString(), precio, idEstadoServicio);
    if (!dao.modificarQt(servicio)) {
        QMessageBox::warning(this, "Servicios", "No se pudo modificar el servicio.");
        return;
    }

    loadServicios();
}

void MainWindow::deleteServicio()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar servicio", "Selecciona un servicio de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar servicio", "Seguro que queres eliminar el servicio seleccionado?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    ServicioDAO dao(conexionBD.getConexionQt());
    if (!dao.eliminarQt(id)) {
        QMessageBox::warning(this, "Servicios", "No se pudo eliminar el servicio.");
        return;
    }

    loadServicios();
}

void MainWindow::changeServicioEstado()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Cambiar estado", "Selecciona un servicio de la tabla.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Cambiar estado del servicio");
    dialog.setMinimumWidth(360);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;
    QComboBox *estadoCombo = new QComboBox(&dialog);

    EstadoServicioDAO estadoDao(conexionBD.getConexionQt());
    if (!fillCombo(estadoCombo, estadoDao.paraComboQt())) {
        return;
    }

    form->addRow("Nuevo estado:", estadoCombo);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    ServicioDAO dao(conexionBD.getConexionQt());
    if (!dao.cambiarEstadoQt(id, selectedComboId(estadoCombo))) {
        QMessageBox::warning(this, "Servicios", "No se pudo cambiar el estado.");
        return;
    }

    loadServicios();
}



