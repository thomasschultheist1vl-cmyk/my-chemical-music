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

bool MainWindow::showCompraHeaderDialog(int &idProveedor, QString &fecha)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Registrar compra");
    dialog.setMinimumWidth(440);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *proveedorCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fechaVisual(fecha), &dialog);
    fechaEdit->setPlaceholderText("DD/MM/AAAA");

    ProveedorDAO proveedorDao(conexionBD.getConexionQt());
    if (!fillCombo(proveedorCombo, proveedorDao.paraComboQt())) {
        return false;
    }

    form->addRow("Proveedor:", proveedorCombo);
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

    idProveedor = selectedComboId(proveedorCombo);
    fecha = fechaParaBase(fechaEdit->text());
    return true;
}

bool MainWindow::showCompraDetalleDialog(int &idProducto, int &cantidad, double &precioCompra)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Agregar producto a la compra");
    dialog.setMinimumWidth(500);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *productoCombo = new QComboBox(&dialog);
    QSpinBox *cantidadEdit = new QSpinBox(&dialog);
    QDoubleSpinBox *precioEdit = new QDoubleSpinBox(&dialog);
    cantidadEdit->setMinimum(1);
    cantidadEdit->setMaximum(999999);
    precioEdit->setMaximum(999999999);
    precioEdit->setDecimals(2);

    ProductoDAO productoDao(conexionBD.getConexionQt());
    if (!fillCombo(productoCombo, productoDao.paraComboQt())) {
        return false;
    }

    form->addRow("Producto:", productoCombo);
    form->addRow("Cantidad:", cantidadEdit);
    form->addRow("Precio compra unitario:", precioEdit);

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
    precioCompra = precioEdit->value();
    return true;
}



