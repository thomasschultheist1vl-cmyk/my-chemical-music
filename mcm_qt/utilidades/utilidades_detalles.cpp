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
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QMessageBox>
#include <QPair>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>

namespace {
QString dinero(double valor)
{
    static const QLocale locale(QLocale::Spanish, QLocale::Argentina);
    return "$" + locale.toString(valor, 'f', 2);
}

QString texto(const QVariant &valor)
{
    return valor.toString().trimmed();
}

void configurarTablaDetalle(QTableWidget *table)
{
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setStyleSheet("QTableWidget { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 6px; }");
}

void agregarFila(QTableWidget *table, const QStringList &valores)
{
    const int row = table->rowCount();
    table->insertRow(row);
    for (int col = 0; col < valores.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(valores[col]);
        if (col > 0) {
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
        table->setItem(row, col, item);
    }
}
}

void MainWindow::showDetalleSimple(const QString &titulo, const QList<QPair<QString, QString>> &campos)
{
    QDialog dialog(this);
    dialog.setWindowTitle(titulo);
    dialog.resize(560, 460);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *title = new QLabel(titulo, &dialog);
    title->setStyleSheet("color: #f87171; font-size: 18px; font-weight: 700;");

    QTextEdit *textoDetalle = new QTextEdit(&dialog);
    textoDetalle->setReadOnly(true);
    textoDetalle->setStyleSheet("QTextEdit { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 7px; padding: 10px; }");

    QString html;
    for (const auto &campo : campos) {
        if (campo.second.trimmed().isEmpty()) {
            continue;
        }
        html += "<p><b style='color:#f87171;'>" + campo.first.toHtmlEscaped() + ":</b> "
              + campo.second.toHtmlEscaped().replace("\n", "<br>") + "</p>";
    }
    textoDetalle->setHtml(html);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");

    layout->addWidget(title);
    layout->addWidget(textoDetalle, 1);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::showClienteDetails()
{
    const int id = selectedId(clientesTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle del cliente", "Selecciona un cliente de la tabla.");
        return;
    }

    ClienteDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.detalleQt(id);
    if (!q.next()) {
        QMessageBox::warning(this, "Detalle del cliente", "No se pudo cargar el cliente seleccionado.");
        return;
    }

    showDetalleSimple("Detalle del cliente", {
        {"ID", texto(q.value(0))},
        {"Nombre", texto(q.value(1))},
        {"Apellido", texto(q.value(2))},
        {"DNI", texto(q.value(3))},
        {"Telefono", texto(q.value(4))},
        {"Email", texto(q.value(5))},
        {"Direccion", texto(q.value(6))}
    });
}

void MainWindow::showProductoDetails()
{
    const int id = selectedId(productosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle del producto", "Selecciona un producto de la tabla.");
        return;
    }

    ProductoDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.detalleQt(id);
    if (!q.next()) {
        QMessageBox::warning(this, "Detalle del producto", "No se pudo cargar el producto seleccionado.");
        return;
    }

    showDetalleSimple("Detalle del producto", {
        {"ID", texto(q.value(0))},
        {"Nombre", texto(q.value(1))},
        {"Descripcion", texto(q.value(2))},
        {"Precio", dinero(q.value(3).toDouble())},
        {"Stock", texto(q.value(4))},
        {"Stock minimo", texto(q.value(5))},
        {"Categoria", texto(q.value(6))},
        {"Marca", texto(q.value(7))},
        {"Proveedor", texto(q.value(8))}
    });
}

void MainWindow::showVentaDetails()
{
    const int id = selectedId(ventasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle de venta", "Selecciona una venta de la tabla.");
        return;
    }

    VentaDAO ventaDao(conexionBD.getConexionQt());
    QSqlQuery venta = ventaDao.detalleQt(id);
    if (!venta.next()) {
        QMessageBox::warning(this, "Detalle de venta", "No se pudo cargar la venta seleccionada.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Detalle de venta " + QString::number(id));
    dialog.resize(820, 560);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("Detalle de venta", &dialog);
    title->setStyleSheet("color: #f87171; font-size: 18px; font-weight: 700;");
    layout->addWidget(title);

    QList<QPair<QString, QString>> campos = {
        {"ID de venta", texto(venta.value(0))},
        {"Cliente", texto(venta.value(1))},
        {"Fecha", texto(venta.value(2))},
        {"Medio de pago", texto(venta.value(3))},
        {"Estado", texto(venta.value(4))},
        {"Total", dinero(venta.value(5).toDouble())}
    };
    if (texto(venta.value(4)) == "Anulada") {
        campos.append({"Motivo de anulacion", texto(venta.value(6))});
        campos.append({"Fecha de anulacion", texto(venta.value(7))});
    }

    QTextEdit *info = new QTextEdit(&dialog);
    info->setReadOnly(true);
    info->setMaximumHeight(210);
    info->setStyleSheet("QTextEdit { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 7px; padding: 8px; }");
    QString html;
    for (const auto &campo : campos) {
        html += "<p><b style='color:#f87171;'>" + campo.first.toHtmlEscaped() + ":</b> " + campo.second.toHtmlEscaped() + "</p>";
    }
    info->setHtml(html);
    layout->addWidget(info);

    QLabel *detalleTitle = new QLabel("Productos vendidos", &dialog);
    detalleTitle->setStyleSheet("color: #f8fafc; font-weight: 700;");
    layout->addWidget(detalleTitle);

    QTableWidget *table = new QTableWidget(&dialog);
    configurarTablaDetalle(table);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Producto", "Cantidad", "Precio unitario", "Subtotal"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    DetalleVentaDAO detalleDao(conexionBD.getConexionQt());
    QSqlQuery detalle = detalleDao.listarPorVentaQt(id);
    while (detalle.next()) {
        agregarFila(table, {texto(detalle.value(0)), texto(detalle.value(1)), dinero(detalle.value(2).toDouble()), dinero(detalle.value(3).toDouble())});
    }
    layout->addWidget(table, 1);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::showServicioDetails()
{
    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle del servicio", "Selecciona un servicio de la tabla.");
        return;
    }

    ServicioDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.detalleQt(id);
    if (!q.next()) {
        QMessageBox::warning(this, "Detalle del servicio", "No se pudo cargar el servicio seleccionado.");
        return;
    }

    QList<QPair<QString, QString>> campos = {
        {"ID", texto(q.value(0))},
        {"Cliente", texto(q.value(1))},
        {"Instrumento", texto(q.value(2))},
        {"Descripcion", texto(q.value(3))},
        {"Fecha de ingreso", texto(q.value(4))},
        {"Fecha de entrega", texto(q.value(5))},
        {"Precio", dinero(q.value(6).toDouble())},
        {"Estado", texto(q.value(7))}
    };
    if (texto(q.value(7)) == "Anulado") {
        campos.append({"Motivo de anulacion", texto(q.value(8))});
        campos.append({"Fecha de anulacion", texto(q.value(9))});
    }
    showDetalleSimple("Detalle del servicio", campos);
}

void MainWindow::showCompraDetails()
{
    const int id = selectedId(comprasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle de compra", "Selecciona una compra de la tabla.");
        return;
    }

    CompraProveedorDAO compraDao(conexionBD.getConexionQt());
    QSqlQuery compra = compraDao.detalleQt(id);
    if (!compra.next()) {
        QMessageBox::warning(this, "Detalle de compra", "No se pudo cargar la compra seleccionada.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Detalle de compra " + QString::number(id));
    dialog.resize(820, 560);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("Detalle de compra", &dialog);
    title->setStyleSheet("color: #f87171; font-size: 18px; font-weight: 700;");
    layout->addWidget(title);

    QList<QPair<QString, QString>> campos = {
        {"ID de compra", texto(compra.value(0))},
        {"Proveedor", texto(compra.value(1))},
        {"Fecha", texto(compra.value(2))},
        {"Total", dinero(compra.value(3).toDouble())},
        {"Estado", texto(compra.value(4))}
    };
    if (texto(compra.value(4)) == "Anulada") {
        campos.append({"Motivo de anulacion", texto(compra.value(5))});
        campos.append({"Fecha de anulacion", texto(compra.value(6))});
    }

    QTextEdit *info = new QTextEdit(&dialog);
    info->setReadOnly(true);
    info->setMaximumHeight(210);
    info->setStyleSheet("QTextEdit { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 7px; padding: 8px; }");
    QString html;
    for (const auto &campo : campos) {
        html += "<p><b style='color:#f87171;'>" + campo.first.toHtmlEscaped() + ":</b> " + campo.second.toHtmlEscaped() + "</p>";
    }
    info->setHtml(html);
    layout->addWidget(info);

    QLabel *detalleTitle = new QLabel("Productos comprados", &dialog);
    detalleTitle->setStyleSheet("color: #f8fafc; font-weight: 700;");
    layout->addWidget(detalleTitle);

    QTableWidget *table = new QTableWidget(&dialog);
    configurarTablaDetalle(table);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Producto", "Cantidad", "Precio compra", "Subtotal"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    DetalleCompraDAO detalleDao(conexionBD.getConexionQt());
    QSqlQuery detalle = detalleDao.listarPorCompraQt(id);
    while (detalle.next()) {
        agregarFila(table, {texto(detalle.value(0)), texto(detalle.value(1)), dinero(detalle.value(2).toDouble()), dinero(detalle.value(3).toDouble())});
    }
    layout->addWidget(table, 1);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::showFacturaDetails()
{
    const int id = selectedId(facturasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle de factura", "Selecciona una factura de la tabla.");
        return;
    }

    FacturaDAO facturaDao(conexionBD.getConexionQt());
    QSqlQuery factura = facturaDao.detalleQt(id);
    if (!factura.next()) {
        QMessageBox::warning(this, "Detalle de factura", "No se pudo cargar la factura seleccionada.");
        return;
    }

    const QString origen = texto(factura.value(5));
    const int operacionId = factura.value(6).toInt();

    QDialog dialog(this);
    dialog.setWindowTitle("Detalle de factura " + QString::number(id));
    dialog.resize(820, 620);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("MY CHEMICAL MUSIC", &dialog);
    title->setStyleSheet("color: #f87171; font-size: 20px; font-weight: 800;");
    layout->addWidget(title);

    QList<QPair<QString, QString>> campos = {
        {"ID interno", texto(factura.value(0))},
        {"Factura N.º", texto(factura.value(1))},
        {"Tipo", texto(factura.value(2))},
        {"Fecha", texto(factura.value(3))},
        {"Estado", texto(factura.value(4))},
        {"Origen", origen},
        {"Operacion facturada", texto(factura.value(7))},
        {"Cliente", texto(factura.value(8))},
        {"Total", dinero(factura.value(9).toDouble())},
        {"Estado de pago", texto(factura.value(12))},
        {"Fecha de pago", texto(factura.value(13))},
        {"Pago registrado por", texto(factura.value(14))}
    };
    if (texto(factura.value(4)) == "Anulada") {
        campos.append({"Motivo de anulacion", texto(factura.value(10))});
        campos.append({"Fecha de anulacion", texto(factura.value(11))});
        campos.append({"Anulada por", texto(factura.value(16))});
    }
    if (usuarioActual.esSupervisor()) {
        campos.append({"Creada por", texto(factura.value(15))});
    }

    QTextEdit *info = new QTextEdit(&dialog);
    info->setReadOnly(true);
    info->setMaximumHeight(260);
    info->setStyleSheet("QTextEdit { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 7px; padding: 8px; }");
    QString html;
    for (const auto &campo : campos) {
        html += "<p><b style='color:#f87171;'>" + campo.first.toHtmlEscaped() + ":</b> " + campo.second.toHtmlEscaped() + "</p>";
    }
    info->setHtml(html);
    layout->addWidget(info);

    if (origen == "Venta") {
        QLabel *detalleTitle = new QLabel("Detalle de la venta", &dialog);
        detalleTitle->setStyleSheet("color: #f8fafc; font-weight: 700;");
        layout->addWidget(detalleTitle);

        QTableWidget *table = new QTableWidget(&dialog);
        configurarTablaDetalle(table);
        table->setColumnCount(4);
        table->setHorizontalHeaderLabels({"Producto", "Cant.", "Precio", "Subtotal"});
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

        DetalleVentaDAO detalleDao(conexionBD.getConexionQt());
        QSqlQuery detalle = detalleDao.listarPorVentaQt(operacionId);
        while (detalle.next()) {
            agregarFila(table, {texto(detalle.value(0)), texto(detalle.value(1)), dinero(detalle.value(2).toDouble()), dinero(detalle.value(3).toDouble())});
        }
        layout->addWidget(table, 1);
    } else {
        ServicioDAO servicioDao(conexionBD.getConexionQt());
        QSqlQuery servicio = servicioDao.detalleQt(operacionId);
        if (servicio.next()) {
            QTextEdit *servicioInfo = new QTextEdit(&dialog);
            servicioInfo->setReadOnly(true);
            servicioInfo->setStyleSheet("QTextEdit { background: #18181b; color: #f8fafc; border: 1px solid #3f3f46; border-radius: 7px; padding: 8px; }");
            const QList<QPair<QString, QString>> servicioCampos = {
                {"Instrumento", texto(servicio.value(2))},
                {"Descripcion", texto(servicio.value(3))},
                {"Ingreso", texto(servicio.value(4))},
                {"Entrega", texto(servicio.value(5))},
                {"Estado del servicio", texto(servicio.value(7))},
                {"Precio", dinero(servicio.value(6).toDouble())}
            };
            QString servicioHtml;
            for (const auto &campo : servicioCampos) {
                servicioHtml += "<p><b style='color:#f87171;'>" + campo.first.toHtmlEscaped() + ":</b> " + campo.second.toHtmlEscaped() + "</p>";
            }
            servicioInfo->setHtml(servicioHtml);
            layout->addWidget(servicioInfo, 1);
        }
    }

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::showConfigDetails()
{
    if (!configTabs) {
        return;
    }

    QTableWidget *table = nullptr;
    QString titulo;
    QList<QPair<QString, QString>> campos;
    const int tab = configTabs->currentIndex();
    if (tab == 0) {
        table = categoriasTable;
        const int id = selectedId(table);
        if (id <= 0) { QMessageBox::information(this, "Detalle de categoria", "Selecciona una categoria de la tabla."); return; }
        CategoriaDAO dao(conexionBD.getConexionQt());
        QSqlQuery q = dao.detalleQt(id);
        if (!q.next()) return;
        titulo = "Detalle de categoria";
        campos = {{"ID", texto(q.value(0))}, {"Nombre", texto(q.value(1))}, {"Descripcion", texto(q.value(2))}};
    } else if (tab == 1) {
        table = marcasTable;
        const int id = selectedId(table);
        if (id <= 0) { QMessageBox::information(this, "Detalle de marca", "Selecciona una marca de la tabla."); return; }
        MarcaDAO dao(conexionBD.getConexionQt());
        QSqlQuery q = dao.detalleQt(id);
        if (!q.next()) return;
        titulo = "Detalle de marca";
        campos = {{"ID", texto(q.value(0))}, {"Nombre", texto(q.value(1))}};
    } else if (tab == 2) {
        table = mediosPagoTable;
        const int id = selectedId(table);
        if (id <= 0) { QMessageBox::information(this, "Detalle de medio de pago", "Selecciona un medio de pago de la tabla."); return; }
        MedioPagoDAO dao(conexionBD.getConexionQt());
        QSqlQuery q = dao.detalleQt(id);
        if (!q.next()) return;
        titulo = "Detalle de medio de pago";
        campos = {{"ID", texto(q.value(0))}, {"Nombre", texto(q.value(1))}};
    } else {
        table = estadosServicioTable;
        const int id = selectedId(table);
        if (id <= 0) { QMessageBox::information(this, "Detalle de estado de servicio", "Selecciona un estado de la tabla."); return; }
        EstadoServicioDAO dao(conexionBD.getConexionQt());
        QSqlQuery q = dao.detalleQt(id);
        if (!q.next()) return;
        titulo = "Detalle de estado de servicio";
        campos = {{"ID", texto(q.value(0))}, {"Nombre", texto(q.value(1))}};
    }

    showDetalleSimple(titulo, campos);
}
