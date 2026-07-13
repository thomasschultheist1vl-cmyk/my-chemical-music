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

bool MainWindow::showClienteDialog(QString &nombre, QString &apellido, QString &telefono, QString &email, QString &direccion, QString &dni, const QString &title)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QLineEdit *nombreEdit = new QLineEdit(nombre, &dialog);
    QLineEdit *apellidoEdit = new QLineEdit(apellido, &dialog);
    QLineEdit *telefonoEdit = new QLineEdit(telefono, &dialog);
    QLineEdit *emailEdit = new QLineEdit(email, &dialog);
    QLineEdit *direccionEdit = new QLineEdit(direccion, &dialog);
    QLineEdit *dniEdit = new QLineEdit(dni, &dialog);

    form->addRow("Nombre:", nombreEdit);
    form->addRow("Apellido:", apellidoEdit);
    form->addRow("Telefono:", telefonoEdit);
    form->addRow("Email:", emailEdit);
    form->addRow("Direccion:", direccionEdit);
    form->addRow("DNI:", dniEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (nombreEdit->text().trimmed().isEmpty() || apellidoEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "Nombre y apellido son obligatorios.");
            return;
        }
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    nombre = nombreEdit->text().trimmed();
    apellido = apellidoEdit->text().trimmed();
    telefono = telefonoEdit->text().trimmed();
    email = emailEdit->text().trimmed();
    direccion = direccionEdit->text().trimmed();
    dni = dniEdit->text().trimmed();
    return true;
}



