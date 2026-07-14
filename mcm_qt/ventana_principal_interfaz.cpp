#include "ventana_principal.h"
#include "./ui_ventana_principal.h"

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

namespace {
// Funciones auxiliares usadas por la interfaz para mostrar y convertir fechas.
// No consultan la base de datos: solamente adaptan el texto que ve el usuario.
QDate leerFecha(const QString &text)
{
    QDate fecha = QDate::fromString(text.trimmed(), "dd/MM/yyyy");
    if (!fecha.isValid()) {
        fecha = QDate::fromString(text.trimmed().left(10), Qt::ISODate);
    }
    return fecha;
}

QString fechaVisual(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        return {};
    }
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString("dd/MM/yyyy") : text;
}

QString fechaParaBase(const QString &text)
{
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString(Qt::ISODate) : text;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pages(nullptr)
    , clientesTable(nullptr)
    , productosTable(nullptr)
    , ventasTable(nullptr)
    , serviciosTable(nullptr)
    , comprasTable(nullptr)
    , facturasTable(nullptr)
    , configuracionTable(nullptr)
    , categoriasTable(nullptr)
    , marcasTable(nullptr)
    , mediosPagoTable(nullptr)
    , estadosServicioTable(nullptr)
    , movimientosTable(nullptr)
    , productosBajoStockList(nullptr)
    , configTabs(nullptr)
    , connectionLabel(nullptr)
    , sectionTitle(nullptr)
    , clientesCountLabel(nullptr)
    , productosCountLabel(nullptr)
    , ventasCountLabel(nullptr)
    , serviciosCountLabel(nullptr)
    , ventasAnularButton(nullptr)
    , serviciosEditButton(nullptr)
    , serviciosEstadoButton(nullptr)
    , serviciosAnularButton(nullptr)
    , comprasAnularButton(nullptr)
    , facturasAnularButton(nullptr)
    , dashboardSequence(0)
{
    // Qt carga primero la ventana base del archivo .ui. Después buildInterface()
    // construye por código el menú lateral y cada apartado de la aplicación.
    ui->setupUi(this);
    buildInterface();
    connectDatabase();
    actualizarDashboard();
    loadClientes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildInterface()
{
    // ESTRUCTURA Y ESTILO GENERAL DE TODA LA APLICACIÓN QT.
    // Aquí se definen colores, tablas, botones, campos y demás estilos compartidos.
    setWindowTitle("My Chemical Music");
    resize(1180, 720);

    qApp->setStyleSheet(R"(
        QMainWindow {
            background: #111111;
        }
        QLabel#appName {
            color: #ffffff;
            font-size: 20px;
            font-weight: 700;
        }
        QLabel#appSubtitle {
            color: #f87171;
            font-size: 12px;
        }
        QLabel#sectionTitle {
            color: #f8fafc;
            font-size: 24px;
            font-weight: 700;
        }
        QLabel#connectionLabel {
            color: #d1d5db;
            font-size: 12px;
        }
        QFrame#sidebar {
            background: #080808;
            border-right: 1px solid #2a2a2a;
        }
        QPushButton#menuButton {
            background: transparent;
            color: #e5e7eb;
            border: none;
            border-left: 4px solid transparent;
            border-radius: 6px;
            padding: 11px 14px;
            text-align: left;
            font-size: 14px;
        }
        QPushButton#menuButton[active="false"]:hover {
            background: #2a0f12;
            color: #ffffff;
            border-left: 4px solid #7f1d1d;
        }
        QPushButton#menuButton[active="true"] {
            background: #3a1117;
            color: #ffffff;
            border-left: 4px solid #dc2626;
            font-weight: 700;
        }
        QPushButton#primaryButton {
            background: #dc2626;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 14px;
            font-weight: 600;
        }
        QPushButton#primaryButton:hover {
            background: #b91c1c;
        }
        QPushButton#secondaryButton {
            background: #18181b;
            color: #f8fafc;
            border: 1px solid #3f3f46;
            border-radius: 6px;
            padding: 9px 14px;
            font-weight: 600;
        }
        QPushButton#secondaryButton:hover {
            background: #2a0f12;
            border: 1px solid #dc2626;
        }
        QFrame#card {
            background: #18181b;
            border: 1px solid #3f3f46;
            border-radius: 8px;
        }
        QLabel#cardTitle {
            color: #f87171;
            font-size: 12px;
        }
        QLabel#cardValue {
            color: #f8fafc;
            font-size: 26px;
            font-weight: 700;
        }
        QTableWidget {
            background: #18181b;
            color: #f8fafc;
            alternate-background-color: #202024;
            border: 1px solid #3f3f46;
            border-radius: 8px;
            gridline-color: #2f2f35;
            selection-background-color: #7f1d1d;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background: #0f0f10;
            color: #f8fafc;
            border: none;
            padding: 8px;
            font-weight: 700;
        }
        QLineEdit {
            background: #18181b;
            color: #f8fafc;
            border: 1px solid #3f3f46;
            border-radius: 6px;
            padding: 9px 12px;
        }
        QLineEdit:focus {
            border: 1px solid #dc2626;
        }
        QTabWidget::pane {
            border: 1px solid #3f3f46;
            border-radius: 8px;
            top: -1px;
        }
        QTabBar::tab {
            background: #18181b;
            color: #d1d5db;
            border: 1px solid #3f3f46;
            padding: 9px 14px;
            min-width: 120px;
        }
        QTabBar::tab:selected {
            background: #7f1d1d;
            color: #ffffff;
            border-color: #dc2626;
        }
        QDialog {
            background: #111111;
        }
        QDialog QLabel {
            color: #f8fafc;
        }
        QMessageBox {
            background: #111111;
        }
        QMessageBox QLabel {
            color: #f8fafc;
        }
    )");

    // Contenedor principal: menú lateral a la izquierda y contenido a la derecha.
    QWidget *root = new QWidget(this);
    QHBoxLayout *rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // MENÚ LATERAL: crea los botones que permiten cambiar de apartado.
    QFrame *sidebar = new QFrame(root);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(270);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 22, 18, 22);
    sidebarLayout->setSpacing(10);

    QLabel *appName = new QLabel("MY CHEMICAL MUSIC", sidebar);
    appName->setObjectName("appName");
    appName->setWordWrap(true);
    QLabel *appSubtitle = new QLabel("Casa de musica", sidebar);
    appSubtitle->setObjectName("appSubtitle");

    sidebarLayout->addWidget(appName);
    sidebarLayout->addWidget(appSubtitle);
    sidebarLayout->addSpacing(18);

    QPushButton *btnInicio = createMenuButton("Inicio");
    QPushButton *btnClientes = createMenuButton("Clientes");
    QPushButton *btnProductos = createMenuButton("Productos");
    QPushButton *btnVentas = createMenuButton("Ventas");
    QPushButton *btnServicios = createMenuButton("Servicios");
    QPushButton *btnCompras = createMenuButton("Compras");
    QPushButton *btnFacturacion = createMenuButton("Facturacion");
    QPushButton *btnConfig = createMenuButton("Datos generales");

    menuButtons = {btnInicio, btnClientes, btnProductos, btnVentas,
                   btnServicios, btnCompras, btnFacturacion, btnConfig};

    sidebarLayout->addWidget(btnInicio);
    sidebarLayout->addWidget(btnClientes);
    sidebarLayout->addWidget(btnProductos);
    sidebarLayout->addWidget(btnVentas);
    sidebarLayout->addWidget(btnServicios);
    sidebarLayout->addWidget(btnCompras);
    sidebarLayout->addWidget(btnFacturacion);
    sidebarLayout->addWidget(btnConfig);
    sidebarLayout->addStretch();

    // ÁREA DERECHA: contiene el título, el estado de conexión y las pantallas.
    QWidget *content = new QWidget(root);
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(28, 24, 28, 24);
    contentLayout->setSpacing(18);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    sectionTitle = new QLabel("Inicio", content);
    sectionTitle->setObjectName("sectionTitle");
    connectionLabel = new QLabel("Conectando a la base...", content);
    connectionLabel->setObjectName("connectionLabel");
    connectionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    headerLayout->addWidget(sectionTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(connectionLabel);

    // PANTALLAS DE LA APLICACIÓN.
    // QStackedWidget guarda todos los apartados y muestra uno por vez.
    // El índice indicado aquí es el mismo que se usa más abajo al navegar.
    pages = new QStackedWidget(content);
    // Índice 0: Inicio.
    pages->addWidget(createDashboardPage());
    // Índice 1: Clientes.
    pages->addWidget(createClientesPage());
    // Índice 2: Productos. createDataPage crea su buscador, botones y tabla.
    pages->addWidget(createDataPage(&productosTable, {"ID", "Nombre", "Descripcion", "Precio", "Stock", "Minimo", "Categoria", "Marca", "Proveedor"}, "productos"));
    // Índice 3: Ventas.
    pages->addWidget(createDataPage(&ventasTable, {"ID", "Cliente", "Fecha", "Total", "Medio de pago", "Estado", "Motivo anulacion", "Fecha anulacion"}, "ventas"));
    // Índice 4: Servicios.
    pages->addWidget(createDataPage(&serviciosTable, {"ID", "Cliente", "Instrumento", "Descripcion", "Ingreso", "Entrega", "Precio", "Estado", "Motivo anulacion", "Fecha anulacion"}, "servicios"));
    // Índice 5: Compras.
    pages->addWidget(createDataPage(&comprasTable, {"ID", "Proveedor", "Fecha", "Total", "Estado", "Motivo anulacion", "Fecha anulacion"}, "compras"));
    // Índice 6: Facturación.
    pages->addWidget(createDataPage(&facturasTable, {"ID factura", "Origen", "Operación facturada", "N.º de factura", "Tipo", "Fecha", "Total", "Estado", "Motivo anulacion", "Fecha anulacion"}, "facturas"));
    // Índice 7: Datos generales.
    pages->addWidget(createConfiguracionPage());

    contentLayout->addLayout(headerLayout);
    contentLayout->addWidget(pages);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);

    setCentralWidget(root);
    marcarSeccionActiva(btnInicio);

    // NAVEGACIÓN: cada botón cambia la pantalla visible, marca la opción activa
    // y vuelve a cargar los datos correspondientes desde los DAO.
    connect(btnInicio, &QPushButton::clicked, this, [this, btnInicio]() {
        sectionTitle->setText("Inicio");
        pages->setCurrentIndex(0);
        marcarSeccionActiva(btnInicio);
        actualizarDashboard();
    });
    connect(btnClientes, &QPushButton::clicked, this, [this, btnClientes]() {
        sectionTitle->setText("Clientes");
        pages->setCurrentIndex(1);
        marcarSeccionActiva(btnClientes);
        loadClientes();
    });
    connect(btnProductos, &QPushButton::clicked, this, [this, btnProductos]() {
        sectionTitle->setText("Productos");
        pages->setCurrentIndex(2);
        marcarSeccionActiva(btnProductos);
        loadProductos();
    });
    connect(btnVentas, &QPushButton::clicked, this, [this, btnVentas]() {
        sectionTitle->setText("Ventas");
        pages->setCurrentIndex(3);
        marcarSeccionActiva(btnVentas);
        loadVentas();
    });
    connect(btnServicios, &QPushButton::clicked, this, [this, btnServicios]() {
        sectionTitle->setText("Servicios");
        pages->setCurrentIndex(4);
        marcarSeccionActiva(btnServicios);
        loadServicios();
    });
    connect(btnCompras, &QPushButton::clicked, this, [this, btnCompras]() {
        sectionTitle->setText("Compras");
        pages->setCurrentIndex(5);
        marcarSeccionActiva(btnCompras);
        loadCompras();
    });
    connect(btnFacturacion, &QPushButton::clicked, this, [this, btnFacturacion]() {
        sectionTitle->setText("Facturacion");
        pages->setCurrentIndex(6);
        marcarSeccionActiva(btnFacturacion);
        loadFacturas();
    });
    connect(btnConfig, &QPushButton::clicked, this, [this, btnConfig]() {
        sectionTitle->setText("Datos generales");
        pages->setCurrentIndex(7);
        marcarSeccionActiva(btnConfig);
        loadConfiguracion();
    });
}

// ============================== INICIO ==============================
// Construye solamente la parte visual del apartado Inicio:
// tarjetas de resumen, últimos movimientos y productos bajos en stock.
// Los valores reales se cargan desde actualizarDashboard(), ubicada en el
// archivo de datos de la ventana principal.
QWidget *MainWindow::createDashboardPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QLabel *welcome = new QLabel("Panel general del sistema de gestion", page);
    welcome->setStyleSheet("font-size: 16px; color: #d1d5db;");

    QGridLayout *cards = new QGridLayout;
    cards->setSpacing(12);

    const QString cardStyle = "QFrame#card { padding: 10px; }";

    // Función local reutilizada para crear las cuatro tarjetas con el mismo estilo.
    auto makeCard = [&](const QString &title, QLabel **valueLabelPointer) {
        QFrame *card = new QFrame(page);
        card->setObjectName("card");
        card->setStyleSheet(cardStyle);
        card->setFixedHeight(118);
        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 10, 14, 10);
        cardLayout->setSpacing(4);
        QLabel *titleLabel = new QLabel(title, card);
        QLabel *valueLabel = new QLabel("0", card);
        titleLabel->setStyleSheet("color: #f87171; font-size: 16px; font-weight: 600;");
        valueLabel->setStyleSheet("color: #f8fafc; font-size: 34px; font-weight: 700;");
        *valueLabelPointer = valueLabel;
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(valueLabel);
        return card;
    };

    cards->addWidget(makeCard("Clientes registrados", &clientesCountLabel), 0, 0);
    cards->addWidget(makeCard("Productos registrados", &productosCountLabel), 0, 1);
    cards->addWidget(makeCard("Ventas registradas", &ventasCountLabel), 0, 2);
    cards->addWidget(makeCard("Servicios registrados", &serviciosCountLabel), 0, 3);

    // Cuadro izquierdo del Inicio: tabla de los últimos siete movimientos.
    QFrame *movimientosPanel = new QFrame(page);
    movimientosPanel->setObjectName("dashboardPanel");
    movimientosPanel->setStyleSheet(
        "QFrame#dashboardPanel { background: #18181b; border: 1px solid #3f3f46; border-radius: 8px; }"
    );
    QVBoxLayout *movimientosLayout = new QVBoxLayout(movimientosPanel);
    movimientosLayout->setContentsMargins(12, 12, 12, 12);
    movimientosLayout->setSpacing(8);

    QLabel *movimientosTitle = new QLabel("Últimos movimientos", movimientosPanel);
    movimientosTitle->setStyleSheet("color: #f8fafc; font-size: 18px; font-weight: 700;");

    movimientosTable = new QTableWidget(movimientosPanel);
    movimientosTable->setStyleSheet("QTableWidget { border: none; border-radius: 0px; }");
    movimientosTable->setColumnCount(3);
    movimientosTable->setHorizontalHeaderLabels({"Tipo", "Descripción", "Fecha"});
    movimientosTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    movimientosTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    movimientosTable->setSelectionMode(QAbstractItemView::SingleSelection);
    movimientosTable->setAlternatingRowColors(true);
    movimientosTable->verticalHeader()->setVisible(false);
    movimientosTable->verticalHeader()->setDefaultSectionSize(38);
    movimientosTable->horizontalHeader()->setFixedHeight(36);
    movimientosTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    movimientosTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    movimientosTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    movimientosTable->setFixedHeight(36 + (7 * 38));

    movimientosLayout->addWidget(movimientosTitle);
    movimientosLayout->addWidget(movimientosTable);

    // Cuadro derecho del Inicio: lista de productos con stock menor o igual a 3.
    QFrame *stockPanel = new QFrame(page);
    stockPanel->setObjectName("dashboardPanel");
    stockPanel->setStyleSheet(
        "QFrame#dashboardPanel { background: #18181b; border: 1px solid #3f3f46; border-radius: 8px; }"
    );
    QVBoxLayout *stockLayout = new QVBoxLayout(stockPanel);
    stockLayout->setContentsMargins(12, 12, 12, 12);
    stockLayout->setSpacing(8);

    QLabel *stockTitle = new QLabel("Productos bajos en stock", stockPanel);
    stockTitle->setStyleSheet("color: #f8fafc; font-size: 18px; font-weight: 700;");

    productosBajoStockList = new QListWidget(stockPanel);
    productosBajoStockList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    productosBajoStockList->setSelectionMode(QAbstractItemView::NoSelection);
    productosBajoStockList->setStyleSheet(
        "QListWidget { background: #18181b; color: #f8fafc; border: none; }"
        "QListWidget::item { padding: 9px 6px; border-bottom: 1px solid #2f2f35; }"
    );

    stockLayout->addWidget(stockTitle);
    stockLayout->addWidget(productosBajoStockList);

    const int panelHeight = 36 + (7 * 38) + 54;
    movimientosPanel->setFixedHeight(panelHeight);
    stockPanel->setFixedHeight(panelHeight);

    // Distribuye los dos cuadros inferiores con el mismo ancho.
    QHBoxLayout *dashboardPanels = new QHBoxLayout;
    dashboardPanels->setContentsMargins(0, 0, 0, 0);
    dashboardPanels->setSpacing(12);
    dashboardPanels->addWidget(movimientosPanel, 1);
    dashboardPanels->addWidget(stockPanel, 1);

    layout->addWidget(welcome);
    layout->addLayout(cards);
    layout->addLayout(dashboardPanels);
    layout->addStretch();
    return page;
}

// ============================= CLIENTES =============================
// Construye el buscador, los botones y la tabla del apartado Clientes.
// Las operaciones de los botones están implementadas en acciones_clientes.cpp.
// La carga de filas se realiza mediante loadClientes() en el archivo de datos.
QWidget *MainWindow::createClientesPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    QHBoxLayout *toolbar = new QHBoxLayout;
    QLineEdit *search = new QLineEdit(page);
    search->setPlaceholderText("Buscar cliente...");

    QPushButton *btnRefresh = new QPushButton("Actualizar", page);
    btnRefresh->setObjectName("primaryButton");
    QPushButton *btnAdd = new QPushButton("Agregar", page);
    btnAdd->setObjectName("secondaryButton");
    QPushButton *btnEdit = new QPushButton("Modificar", page);
    btnEdit->setObjectName("secondaryButton");
    QPushButton *btnDelete = new QPushButton("Eliminar", page);
    btnDelete->setObjectName("secondaryButton");
    QPushButton *btnDetails = new QPushButton("Ver detalles", page);
    btnDetails->setObjectName("secondaryButton");

    toolbar->addWidget(search, 1);
    toolbar->addWidget(btnRefresh);
    toolbar->addWidget(btnDetails);
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDelete);

    clientesTable = new QTableWidget(page);
    clientesTable->setColumnCount(7);
    clientesTable->setHorizontalHeaderLabels({"ID", "Nombre", "Apellido", "Telefono", "Email", "Direccion", "DNI"});
    clientesTable->horizontalHeader()->setStretchLastSection(true);
    clientesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    clientesTable->verticalHeader()->setVisible(false);
    clientesTable->setAlternatingRowColors(true);
    clientesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    clientesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(toolbar);
    layout->addWidget(clientesTable);

    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::loadClientes);
    connect(btnDetails, &QPushButton::clicked, this, &MainWindow::showClienteDetails);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addCliente);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::editCliente);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteCliente);
    connect(clientesTable, &QTableWidget::cellDoubleClicked, this, [this](int, int) {
        showClienteDetails();
    });
    // El buscador oculta visualmente las filas que no contienen el texto escrito.
    connect(search, &QLineEdit::textChanged, this, [this](const QString &text) {
        for (int row = 0; row < clientesTable->rowCount(); ++row) {
            bool match = text.trimmed().isEmpty();
            for (int col = 0; col < clientesTable->columnCount() && !match; ++col) {
                QTableWidgetItem *item = clientesTable->item(row, col);
                match = item && item->text().contains(text, Qt::CaseInsensitive);
            }
            clientesTable->setRowHidden(row, !match);
        }
    });

    return page;
}

// ===== PRODUCTOS, VENTAS, SERVICIOS, COMPRAS Y FACTURACIÓN =====
// Función visual reutilizable para esos cinco apartados
// De esta forma no se repite la misma estructura gráfica en cinco funciones.
QWidget *MainWindow::createDataPage(QTableWidget **table, const QStringList &headers, const QString &module)
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    QHBoxLayout *toolbar = new QHBoxLayout;
    QLineEdit *search = new QLineEdit(page);
    search->setPlaceholderText("Buscar...");

    QPushButton *btnRefresh = new QPushButton("Actualizar", page);
    btnRefresh->setObjectName("primaryButton");
    QPushButton *btnAdd = new QPushButton("Agregar", page);
    btnAdd->setObjectName("secondaryButton");
    QPushButton *btnEdit = new QPushButton("Modificar", page);
    btnEdit->setObjectName("secondaryButton");
    QPushButton *btnDelete = new QPushButton("Eliminar", page);
    btnDelete->setObjectName("secondaryButton");
    QPushButton *btnAnular = new QPushButton("Anular", page);
    btnAnular->setObjectName("secondaryButton");
    QPushButton *btnDetails = new QPushButton("Ver detalles", page);
    btnDetails->setObjectName("secondaryButton");
    QPushButton *btnExtra = new QPushButton("Detalles", page);
    btnExtra->setObjectName("secondaryButton");

    toolbar->addWidget(search, 1);
    toolbar->addWidget(btnRefresh);
    toolbar->addWidget(btnDetails);
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnAnular);
    toolbar->addWidget(btnDelete);
    toolbar->addWidget(btnExtra);

    // Adapta los botones visibles y sus textos según el apartado actual.
    if (module == "ventas") {
        btnAdd->setText("Registrar venta");
        btnEdit->hide();
        btnDelete->hide();
        btnAnular->setText("Anular venta");
        ventasAnularButton = btnAnular;
        btnExtra->hide();
    } else if (module == "servicios") {
        btnAdd->setText("Registrar servicio");
        btnAnular->setText("Anular servicio");
        serviciosEditButton = btnEdit;
        serviciosAnularButton = btnAnular;
        serviciosEstadoButton = btnExtra;
        btnExtra->setText("Cambiar estado");
    } else if (module == "compras") {
        btnAdd->setText("Registrar compra");
        btnEdit->hide();
        btnAnular->setText("Anular compra");
        comprasAnularButton = btnAnular;
        btnExtra->hide();
    } else if (module == "facturas") {
        btnAdd->setText("Generar factura");
        btnEdit->hide();
        btnAnular->setText("Anular factura");
        facturasAnularButton = btnAnular;
        btnExtra->hide();
    } else {
        btnAnular->hide();
        btnExtra->hide();
    }

    *table = new QTableWidget(page);
    (*table)->setColumnCount(headers.size());
    (*table)->setHorizontalHeaderLabels(headers);
    (*table)->horizontalHeader()->setStretchLastSection(true);
    (*table)->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    (*table)->verticalHeader()->setVisible(false);
    (*table)->setAlternatingRowColors(true);
    (*table)->setSelectionBehavior(QAbstractItemView::SelectRows);
    (*table)->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(toolbar);
    layout->addWidget(*table);

    QTableWidget *currentTable = *table;
    // Buscador compartido: filtra las filas que ya se encuentran en la tabla.
    connect(search, &QLineEdit::textChanged, this, [currentTable](const QString &text) {
        for (int row = 0; row < currentTable->rowCount(); ++row) {
            bool match = text.trimmed().isEmpty();
            for (int col = 0; col < currentTable->columnCount() && !match; ++col) {
                QTableWidgetItem *item = currentTable->item(row, col);
                match = item && item->text().contains(text, Qt::CaseInsensitive);
            }
            currentTable->setRowHidden(row, !match);
        }
    });

    // Actualiza la tabla correcta según el apartado que está visible.
    connect(btnRefresh, &QPushButton::clicked, this, [this]() {
        switch (pages->currentIndex()) {
            case 2: loadProductos(); break;
            case 3: loadVentas(); break;
            case 4: loadServicios(); break;
            case 5: loadCompras(); break;
            case 6: loadFacturas(); break;
            case 7: loadConfiguracion(); break;
            default: break;
        }
    });

    // Relaciona los botones gráficos con la función de acciones correspondiente.
    connect(btnAdd, &QPushButton::clicked, this, [this, module]() {
        if (module == "productos") addProducto();
        else if (module == "ventas") addVenta();
        else if (module == "servicios") addServicio();
        else if (module == "compras") addCompra();
        else if (module == "facturas") addFactura();
    });

    connect(btnEdit, &QPushButton::clicked, this, [this, module]() {
        if (module == "productos") editProducto();
        else if (module == "servicios") editServicio();
    });

    connect(btnDelete, &QPushButton::clicked, this, [this, module]() {
        if (module == "productos") deleteProducto();
        else if (module == "servicios") deleteServicio();
        else if (module == "compras") deleteCompra();
        else if (module == "facturas") deleteFactura();
    });

    connect(btnAnular, &QPushButton::clicked, this, [this, module]() {
        if (module == "ventas") anularVenta();
        else if (module == "servicios") anularServicio();
        else if (module == "compras") anularCompra();
        else if (module == "facturas") anularFactura();
    });

    connect(btnDetails, &QPushButton::clicked, this, [this, module]() {
        if (module == "productos") showProductoDetails();
        else if (module == "ventas") showVentaDetails();
        else if (module == "servicios") showServicioDetails();
        else if (module == "compras") showCompraDetails();
        else if (module == "facturas") showFacturaDetails();
    });

    connect(btnExtra, &QPushButton::clicked, this, [this, module]() {
        if (module == "servicios") changeServicioEstado();
    });

    connect(currentTable, &QTableWidget::cellDoubleClicked, this, [this, module](int, int) {
        if (module == "productos") showProductoDetails();
        else if (module == "ventas") showVentaDetails();
        else if (module == "servicios") showServicioDetails();
        else if (module == "compras") showCompraDetails();
        else if (module == "facturas") showFacturaDetails();
    });

    if (module == "ventas" || module == "servicios" || module == "compras" || module == "facturas") {
        connect(currentTable, &QTableWidget::itemSelectionChanged, this, [this, currentTable, module]() {
            if (module == "ventas") updateOperacionButtons(currentTable, ventasAnularButton);
            else if (module == "servicios") updateOperacionButtons(currentTable, serviciosAnularButton, serviciosEditButton, serviciosEstadoButton);
            else if (module == "compras") updateOperacionButtons(currentTable, comprasAnularButton);
            else if (module == "facturas") updateOperacionButtons(currentTable, facturasAnularButton);
        });
        updateOperacionButtons(currentTable, btnAnular, module == "servicios" ? btnEdit : nullptr, module == "servicios" ? btnExtra : nullptr);
    }

    return page;
}

// ========================= DATOS GENERALES ==========================
// Construye el apartado que agrupa Categorías, Marcas, Medios de pago
// y Estados de servicio. Las cuatro opciones se organizan en pestañas.
QWidget *MainWindow::createConfiguracionPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    QPushButton *btnRefresh = new QPushButton("Actualizar", page);
    btnRefresh->setObjectName("primaryButton");
    QPushButton *btnAdd = new QPushButton("Agregar", page);
    btnAdd->setObjectName("secondaryButton");
    QPushButton *btnEdit = new QPushButton("Modificar", page);
    btnEdit->setObjectName("secondaryButton");
    QPushButton *btnDelete = new QPushButton("Eliminar", page);
    btnDelete->setObjectName("secondaryButton");
    QPushButton *btnDetails = new QPushButton("Ver detalles", page);
    btnDetails->setObjectName("secondaryButton");

    // Cada pestaña recibe su propia tabla, columnas y buscador.
    configTabs = new QTabWidget(page);
    configTabs->addTab(createConfigTab(&categoriasTable, {"ID", "Nombre", "Descripcion"}, true), "Categorias");
    configTabs->addTab(createConfigTab(&marcasTable, {"ID", "Nombre"}, false), "Marcas");
    configTabs->addTab(createConfigTab(&mediosPagoTable, {"ID", "Nombre"}, false), "Medios de pago");
    configTabs->addTab(createConfigTab(&estadosServicioTable, {"ID", "Nombre"}, false), "Estados servicio");
    configTabs->setStyleSheet(
        "QTabWidget::pane { top: 12px; background-color: #18181b; }"
        "QTabBar::tab { min-width: 82px; padding: 9px 10px; }"
    );

    // Botones compartidos ubicados a la derecha de la barra de pestañas.
    QWidget *actions = new QWidget(configTabs);
    QHBoxLayout *toolbar = new QHBoxLayout(actions);
    toolbar->setContentsMargins(6, 0, 6, 0);
    toolbar->setSpacing(6);

    for (QPushButton *button : {btnRefresh, btnDetails, btnAdd, btnEdit, btnDelete}) {
        button->setFixedHeight(36);
        button->setMinimumWidth(78);
        toolbar->addWidget(button);
    }

    configTabs->setCornerWidget(actions, Qt::TopRightCorner);

    layout->addWidget(configTabs);

    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshConfigTab);
    connect(btnDetails, &QPushButton::clicked, this, &MainWindow::showConfigDetails);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addConfigItem);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::editConfigItem);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteConfigItem);
    connect(configTabs, &QTabWidget::currentChanged, this, [this]() {
        refreshConfigTab();
    });
    connect(categoriasTable, &QTableWidget::cellDoubleClicked, this, [this](int, int) { showConfigDetails(); });
    connect(marcasTable, &QTableWidget::cellDoubleClicked, this, [this](int, int) { showConfigDetails(); });
    connect(mediosPagoTable, &QTableWidget::cellDoubleClicked, this, [this](int, int) { showConfigDetails(); });
    connect(estadosServicioTable, &QTableWidget::cellDoubleClicked, this, [this](int, int) { showConfigDetails(); });

    return page;
}

// Crea el contenido gráfico reutilizable de cada pestaña de Datos generales:
// un campo de búsqueda y una tabla con las columnas recibidas.
QWidget *MainWindow::createConfigTab(QTableWidget **table, const QStringList &headers, bool hasDescription)
{
    QWidget *page = new QWidget;
    page->setObjectName("configContent");
    page->setStyleSheet("QWidget#configContent { background-color: #18181b; }");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(14);

    QLineEdit *search = new QLineEdit(page);
    search->setPlaceholderText("Buscar...");

    *table = new QTableWidget(page);
    (*table)->setStyleSheet("QTableWidget { background-color: #18181b; border: none; border-radius: 0px; }");
    (*table)->viewport()->setStyleSheet("background-color: #18181b;");
    (*table)->setColumnCount(headers.size());
    (*table)->setHorizontalHeaderLabels(headers);
    (*table)->horizontalHeader()->setStretchLastSection(true);
    (*table)->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    (*table)->verticalHeader()->setVisible(false);
    (*table)->setAlternatingRowColors(true);
    (*table)->setSelectionBehavior(QAbstractItemView::SelectRows);
    (*table)->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(search);
    layout->addWidget(*table);

    Q_UNUSED(hasDescription);
    QTableWidget *currentTable = *table;
    connect(search, &QLineEdit::textChanged, this, [currentTable](const QString &text) {
        for (int row = 0; row < currentTable->rowCount(); ++row) {
            bool match = text.trimmed().isEmpty();
            for (int col = 0; col < currentTable->columnCount() && !match; ++col) {
                QTableWidgetItem *item = currentTable->item(row, col);
                match = item && item->text().contains(text, Qt::CaseInsensitive);
            }
            currentTable->setRowHidden(row, !match);
        }
    });

    return page;
}

// Crea un botón del menú lateral con las mismas propiedades visuales.
QPushButton *MainWindow::createMenuButton(const QString &text)
{
    QPushButton *button = new QPushButton(text);
    button->setObjectName("menuButton");
    button->setProperty("active", false);
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(42);
    return button;
}

// Deja activo solamente el botón del apartado visible y actualiza su estilo.
void MainWindow::marcarSeccionActiva(QPushButton *activeButton)
{
    for (QPushButton *button : menuButtons) {
        button->setProperty("active", button == activeButton);
        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    }
}


