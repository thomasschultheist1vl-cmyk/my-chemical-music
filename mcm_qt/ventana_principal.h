#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QList>
#include <QStringList>
#include "../database/ConexionBD.h"
#include "../models/Usuario.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSqlQuery;
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
    static constexpr int CodigoCerrarSesion = 1001;
    explicit MainWindow(const Usuario &usuario, QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void cerrarSesionSolicitada();

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
    QTableWidget *usuariosTable;
    QTableWidget *movimientosTable;
    QListWidget *productosBajoStockList;
    QTabWidget *configTabs;
    QLabel *connectionLabel;
    QLabel *sectionTitle;
    QLabel *sessionLabel;
    QLabel *clientesCountLabel;
    QLabel *productosCountLabel;
    QLabel *ventasCountLabel;
    QLabel *serviciosCountLabel;
    QPushButton *ventasAnularButton;
    QPushButton *serviciosEditButton;
    QPushButton *serviciosEstadoButton;
    QPushButton *serviciosAnularButton;
    QPushButton *comprasAnularButton;
    QPushButton *facturasAnularButton;
    QPushButton *facturasPagoButton;
    QPushButton *usuariosActivarButton;
    QPushButton *usuariosDesactivarButton;
    QPushButton *btnClientesAdd;
    QPushButton *btnClientesEdit;
    QPushButton *btnClientesDelete;
    QPushButton *btnProductosAdd;
    QPushButton *btnProductosEdit;
    QPushButton *btnProductosDelete;
    QPushButton *btnVentasAdd;
    QPushButton *btnServiciosAdd;
    QPushButton *btnServiciosDelete;
    QPushButton *btnComprasAdd;
    QPushButton *btnComprasDelete;
    QPushButton *btnFacturasAdd;
    QPushButton *btnFacturasDelete;
    QPushButton *btnConfigAdd;
    QPushButton *btnConfigEdit;
    QPushButton *btnConfigDelete;
    QList<QPushButton *> menuButtons;
    QHash<QString, QPushButton *> menuPorModulo;
    QHash<QString, int> dashboardMaxId;
    QHash<QString, quint64> dashboardDetectedOrder;
    quint64 dashboardSequence;
    ConexionBD conexionBD;
    Usuario usuarioActual;

    void buildInterface();
    QWidget *createDashboardPage();
    QWidget *createClientesPage();
    QWidget *createDataPage(QTableWidget **table, const QStringList &headers, const QString &module);
    QWidget *createConfiguracionPage();
    QWidget *createUsuariosPage();
    QWidget *createConfigTab(QTableWidget **table, const QStringList &headers, bool hasDescription);
    QPushButton *createMenuButton(const QString &text);
    void marcarSeccionActiva(QPushButton *activeButton);
    void aplicarPermisos();
    void cerrarSesion();
    bool puedeAdministrar() const;
    bool puedeVerModulo(const QString &module) const;
    bool puedeEditarOperacionPropia(QTableWidget *table) const;
    void connectDatabase();
    void actualizarDashboard();
    void fillTable(QTableWidget *table, const QStringList &headers, QSqlQuery query);
    bool fillCombo(QComboBox *combo, QSqlQuery query);
    int selectedComboId(QComboBox *combo) const;
    int selectedId(QTableWidget *table) const;
    int columnIndex(QTableWidget *table, const QString &header) const;
    bool rowIsAnulada(QTableWidget *table, int row) const;
    bool selectedRowIsAnulada(QTableWidget *table) const;
    bool requestAnulacionMotivo(const QString &titulo, const QString &mensaje, QString &motivo);
    void updateOperacionButtons(QTableWidget *table, QPushButton *anularButton, QPushButton *editButton = nullptr, QPushButton *stateButton = nullptr);
    void showAnulacionInfo(QTableWidget *table);
    void showDetalleSimple(const QString &titulo, const QList<QPair<QString, QString>> &campos);
    void showClienteDetails();
    void showProductoDetails();
    void showServicioDetails();
    void showFacturaDetails();
    void showConfigDetails();
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
    void loadUsuarios();
    void refreshConfigTab();
    bool showClienteDialog(QString &nombre, QString &apellido, QString &telefono, QString &email, QString &direccion, QString &dni, const QString &title);
    bool showConfigDialog(QString &nombre, QString &descripcion, bool hasDescription, const QString &title);
    bool showProductoDialog(QString &nombre, QString &descripcion, double &precioVenta, int &stock, int &stockMinimo, int &idCategoria, int &idMarca, int &idProveedor, const QString &title);
    bool showServicioDialog(int &idCliente, QString &instrumento, QString &descripcion, QString &fechaIngreso, QString &fechaEntrega, double &precio, int &idEstadoServicio, const QString &title);
    bool showVentaHeaderDialog(int &idCliente, int &idMedioPago, QString &fecha);
    bool showVentaDetalleDialog(int &idProducto, int &cantidad);
    bool showCompraHeaderDialog(int &idProveedor, QString &fecha);
    bool showCompraDetalleDialog(int &idProducto, int &cantidad, double &precioCompra);
    bool showFacturaDialog(int &idVenta, int &idServicio, QString &numeroFactura, QString &tipoFactura, QString &fecha, double &total);
    bool showUsuarioDialog(Usuario &usuario, QString &password, bool nuevoUsuario);
    int selectedClienteId() const;
    void addCliente();
    void editCliente();
    void deleteCliente();
    void addProducto();
    void editProducto();
    void deleteProducto();
    void addVenta();
    void showVentaDetails();
    void anularVenta();
    void addServicio();
    void editServicio();
    void deleteServicio();
    void changeServicioEstado();
    void anularServicio();
    void addCompra();
    void deleteCompra();
    void showCompraDetails();
    void anularCompra();
    void addFactura();
    void deleteFactura();
    void anularFactura();
    void marcarFacturaPagada();
    void addUsuario();
    void editUsuario();
    void activarUsuario();
    void desactivarUsuario();
    void showUsuarioDetails();
    void addConfigItem();
    void editConfigItem();
    void deleteConfigItem();
    void showMessage(const QString &title);
};
#endif // MAINWINDOW_H

