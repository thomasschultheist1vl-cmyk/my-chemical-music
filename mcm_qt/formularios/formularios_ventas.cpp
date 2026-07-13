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

bool MainWindow::showVentaHeaderDialog(int &idCliente, int &idMedioPago, QString &fecha)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Registrar venta");
    dialog.setMinimumWidth(440);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *clienteCombo = new QComboBox(&dialog);
    QComboBox *medioPagoCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fechaVisual(fecha), &dialog);
    fechaEdit->setPlaceholderText("DD/MM/AAAA");

    ClienteDAO clienteDao(conexionBD.getConexionQt()); MedioPagoDAO medioDao(conexionBD.getConexionQt());
    if (!fillCombo(clienteCombo, clienteDao.paraComboQt()) || !fillCombo(medioPagoCombo, medioDao.paraComboQt())) {
        return false;
    }

    form->addRow("Cliente:", clienteCombo);
    form->addRow("Medio de pago:", medioPagoCombo);
    form->addRow("Fecha:", fechaEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Continuar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (fechaEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "La fecha es obligatoria.");
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

    idCliente = selectedComboId(clienteCombo);
    idMedioPago = selectedComboId(medioPagoCombo);
    fecha = fechaParaBase(fechaEdit->text());
    return true;
}

bool MainWindow::showVentaDetalleDialog(int &idProducto, int &cantidad)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Agregar producto a la venta");
    dialog.setMinimumWidth(500);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *productoCombo = new QComboBox(&dialog);
    QSpinBox *cantidadEdit = new QSpinBox(&dialog);
    cantidadEdit->setMinimum(1);
    cantidadEdit->setMaximum(999999);
    cantidadEdit->setValue(1);

    ProductoDAO productoDao(conexionBD.getConexionQt());
    if (!fillCombo(productoCombo, productoDao.paraComboQt(true))) {
        return false;
    }

    form->addRow("Producto:", productoCombo);
    form->addRow("Cantidad:", cantidadEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Agregar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idProducto = selectedComboId(productoCombo);
    cantidad = cantidadEdit->value();
    return true;
}



