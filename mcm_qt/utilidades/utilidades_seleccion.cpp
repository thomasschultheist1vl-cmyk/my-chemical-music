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

#include "utilidades_fecha.h"

int MainWindow::selectedClienteId() const
{
    if (!clientesTable || clientesTable->currentRow() < 0) {
        return -1;
    }

    QTableWidgetItem *idItem = clientesTable->item(clientesTable->currentRow(), 0);
    if (!idItem) {
        return -1;
    }

    return idItem->text().toInt();
}

int MainWindow::selectedId(QTableWidget *table) const
{
    if (!table || table->currentRow() < 0) {
        return -1;
    }

    QTableWidgetItem *idItem = table->item(table->currentRow(), 0);
    if (!idItem) {
        return -1;
    }

    return idItem->text().toInt();
}

int MainWindow::columnIndex(QTableWidget *table, const QString &header) const
{
    if (!table) {
        return -1;
    }

    for (int col = 0; col < table->columnCount(); ++col) {
        QTableWidgetItem *item = table->horizontalHeaderItem(col);
        if (item && item->text() == header) {
            return col;
        }
    }

    return -1;
}

bool MainWindow::rowIsAnulada(QTableWidget *table, int row) const
{
    const int estadoCol = columnIndex(table, "Estado");
    if (!table || row < 0 || estadoCol < 0) {
        return false;
    }

    QTableWidgetItem *estadoItem = table->item(row, estadoCol);
    if (!estadoItem) {
        return false;
    }

    const QString estado = estadoItem->text().trimmed();
    return estado == "Anulada" || estado == "Anulado";
}

bool MainWindow::selectedRowIsAnulada(QTableWidget *table) const
{
    return table && rowIsAnulada(table, table->currentRow());
}

bool MainWindow::requestAnulacionMotivo(const QString &titulo, const QString &mensaje, QString &motivo)
{
    QDialog dialog(this);
    dialog.setWindowTitle(titulo);
    dialog.setMinimumWidth(430);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *label = new QLabel(mensaje, &dialog);
    label->setWordWrap(true);
    QLineEdit *motivoEdit = new QLineEdit(&dialog);
    motivoEdit->setPlaceholderText("Motivo de anulacion");
    motivoEdit->setMaxLength(255);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Ok)->setText("Confirmar anulacion");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addWidget(label);
    layout->addWidget(motivoEdit);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        const QString texto = motivoEdit->text().trimmed();
        if (texto.isEmpty()) {
            QMessageBox::warning(&dialog, titulo, "El motivo de anulacion es obligatorio.");
            return;
        }
        motivo = texto;
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

void MainWindow::updateOperacionButtons(QTableWidget *table, QPushButton *anularButton, QPushButton *editButton, QPushButton *stateButton)
{
    const bool haySeleccion = table && table->currentRow() >= 0;
    const bool anulada = haySeleccion && selectedRowIsAnulada(table);

    if (anularButton) {
        anularButton->setEnabled(haySeleccion && !anulada);
    }
    if (editButton) {
        editButton->setEnabled(!anulada);
    }
    if (stateButton) {
        stateButton->setEnabled(!anulada);
    }
}

void MainWindow::showAnulacionInfo(QTableWidget *table)
{
    if (!table || table->currentRow() < 0 || !selectedRowIsAnulada(table)) {
        return;
    }

    const int row = table->currentRow();
    const int estadoCol = columnIndex(table, "Estado");
    const int motivoCol = columnIndex(table, "Motivo anulacion");
    const int fechaCol = columnIndex(table, "Fecha anulacion");

    const QString estado = estadoCol >= 0 && table->item(row, estadoCol) ? table->item(row, estadoCol)->text() : QString();
    const QString motivo = motivoCol >= 0 && table->item(row, motivoCol) ? table->item(row, motivoCol)->text() : QString();
    const QString fecha = fechaCol >= 0 && table->item(row, fechaCol) ? table->item(row, fechaCol)->text() : QString();

    QMessageBox::information(
        this,
        "Detalle de anulacion",
        "Estado: " + estado + "\nMotivo: " + motivo + "\nFecha anulacion: " + fecha
    );
}





