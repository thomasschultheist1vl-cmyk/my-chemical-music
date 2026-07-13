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

void MainWindow::addCliente()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre, apellido, telefono, email, direccion, dni;
    if (!showClienteDialog(nombre, apellido, telefono, email, direccion, dni, "Agregar cliente")) {
        return;
    }

    Cliente cliente(0, nombre.toStdString(), apellido.toStdString(), telefono.toStdString(), email.toStdString(), direccion.toStdString(), dni.toStdString());
    ClienteDAO dao(conexionBD.getConexionQt());
    if (!dao.agregarQt(cliente)) {
        QMessageBox::warning(this, "Clientes", "No se pudo agregar el cliente.");
        return;
    }

    loadClientes();
    actualizarDashboard();
}
void MainWindow::editCliente()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    int id = selectedClienteId();
    if (id <= 0) {
        QMessageBox::information(this, "Modificar cliente", "Selecciona un cliente de la tabla.");
        return;
    }

    int row = clientesTable->currentRow();
    QString nombre = clientesTable->item(row, 1)->text();
    QString apellido = clientesTable->item(row, 2)->text();
    QString telefono = clientesTable->item(row, 3)->text();
    QString email = clientesTable->item(row, 4)->text();
    QString direccion = clientesTable->item(row, 5)->text();
    QString dni = clientesTable->item(row, 6)->text();

    if (!showClienteDialog(nombre, apellido, telefono, email, direccion, dni, "Modificar cliente")) {
        return;
    }

    Cliente cliente(id, nombre.toStdString(), apellido.toStdString(), telefono.toStdString(), email.toStdString(), direccion.toStdString(), dni.toStdString());
    ClienteDAO dao(conexionBD.getConexionQt());
    if (!dao.modificarQt(cliente)) {
        QMessageBox::warning(this, "Clientes", "No se pudo modificar el cliente.");
        return;
    }

    loadClientes();
}
void MainWindow::deleteCliente()
{
    if (!conexionBD.estaConectadoQt()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    int id = selectedClienteId();
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar cliente", "Selecciona un cliente de la tabla.");
        return;
    }

    QString nombre = clientesTable->item(clientesTable->currentRow(), 1)->text();
    QString apellido = clientesTable->item(clientesTable->currentRow(), 2)->text();

    int answer = QMessageBox::question(
        this,
        "Eliminar cliente",
        "Seguro que queres eliminar a " + nombre + " " + apellido + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    ClienteDAO dao(conexionBD.getConexionQt());
    if (!dao.eliminarQt(id)) {
        QMessageBox::warning(this, "Clientes", "No se pudo eliminar el cliente.");
        return;
    }

    loadClientes();
}



