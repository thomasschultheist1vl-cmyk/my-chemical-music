#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStringList>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;
class QTabWidget;
class QTableWidget;
class QWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QStackedWidget *pages;
    QTableWidget *clientesTable;
    QTableWidget *productosTable;
    QTableWidget *ventasTable;
    QTableWidget *serviciosTable;
    QTableWidget *comprasTable;
    QTableWidget *facturasTable;
    QTableWidget *configuracionTable;
    QTableWidget *categoriasTable;
    QTableWidget *marcasTable;
    QTableWidget *mediosPagoTable;
    QTableWidget *estadosServicioTable;
    QTabWidget *configTabs;
    QLabel *connectionLabel;
    QLabel *sectionTitle;
    QSqlDatabase db;

    void buildInterface();
    QWidget *createDashboardPage();
    QWidget *createClientesPage();
    QWidget *createDataPage(QTableWidget **table, const QStringList &headers, const QString &module);
    QWidget *createConfiguracionPage();
    QWidget *createConfigTab(QTableWidget **table, const QStringList &headers, bool hasDescription);
    QPushButton *createMenuButton(const QString &text);
    void connectDatabase();
    void fillTable(QTableWidget *table, const QStringList &headers, const QString &sql);
    bool fillCombo(QComboBox *combo, const QString &sql);
    int selectedComboId(QComboBox *combo) const;
    int selectedId(QTableWidget *table) const;
    void loadClientes();
    void loadProductos();
    void loadVentas();
    void loadServicios();
    void loadCompras();
    void loadFacturas();
    void loadConfiguracion();
    void loadCategorias();
    void loadMarcas();
    void loadMediosPago();
    void loadEstadosServicio();
    void refreshConfigTab();
    bool showClienteDialog(QString &nombre, QString &apellido, QString &telefono, QString &email, QString &direccion, QString &dni, const QString &title);
    bool showConfigDialog(QString &nombre, QString &descripcion, bool hasDescription, const QString &title);
    bool showProductoDialog(QString &nombre, QString &descripcion, double &precioVenta, int &stock, int &stockMinimo, int &idCategoria, int &idMarca, int &idProveedor, const QString &title);
    bool showServicioDialog(int &idCliente, QString &instrumento, QString &descripcion, QString &fechaIngreso, QString &fechaEntrega, double &precio, int &idEstadoServicio, const QString &title);
    bool showVentaHeaderDialog(int &idCliente, int &idMedioPago, QString &fecha);
    bool showVentaDetalleDialog(int &idProducto, int &cantidad);
    bool showCompraHeaderDialog(int &idProveedor, QString &fecha);
    bool showCompraDetalleDialog(int &idProducto, int &cantidad, double &precioCompra);
    bool showFacturaDialog(int &idVenta, QString &numeroFactura, QString &tipoFactura, QString &fecha, double &total);
    int selectedClienteId() const;
    void addCliente();
    void editCliente();
    void deleteCliente();
    void addProducto();
    void editProducto();
    void deleteProducto();
    void addVenta();
    void showVentaDetails();
    void addServicio();
    void editServicio();
    void deleteServicio();
    void changeServicioEstado();
    void addCompra();
    void deleteCompra();
    void showCompraDetails();
    void addFactura();
    void deleteFactura();
    void addConfigItem();
    void editConfigItem();
    void deleteConfigItem();
    void showMessage(const QString &title);
};
#endif // MAINWINDOW_H

