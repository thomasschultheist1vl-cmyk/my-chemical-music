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

bool MainWindow::showProductoDialog(QString &nombre, QString &descripcion, double &precioVenta, int &stock, int &stockMinimo, int &idCategoria, int &idMarca, int &idProveedor, const QString &title)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(460);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QLineEdit *nombreEdit = new QLineEdit(nombre, &dialog);
    QLineEdit *descripcionEdit = new QLineEdit(descripcion, &dialog);
    QDoubleSpinBox *precioEdit = new QDoubleSpinBox(&dialog);
    precioEdit->setMaximum(999999999);
    precioEdit->setDecimals(2);
    precioEdit->setValue(precioVenta);
    QSpinBox *stockEdit = new QSpinBox(&dialog);
    stockEdit->setMaximum(999999);
    stockEdit->setValue(stock);
    QSpinBox *stockMinimoEdit = new QSpinBox(&dialog);
    stockMinimoEdit->setMaximum(999999);
    stockMinimoEdit->setValue(stockMinimo);
    QComboBox *categoriaCombo = new QComboBox(&dialog);
    QComboBox *marcaCombo = new QComboBox(&dialog);
    QComboBox *proveedorCombo = new QComboBox(&dialog);

    CategoriaDAO categoriaDao(conexionBD.getConexionQt()); MarcaDAO marcaDao(conexionBD.getConexionQt()); ProveedorDAO proveedorDao(conexionBD.getConexionQt());
    if (!fillCombo(categoriaCombo, categoriaDao.paraComboQt()) || !fillCombo(marcaCombo, marcaDao.paraComboQt()) || !fillCombo(proveedorCombo, proveedorDao.paraComboQt())) {
        return false;
    }

    int index = categoriaCombo->findData(idCategoria);
    if (index >= 0) categoriaCombo->setCurrentIndex(index);
    index = marcaCombo->findData(idMarca);
    if (index >= 0) marcaCombo->setCurrentIndex(index);
    index = proveedorCombo->findData(idProveedor);
    if (index >= 0) proveedorCombo->setCurrentIndex(index);

    form->addRow("Nombre:", nombreEdit);
    form->addRow("Descripcion:", descripcionEdit);
    form->addRow("Precio venta:", precioEdit);
    form->addRow("Stock:", stockEdit);
    form->addRow("Stock minimo:", stockMinimoEdit);
    form->addRow("Categoria:", categoriaCombo);
    form->addRow("Marca:", marcaCombo);
    form->addRow("Proveedor:", proveedorCombo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (nombreEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "El nombre es obligatorio.");
            return;
        }
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    nombre = nombreEdit->text().trimmed();
    descripcion = descripcionEdit->text().trimmed();
    precioVenta = precioEdit->value();
    stock = stockEdit->value();
    stockMinimo = stockMinimoEdit->value();
    idCategoria = selectedComboId(categoriaCombo);
    idMarca = selectedComboId(marcaCombo);
    idProveedor = selectedComboId(proveedorCombo);
    return true;
}



