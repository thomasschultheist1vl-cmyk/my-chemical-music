#include "../ventana_principal.h"
#include "ui_ventana_principal.h"
#include "../../dao/CategoriaDAO.h"
#include "../../dao/ClienteDAO.h"
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

#include "../utilidades/utilidades_fecha.h"

bool MainWindow::showFacturaDialog(int &idVenta, int &idServicio, QString &numeroFactura, QString &tipoFactura, QString &fecha, double &total)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Generar factura");
    dialog.setMinimumWidth(460);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *origenCombo = new QComboBox(&dialog);
    QComboBox *referenciaCombo = new QComboBox(&dialog);
    QLineEdit *numeroEdit = new QLineEdit(numeroFactura, &dialog);
    QComboBox *tipoCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fechaVisual(fecha), &dialog);
    QLabel *totalLabel = new QLabel("$0", &dialog);
    fechaEdit->setPlaceholderText("DD/MM/AAAA");

    origenCombo->addItem("Venta", "venta");
    origenCombo->addItem("Servicio", "servicio");
    tipoCombo->addItems({"A", "B", "C"});
    const int tipoIndex = tipoCombo->findText(tipoFactura);
    if (tipoIndex >= 0) {
        tipoCombo->setCurrentIndex(tipoIndex);
    }

    auto populateReferencias = [&]() {
        referenciaCombo->clear();
        const bool esVenta = origenCombo->currentData().toString() == "venta";
        VentaDAO ventaDao(conexionBD.getConexionQt()); ServicioDAO servicioDao(conexionBD.getConexionQt());
        QSqlQuery query = esVenta ? ventaDao.paraComboQt() : servicioDao.paraComboQt();
        if (query.lastError().isValid()) {
            QMessageBox::warning(&dialog, "Base de datos", "No se pudieron cargar las opciones:\n" + query.lastError().text());
            return;
        }

        while (query.next()) {
            referenciaCombo->addItem(query.value(1).toString(), query.value(0).toInt());
        }
    };

    auto updateTotal = [&]() {
        total = 0;
        totalLabel->setText("$0.00");
        if (referenciaCombo->count() == 0) {
            return;
        }

        VentaDAO ventaDao(conexionBD.getConexionQt()); ServicioDAO servicioDao(conexionBD.getConexionQt());
        QSqlQuery query = origenCombo->currentData().toString() == "venta"
            ? ventaDao.totalQt(selectedComboId(referenciaCombo))
            : servicioDao.precioQt(selectedComboId(referenciaCombo));
        if (query.next()) {
            total = query.value(0).toDouble();
            totalLabel->setText("$" + QString::number(total, 'f', 2));
        }
    };

    auto refreshReferencias = [&]() {
        populateReferencias();
        updateTotal();
    };

    connect(origenCombo, &QComboBox::currentIndexChanged, &dialog, refreshReferencias);
    connect(referenciaCombo, &QComboBox::currentIndexChanged, &dialog, updateTotal);
    refreshReferencias();

    form->addRow("Facturar:", origenCombo);
    form->addRow("Referencia:", referenciaCombo);
    form->addRow("Numero factura:", numeroEdit);
    form->addRow("Tipo factura:", tipoCombo);
    form->addRow("Fecha:", fechaEdit);
    form->addRow("Total:", totalLabel);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (referenciaCombo->count() == 0) {
            QMessageBox::warning(&dialog, "Datos incompletos", "No hay ventas o servicios disponibles para facturar.");
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
        idVenta = selectedComboId(referenciaCombo);
        idServicio = -1;
    } else {
        idVenta = -1;
        idServicio = selectedComboId(referenciaCombo);
    }
    numeroFactura = numeroEdit->text().trimmed();
    tipoFactura = tipoCombo->currentText();
    fecha = fechaParaBase(fechaEdit->text());
    return true;
}



