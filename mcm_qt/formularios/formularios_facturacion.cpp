#include "../ventana_principal.h"
#include "ui_ventana_principal.h"
#include "../../dao/CategoriaDAO.h"
#include "../../dao/ClienteDAO.h"
#include "../../dao/DetalleVentaDAO.h"
#include "../../dao/EstadoServicioDAO.h"
#include "../../dao/MarcaDAO.h"
#include "../../dao/MedioPagoDAO.h"
#include "../../dao/ProductoDAO.h"
#include "../../dao/ProveedorDAO.h"
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
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStackedWidget>
#include <QSpinBox>
#include <QSignalBlocker>
#include <QTabBar>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVariant>
#include <QVector>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>

#include "../utilidades/utilidades_fecha.h"

namespace {
QString importeFactura(double valor)
{
    const bool sinDecimales = std::abs(valor - std::round(valor)) < 0.005;
    const QLocale locale(QLocale::Spanish, QLocale::Argentina);
    return "$" + locale.toString(valor, 'f', sinDecimales ? 0 : 2);
}
}

bool MainWindow::showFacturaDialog(int &idVenta, int &idServicio, QString &numeroFactura, QString &tipoFactura, QString &fecha, double &total)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Generar factura");
    dialog.setMinimumSize(760, 610);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(12);

    QFormLayout *referenciaForm = new QFormLayout;
    referenciaForm->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    QFormLayout *facturaForm = new QFormLayout;
    facturaForm->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    QComboBox *origenCombo = new QComboBox(&dialog);
    QComboBox *referenciaCombo = new QComboBox(&dialog);
    referenciaCombo->setMinimumWidth(580);
    QLineEdit *numeroEdit = new QLineEdit(numeroFactura, &dialog);
    QComboBox *tipoCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fechaVisual(fecha), &dialog);
    QLabel *totalLabel = new QLabel("$0", &dialog);
    totalLabel->setStyleSheet("font-size: 17px; font-weight: 700; color: #f8fafc;");
    fechaEdit->setPlaceholderText("DD/MM/AAAA");

    origenCombo->addItem("Venta", "venta");
    origenCombo->addItem("Servicio", "servicio");
    tipoCombo->addItems({"A", "B", "C"});
    const int tipoIndex = tipoCombo->findText(tipoFactura);
    if (tipoIndex >= 0) {
        tipoCombo->setCurrentIndex(tipoIndex);
    }

    QLabel *detalleTitulo = new QLabel("Detalle", &dialog);
    detalleTitulo->setStyleSheet("font-size: 16px; font-weight: 700; color: #f8fafc;");

    QFrame *detallePanel = new QFrame(&dialog);
    detallePanel->setObjectName("detalleFacturaPanel");
    detallePanel->setStyleSheet(
        "QFrame#detalleFacturaPanel { background: #18181b; border: 1px solid #3f3f46; border-radius: 7px; }"
    );
    QVBoxLayout *detalleLayout = new QVBoxLayout(detallePanel);
    detalleLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *detalleMensaje = new QLabel(detallePanel);
    detalleMensaje->setWordWrap(true);
    detalleMensaje->setAlignment(Qt::AlignCenter);
    detalleMensaje->setStyleSheet("color: #a1a1aa; padding: 24px;");

    QTableWidget *detalleTable = new QTableWidget(detallePanel);
    detalleTable->setMinimumHeight(220);
    detalleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detalleTable->setSelectionMode(QAbstractItemView::NoSelection);
    detalleTable->setAlternatingRowColors(true);
    detalleTable->verticalHeader()->setVisible(false);
    detalleTable->verticalHeader()->setDefaultSectionSize(34);
    detalleTable->setStyleSheet("QTableWidget { border: none; border-radius: 0px; }");

    detalleLayout->addWidget(detalleMensaje);
    detalleLayout->addWidget(detalleTable);

    referenciaForm->addRow("Facturar:", origenCombo);
    referenciaForm->addRow("Referencia:", referenciaCombo);
    facturaForm->addRow("Numero factura:", numeroEdit);
    facturaForm->addRow("Tipo factura:", tipoCombo);
    facturaForm->addRow("Fecha:", fechaEdit);
    facturaForm->addRow("Total:", totalLabel);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    QPushButton *guardarButton = buttons->button(QDialogButtonBox::Save);
    guardarButton->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(referenciaForm);
    layout->addWidget(detalleTitulo);
    layout->addWidget(detallePanel, 1);
    layout->addLayout(facturaForm);
    layout->addWidget(buttons);

    auto limpiarDetalle = [&]() {
        total = 0;
        totalLabel->setText("$0");
        detalleTable->clear();
        detalleTable->setRowCount(0);
        detalleTable->setColumnCount(0);
        detalleTable->hide();
        detalleMensaje->clear();
        detalleMensaje->show();
    };

    auto actualizarDetalle = [&]() {
        limpiarDetalle();
        const int referenciaId = referenciaCombo->currentData().toInt();
        const bool referenciaValida = referenciaCombo->currentIndex() >= 0 && referenciaId > 0;
        guardarButton->setEnabled(referenciaValida);
        if (!referenciaValida) {
            detalleTitulo->setText("Detalle");
            detalleMensaje->setText(origenCombo->currentData().toString() == "venta"
                ? "No hay ventas disponibles para facturar"
                : "No hay servicios disponibles para facturar");
            return;
        }

        total = referenciaCombo->currentData(Qt::UserRole + 1).toDouble();
        totalLabel->setText(importeFactura(total));

        if (origenCombo->currentData().toString() == "venta") {
            detalleTitulo->setText("Detalle de la venta");
            detalleTable->setColumnCount(4);
            detalleTable->setHorizontalHeaderLabels({"Producto", "Cantidad", "Precio unitario", "Subtotal"});
            detalleTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            detalleTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
            detalleTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
            detalleTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

            DetalleVentaDAO detalleDao(conexionBD.getConexionQt());
            QSqlQuery query = detalleDao.listarPorVentaQt(referenciaId);
            if (query.lastError().isValid()) {
                detalleMensaje->setText("No se pudo cargar el detalle de la venta.");
                return;
            }

            while (query.next()) {
                const int row = detalleTable->rowCount();
                detalleTable->insertRow(row);
                detalleTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
                detalleTable->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toInt())));
                detalleTable->setItem(row, 2, new QTableWidgetItem(importeFactura(query.value(2).toDouble())));
                detalleTable->setItem(row, 3, new QTableWidgetItem(importeFactura(query.value(3).toDouble())));
                for (int column = 1; column < 4; ++column) {
                    detalleTable->item(row, column)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                }
            }

            if (detalleTable->rowCount() == 0) {
                detalleMensaje->setText("La venta no tiene productos registrados.");
                return;
            }
        } else {
            detalleTitulo->setText("Detalle del servicio");
            detalleTable->setColumnCount(2);
            detalleTable->setHorizontalHeaderLabels({"Dato", "Detalle"});
            detalleTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            detalleTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

            ServicioDAO servicioDao(conexionBD.getConexionQt());
            QSqlQuery query = servicioDao.detalleParaFacturaQt(referenciaId);
            if (query.lastError().isValid() || !query.next()) {
                detalleMensaje->setText("No se pudo cargar el detalle del servicio.");
                return;
            }

            auto agregarDato = [&](const QString &nombre, const QString &valor) {
                if (valor.trimmed().isEmpty()) {
                    return;
                }
                const int row = detalleTable->rowCount();
                detalleTable->insertRow(row);
                detalleTable->setItem(row, 0, new QTableWidgetItem(nombre));
                detalleTable->setItem(row, 1, new QTableWidgetItem(valor));
            };

            agregarDato("Cliente", query.value(0).toString());
            agregarDato("Instrumento", query.value(1).toString());
            agregarDato("Trabajo", query.value(2).toString());
            agregarDato("Fecha de ingreso", query.value(3).toString());
            agregarDato("Fecha de entrega", query.value(4).toString());
            agregarDato("Estado", query.value(5).toString());
            agregarDato("Total", importeFactura(query.value(6).toDouble()));
        }

        detalleMensaje->hide();
        detalleTable->show();
    };

    auto cargarReferencias = [&]() {
        const QSignalBlocker blocker(referenciaCombo);
        referenciaCombo->clear();
        const bool esVenta = origenCombo->currentData().toString() == "venta";

        if (esVenta) {
            VentaDAO ventaDao(conexionBD.getConexionQt());
            QSqlQuery query = ventaDao.disponiblesParaFacturarQt();
            if (query.lastError().isValid()) {
                QMessageBox::warning(&dialog, "Base de datos", "No se pudieron cargar las ventas:\n" + query.lastError().text());
            } else {
                while (query.next()) {
                    const int id = query.value(0).toInt();
                    const QString cliente = query.value(1).toString();
                    const int productos = query.value(2).toInt();
                    const double importe = query.value(5).toDouble();
                    QString resumen;
                    if (productos == 1) {
                        resumen = QString("Venta %1 — %2 — %3 x%4 — %5")
                            .arg(id).arg(cliente, query.value(3).toString())
                            .arg(query.value(4).toInt()).arg(importeFactura(importe));
                    } else {
                        resumen = QString("Venta %1 — %2 — %3 productos — %4")
                            .arg(id).arg(cliente).arg(productos).arg(importeFactura(importe));
                    }
                    referenciaCombo->addItem(resumen, id);
                    referenciaCombo->setItemData(referenciaCombo->count() - 1, importe, Qt::UserRole + 1);
                }
            }
        } else {
            ServicioDAO servicioDao(conexionBD.getConexionQt());
            QSqlQuery query = servicioDao.disponiblesParaFacturarQt();
            if (query.lastError().isValid()) {
                QMessageBox::warning(&dialog, "Base de datos", "No se pudieron cargar los servicios:\n" + query.lastError().text());
            } else {
                while (query.next()) {
                    const int id = query.value(0).toInt();
                    const QString cliente = query.value(1).toString();
                    const QString trabajo = query.value(3).toString().trimmed().isEmpty()
                        ? query.value(2).toString() : query.value(3).toString();
                    const double importe = query.value(4).toDouble();
                    const QString resumen = QString("Servicio %1 — %2 — %3 — %4")
                        .arg(id).arg(cliente, trabajo, importeFactura(importe));
                    referenciaCombo->addItem(resumen, id);
                    referenciaCombo->setItemData(referenciaCombo->count() - 1, importe, Qt::UserRole + 1);
                }
            }
        }

        referenciaCombo->setEnabled(referenciaCombo->count() > 0);
        if (referenciaCombo->count() > 0) {
            referenciaCombo->setCurrentIndex(0);
        }
    };

    connect(origenCombo, &QComboBox::currentIndexChanged, &dialog, [&]() {
        cargarReferencias();
        actualizarDetalle();
    });
    connect(referenciaCombo, &QComboBox::currentIndexChanged, &dialog, actualizarDetalle);
    cargarReferencias();
    actualizarDetalle();

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (referenciaCombo->currentIndex() < 0 || referenciaCombo->currentData().toInt() <= 0) {
            QMessageBox::warning(&dialog, "Datos incompletos",
                origenCombo->currentData().toString() == "venta"
                    ? "No hay ventas disponibles para facturar."
                    : "No hay servicios disponibles para facturar.");
            return;
        }
        if (numeroEdit->text().trimmed().isEmpty() || fechaEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "Numero, tipo y fecha son obligatorios.");
            return;
        }
        if (!leerFecha(fechaEdit->text()).isValid()) {
            QMessageBox::warning(&dialog, "Fecha incorrecta", "La fecha debe tener el formato DD/MM/AAAA.");
            return;
        }
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    if (origenCombo->currentData().toString() == "venta") {
        idVenta = referenciaCombo->currentData().toInt();
        idServicio = -1;
    } else {
        idVenta = -1;
        idServicio = referenciaCombo->currentData().toInt();
    }
    numeroFactura = numeroEdit->text().trimmed();
    tipoFactura = tipoCombo->currentText();
    fecha = fechaParaBase(fechaEdit->text());
    return true;
}



