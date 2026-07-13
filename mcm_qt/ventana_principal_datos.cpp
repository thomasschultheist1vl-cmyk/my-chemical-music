#include "ventana_principal.h"
#include "./ui_ventana_principal.h"
#include "../dao/CategoriaDAO.h"
#include "../dao/ClienteDAO.h"
#include "../dao/CompraProveedorDAO.h"
#include "../dao/EstadoServicioDAO.h"
#include "../dao/FacturaDAO.h"
#include "../dao/MarcaDAO.h"
#include "../dao/MedioPagoDAO.h"
#include "../dao/ProductoDAO.h"
#include "../dao/ServicioDAO.h"
#include "../dao/VentaDAO.h"

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

namespace {
QDate leerFecha(const QString &text)
{
    QDate fecha = QDate::fromString(text.trimmed(), "dd/MM/yyyy");
    if (!fecha.isValid()) {
        fecha = QDate::fromString(text.trimmed().left(10), Qt::ISODate);
    }
    return fecha;
}

QString fechaVisual(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        return {};
    }
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString("dd/MM/yyyy") : text;
}

QString fechaParaBase(const QString &text)
{
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString(Qt::ISODate) : text;
}
}

void MainWindow::connectDatabase()
{
    QString error;
    if (!conexionBD.conectarQt(&error)) {
        connectionLabel->setText("Sin conexion ODBC 64: my_chemical_music64");
        connectionLabel->setStyleSheet("color: #f87171;");
        return;
    }

    connectionLabel->setText("Conectado a MySQL por ODBC");
    connectionLabel->setStyleSheet("color: #22c55e;");
}

void MainWindow::actualizarDashboard()
{
    if (!movimientosTable || !productosBajoStockList) {
        return;
    }

    movimientosTable->clearSpans();
    movimientosTable->setRowCount(0);
    productosBajoStockList->clear();

    if (!conexionBD.estaConectadoQt()) {
        clientesCountLabel->setText("0");
        productosCountLabel->setText("0");
        ventasCountLabel->setText("0");
        serviciosCountLabel->setText("0");
        movimientosTable->insertRow(0);
        movimientosTable->setItem(0, 0, new QTableWidgetItem("No hay movimientos registrados"));
        movimientosTable->setSpan(0, 0, 1, 3);
        productosBajoStockList->addItem("No hay productos bajos en stock");
        return;
    }

    auto actualizarCantidad = [](QLabel *label, QSqlQuery query) {
        if (query.next()) {
            label->setText(query.value(0).toString());
        } else {
            label->setText("0");
        }
    };

    ClienteDAO clienteDao(conexionBD.getConexionQt());
    ProductoDAO productoDao(conexionBD.getConexionQt());
    VentaDAO ventaDao(conexionBD.getConexionQt());
    ServicioDAO servicioDao(conexionBD.getConexionQt());
    CompraProveedorDAO compraDao(conexionBD.getConexionQt());
    FacturaDAO facturaDao(conexionBD.getConexionQt());
    actualizarCantidad(clientesCountLabel, clienteDao.contarQt());
    actualizarCantidad(productosCountLabel, productoDao.contarQt());
    actualizarCantidad(ventasCountLabel, ventaDao.contarQt());
    actualizarCantidad(serviciosCountLabel, servicioDao.contarQt());

    QSqlQuery stockQuery = productoDao.stockBajoQt(3);
    if (!stockQuery.lastError().isValid()) {
        while (stockQuery.next()) {
            const QString nombre = stockQuery.value(0).toString();
            const int stock = stockQuery.value(1).toInt();
            const QString unidades = stock == 1 ? " unidad" : " unidades";
            const QString texto = QString(QChar(0x26A0)) + " " + nombre + " "
                                + QString(QChar(0x2014)) + " "
                                + QString::number(stock) + unidades;
            productosBajoStockList->addItem(texto);
        }
    }

    if (productosBajoStockList->count() == 0) {
        productosBajoStockList->addItem("No hay productos bajos en stock");
    }

    struct Movimiento {
        QString clave;
        QString tipo;
        QString descripcion;
        QString fechaTexto;
        QDate fecha;
        int id;
        quint64 ordenDetectado = 0;
    };

    QVector<Movimiento> movimientos;

    auto agregarMovimientos = [this, &movimientos](QSqlQuery query,
                                                   const QString &tipo,
                                                   const QString &descripcionInicial,
                                                   const QString &descripcionFinal,
                                                   bool tieneNombre,
                                                   bool tieneFecha) {
        if (query.lastError().isValid()) {
            return;
        }

        const int inicio = movimientos.size();
        int maxId = 0;
        while (query.next()) {
            Movimiento movimiento;
            movimiento.id = query.value(0).toInt();
            movimiento.tipo = tipo;
            movimiento.clave = tipo + ":" + QString::number(movimiento.id);
            movimiento.ordenDetectado = dashboardDetectedOrder.value(movimiento.clave, 0);
            maxId = std::max(maxId, movimiento.id);
            const QString referencia = tieneNombre ? query.value(1).toString()
                                                   : QString::number(movimiento.id);
            movimiento.descripcion = descripcionInicial + referencia + descripcionFinal;
            if (tieneFecha) {
                movimiento.fecha = QDate::fromString(query.value(2).toString().left(10), Qt::ISODate);
                movimiento.fechaTexto = movimiento.fecha.isValid()
                    ? movimiento.fecha.toString("dd/MM/yyyy")
                    : query.value(2).toString();
            }
            movimientos.append(movimiento);
        }

        if (!dashboardMaxId.contains(tipo)) {
            dashboardMaxId.insert(tipo, maxId);
            return;
        }

        const int anteriorMaxId = dashboardMaxId.value(tipo);
        for (int index = movimientos.size() - 1; index >= inicio; --index) {
            Movimiento &movimiento = movimientos[index];
            if (movimiento.id > anteriorMaxId && !dashboardDetectedOrder.contains(movimiento.clave)) {
                const quint64 orden = ++dashboardSequence;
                dashboardDetectedOrder.insert(movimiento.clave, orden);
                movimiento.ordenDetectado = orden;
            }
        }
        dashboardMaxId.insert(tipo, std::max(anteriorMaxId, maxId));
    };

    agregarMovimientos(clienteDao.ultimosQt(),
                       "Cliente", "Se agregó el cliente ", ".", true, false);
    agregarMovimientos(productoDao.ultimosQt(),
                       "Producto", "Se agregó el producto ", ".", true, false);
    agregarMovimientos(ventaDao.ultimosQt(),
                       "Venta", "Se registró la venta N.º ", ".", false, true);
    agregarMovimientos(servicioDao.ultimosQt(),
                       "Servicio", "Se registró el servicio N.º ", ".", false, true);
    agregarMovimientos(compraDao.ultimosQt(),
                       "Compra", "Se registró la compra N.º ", ".", false, true);
    agregarMovimientos(facturaDao.ultimosQt(),
                       "Factura", "Se generó la factura N.º ", ".", false, true);

    std::stable_sort(movimientos.begin(), movimientos.end(), [](const Movimiento &a, const Movimiento &b) {
        if (a.ordenDetectado != b.ordenDetectado) {
            return a.ordenDetectado > b.ordenDetectado;
        }
        if (a.fecha.isValid() != b.fecha.isValid()) {
            return a.fecha.isValid();
        }
        if (a.fecha.isValid() && a.fecha != b.fecha) {
            return a.fecha > b.fecha;
        }
        if (a.tipo == b.tipo) {
            return a.id > b.id;
        }
        return false;
    });

    const int cantidad = std::min(7, static_cast<int>(movimientos.size()));
    for (int row = 0; row < cantidad; ++row) {
        movimientosTable->insertRow(row);
        movimientosTable->setItem(row, 0, new QTableWidgetItem(movimientos[row].tipo));
        movimientosTable->setItem(row, 1, new QTableWidgetItem(movimientos[row].descripcion));
        movimientosTable->setItem(row, 2, new QTableWidgetItem(movimientos[row].fechaTexto));
    }

    if (cantidad == 0) {
        movimientosTable->insertRow(0);
        movimientosTable->setItem(0, 0, new QTableWidgetItem("No hay movimientos registrados"));
        movimientosTable->setSpan(0, 0, 1, 3);
    }
}

void MainWindow::fillTable(QTableWidget *table, const QStringList &headers, QSqlQuery query)
{
    if (!table) {
        return;
    }

    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setRowCount(0);

    if (!conexionBD.estaConectadoQt()) {
        return;
    }

    if (query.lastError().isValid()) {
        QMessageBox::warning(this, "Base de datos", "No se pudieron cargar los datos:\n" + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}

bool MainWindow::fillCombo(QComboBox *combo, QSqlQuery query)
{
    if (!combo || !conexionBD.estaConectadoQt()) {
        return false;
    }

    combo->clear();

    if (query.lastError().isValid()) {
        QMessageBox::warning(this, "Base de datos", "No se pudieron cargar las opciones:\n" + query.lastError().text());
        return false;
    }

    while (query.next()) {
        combo->addItem(query.value(1).toString(), query.value(0).toInt());
    }

    return combo->count() > 0;
}

int MainWindow::selectedComboId(QComboBox *combo) const
{
    if (!combo || combo->currentIndex() < 0) {
        return -1;
    }

    return combo->currentData().toInt();
}

void MainWindow::loadClientes()
{
    ClienteDAO dao(conexionBD.getConexionQt());
    fillTable(clientesTable, {"ID", "Nombre", "Apellido", "Telefono", "Email", "Direccion", "DNI"}, dao.listarQt());
}

void MainWindow::loadProductos()
{
    ProductoDAO dao(conexionBD.getConexionQt());
    fillTable(productosTable, {"ID", "Nombre", "Descripcion", "Precio", "Stock", "Minimo", "Categoria", "Marca", "Proveedor"}, dao.listarQt());
}

void MainWindow::loadVentas()
{
    VentaDAO dao(conexionBD.getConexionQt());
    fillTable(ventasTable, {"ID", "Cliente", "Fecha", "Total", "Medio de pago"}, dao.listarQt());
}

void MainWindow::loadServicios()
{
    ServicioDAO dao(conexionBD.getConexionQt());
    fillTable(serviciosTable, {"ID", "Cliente", "Instrumento", "Descripcion", "Ingreso", "Entrega", "Precio", "Estado"}, dao.listarQt());
}

void MainWindow::loadCompras()
{
    CompraProveedorDAO dao(conexionBD.getConexionQt());
    fillTable(comprasTable, {"ID", "Proveedor", "Fecha", "Total"}, dao.listarQt());
}

void MainWindow::loadFacturas()
{
    FacturaDAO dao(conexionBD.getConexionQt());
    fillTable(facturasTable, {"ID", "Origen", "Referencia", "Numero", "Tipo", "Fecha", "Total"}, dao.listarQt());
}

void MainWindow::loadConfiguracion()
{
    loadCategorias();
    loadMarcas();
    loadMediosPago();
    loadEstadosServicio();
}

void MainWindow::loadCategorias()
{
    CategoriaDAO dao(conexionBD.getConexionQt());
    fillTable(categoriasTable, {"ID", "Nombre", "Descripcion"}, dao.listarQt());
}

void MainWindow::loadMarcas()
{
    MarcaDAO dao(conexionBD.getConexionQt());
    fillTable(marcasTable, {"ID", "Nombre"}, dao.listarQt());
}

void MainWindow::loadMediosPago()
{
    MedioPagoDAO dao(conexionBD.getConexionQt());
    fillTable(mediosPagoTable, {"ID", "Nombre"}, dao.listarQt());
}

void MainWindow::loadEstadosServicio()
{
    EstadoServicioDAO dao(conexionBD.getConexionQt());
    fillTable(estadosServicioTable, {"ID", "Nombre"}, dao.listarQt());
}

void MainWindow::refreshConfigTab()
{
    if (!configTabs) {
        return;
    }

    switch (configTabs->currentIndex()) {
        case 0: loadCategorias(); break;
        case 1: loadMarcas(); break;
        case 2: loadMediosPago(); break;
        case 3: loadEstadosServicio(); break;
        default: break;
    }
}



