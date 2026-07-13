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






