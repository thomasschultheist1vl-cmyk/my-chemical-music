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
    if (!dao.agregarQt(factura)) {
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



