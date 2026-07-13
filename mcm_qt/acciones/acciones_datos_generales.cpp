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

void MainWindow::addConfigItem()
{
    if (!conexionBD.estaConectadoQt() || !configTabs) {
        QMessageBox::warning(this, "Datos generales", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre;
    QString descripcion;
    const int tab = configTabs->currentIndex();
    const bool hasDescription = tab == 0;

    if (!showConfigDialog(nombre, descripcion, hasDescription, "Agregar")) {
        return;
    }

    bool ok = false;
    switch (tab) {
        case 0: { Categoria m(0, nombre.toStdString(), descripcion.toStdString()); CategoriaDAO dao(conexionBD.getConexionQt()); ok = dao.agregarQt(m); break; }
        case 1: { Marca m(0, nombre.toStdString()); MarcaDAO dao(conexionBD.getConexionQt()); ok = dao.agregarQt(m); break; }
        case 2: { MedioPago m(0, nombre.toStdString()); MedioPagoDAO dao(conexionBD.getConexionQt()); ok = dao.agregarQt(m); break; }
        case 3: { EstadoServicio m(0, nombre.toStdString()); EstadoServicioDAO dao(conexionBD.getConexionQt()); ok = dao.agregarQt(m); break; }
        default:
            return;
    }

    if (!ok) {
        QMessageBox::warning(this, "Datos generales", "No se pudo agregar.");
        return;
    }

    refreshConfigTab();
}

void MainWindow::editConfigItem()
{
    if (!conexionBD.estaConectadoQt() || !configTabs) {
        QMessageBox::warning(this, "Datos generales", "No hay conexion con la base de datos.");
        return;
    }

    QTableWidget *table = nullptr;
    bool hasDescription = false;

    switch (configTabs->currentIndex()) {
        case 0: table = categoriasTable; hasDescription = true; break;
        case 1: table = marcasTable; break;
        case 2: table = mediosPagoTable; break;
        case 3: table = estadosServicioTable; break;
        default: return;
    }

    const int id = selectedId(table);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar", "Selecciona un registro.");
        return;
    }

    const int row = table->currentRow();
    QString nombre = table->item(row, 1)->text();
    QString descripcion = hasDescription && table->item(row, 2) ? table->item(row, 2)->text() : "";

    if (!showConfigDialog(nombre, descripcion, hasDescription, "Modificar")) {
        return;
    }

    bool ok = false;
    switch (configTabs->currentIndex()) {
        case 0: { Categoria m(id, nombre.toStdString(), descripcion.toStdString()); CategoriaDAO dao(conexionBD.getConexionQt()); ok = dao.modificarQt(m); break; }
        case 1: { Marca m(id, nombre.toStdString()); MarcaDAO dao(conexionBD.getConexionQt()); ok = dao.modificarQt(m); break; }
        case 2: { MedioPago m(id, nombre.toStdString()); MedioPagoDAO dao(conexionBD.getConexionQt()); ok = dao.modificarQt(m); break; }
        case 3: { EstadoServicio m(id, nombre.toStdString()); EstadoServicioDAO dao(conexionBD.getConexionQt()); ok = dao.modificarQt(m); break; }
    }
    if (!ok) {
        QMessageBox::warning(this, "Datos generales", "No se pudo modificar.");
        return;
    }

    refreshConfigTab();
}

void MainWindow::deleteConfigItem()
{
    if (!conexionBD.estaConectadoQt() || !configTabs) {
        QMessageBox::warning(this, "Datos generales", "No hay conexion con la base de datos.");
        return;
    }

    QTableWidget *table = nullptr;

    switch (configTabs->currentIndex()) {
        case 0: table = categoriasTable; break;
        case 1: table = marcasTable; break;
        case 2: table = mediosPagoTable; break;
        case 3: table = estadosServicioTable; break;
        default: return;
    }

    const int id = selectedId(table);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar", "Selecciona un registro.");
        return;
    }

    const QString nombre = table->item(table->currentRow(), 1)->text();
    int answer = QMessageBox::question(
        this,
        "Eliminar",
        "Seguro que queres eliminar \"" + nombre + "\"?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    bool ok = false;
    switch (configTabs->currentIndex()) {
        case 0: { CategoriaDAO dao(conexionBD.getConexionQt()); ok = dao.eliminarQt(id); break; }
        case 1: { MarcaDAO dao(conexionBD.getConexionQt()); ok = dao.eliminarQt(id); break; }
        case 2: { MedioPagoDAO dao(conexionBD.getConexionQt()); ok = dao.eliminarQt(id); break; }
        case 3: { EstadoServicioDAO dao(conexionBD.getConexionQt()); ok = dao.eliminarQt(id); break; }
    }
    if (!ok) {
        QMessageBox::warning(this, "Datos generales", "No se pudo eliminar.");
        return;
    }

    refreshConfigTab();
}

void MainWindow::showMessage(const QString &title)
{
    QMessageBox::information(
        this,
        title,
        "Esta accion ya esta funcionando en la version consola.\n"
        "La pantalla grafica inicial ya esta preparada para continuar con este formulario."
    );
}







