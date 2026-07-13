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

bool MainWindow::showServicioDialog(int &idCliente, QString &instrumento, QString &descripcion, QString &fechaIngreso, QString &fechaEntrega, double &precio, int &idEstadoServicio, const QString &title)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(460);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *clienteCombo = new QComboBox(&dialog);
    QLineEdit *instrumentoEdit = new QLineEdit(instrumento, &dialog);
    QLineEdit *descripcionEdit = new QLineEdit(descripcion, &dialog);
    QLineEdit *fechaIngresoEdit = new QLineEdit(fechaVisual(fechaIngreso), &dialog);
    QLineEdit *fechaEntregaEdit = new QLineEdit(fechaVisual(fechaEntrega), &dialog);
    QDoubleSpinBox *precioEdit = new QDoubleSpinBox(&dialog);
    QComboBox *estadoCombo = new QComboBox(&dialog);

    fechaIngresoEdit->setPlaceholderText("DD/MM/AAAA");
    fechaEntregaEdit->setPlaceholderText("DD/MM/AAAA");
    precioEdit->setMaximum(999999999);
    precioEdit->setDecimals(2);
    precioEdit->setValue(precio);

    ClienteDAO clienteDao(conexionBD.getConexionQt()); EstadoServicioDAO estadoDao(conexionBD.getConexionQt());
    if (!fillCombo(clienteCombo, clienteDao.paraComboQt()) || !fillCombo(estadoCombo, estadoDao.paraComboQt())) {
        return false;
    }

    int index = clienteCombo->findData(idCliente);
    if (index >= 0) clienteCombo->setCurrentIndex(index);
    index = estadoCombo->findData(idEstadoServicio);
    if (index >= 0) estadoCombo->setCurrentIndex(index);

    form->addRow("Cliente:", clienteCombo);
    form->addRow("Instrumento:", instrumentoEdit);
    form->addRow("Descripcion:", descripcionEdit);
    form->addRow("Fecha ingreso:", fechaIngresoEdit);
    form->addRow("Fecha entrega:", fechaEntregaEdit);
    form->addRow("Precio:", precioEdit);
    form->addRow("Estado:", estadoCombo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (instrumentoEdit->text().trimmed().isEmpty() || fechaIngresoEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "Instrumento y fecha de ingreso son obligatorios.");
            return;
        }
        if (!leerFecha(fechaIngresoEdit->text()).isValid() ||
            (!fechaEntregaEdit->text().trimmed().isEmpty() && !leerFecha(fechaEntregaEdit->text()).isValid())) {
            QMessageBox::warning(&dialog, "Fecha incorrecta", "Las fechas deben tener el formato DD/MM/AAAA.");
            return;
        }
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idCliente = selectedComboId(clienteCombo);
    instrumento = instrumentoEdit->text().trimmed();
    descripcion = descripcionEdit->text().trimmed();
    fechaIngreso = fechaParaBase(fechaIngresoEdit->text());
    fechaEntrega = fechaEntregaEdit->text().trimmed().isEmpty() ? QString() : fechaParaBase(fechaEntregaEdit->text());
    precio = precioEdit->value();
    idEstadoServicio = selectedComboId(estadoCombo);
    return true;
}



