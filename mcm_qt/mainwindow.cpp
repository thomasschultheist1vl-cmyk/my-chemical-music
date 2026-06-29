#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QApplication>
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
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStackedWidget>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

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
    , configTabs(nullptr)
    , connectionLabel(nullptr)
    , sectionTitle(nullptr)
{
    ui->setupUi(this);
    buildInterface();
    connectDatabase();
    loadClientes();
}

MainWindow::~MainWindow()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

void MainWindow::buildInterface()
{
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
            border-radius: 6px;
            padding: 11px 14px;
            text-align: left;
            font-size: 14px;
        }
        QPushButton#menuButton:hover {
            background: #2a0f12;
            color: #ffffff;
            border-left: 4px solid #dc2626;
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

    QWidget *root = new QWidget(this);
    QHBoxLayout *rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

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
    QPushButton *btnConfig = createMenuButton("Configuracion");

    sidebarLayout->addWidget(btnInicio);
    sidebarLayout->addWidget(btnClientes);
    sidebarLayout->addWidget(btnProductos);
    sidebarLayout->addWidget(btnVentas);
    sidebarLayout->addWidget(btnServicios);
    sidebarLayout->addWidget(btnCompras);
    sidebarLayout->addWidget(btnFacturacion);
    sidebarLayout->addWidget(btnConfig);
    sidebarLayout->addStretch();

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

    pages = new QStackedWidget(content);
    pages->addWidget(createDashboardPage());
    pages->addWidget(createClientesPage());
    pages->addWidget(createDataPage(&productosTable, {"ID", "Nombre", "Descripcion", "Precio", "Stock", "Minimo", "Categoria", "Marca", "Proveedor"}, "productos"));
    pages->addWidget(createDataPage(&ventasTable, {"ID", "Cliente", "Fecha", "Total", "Medio de pago"}, "ventas"));
    pages->addWidget(createDataPage(&serviciosTable, {"ID", "Cliente", "Instrumento", "Descripcion", "Ingreso", "Entrega", "Precio", "Estado"}, "servicios"));
    pages->addWidget(createDataPage(&comprasTable, {"ID", "Proveedor", "Fecha", "Total"}, "compras"));
    pages->addWidget(createDataPage(&facturasTable, {"ID", "Venta", "Numero", "Tipo", "Fecha", "Total"}, "facturas"));
    pages->addWidget(createConfiguracionPage());

    contentLayout->addLayout(headerLayout);
    contentLayout->addWidget(pages);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);

    setCentralWidget(root);

    connect(btnInicio, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Inicio");
        pages->setCurrentIndex(0);
    });
    connect(btnClientes, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Clientes");
        pages->setCurrentIndex(1);
        loadClientes();
    });
    connect(btnProductos, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Productos");
        pages->setCurrentIndex(2);
        loadProductos();
    });
    connect(btnVentas, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Ventas");
        pages->setCurrentIndex(3);
        loadVentas();
    });
    connect(btnServicios, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Servicios");
        pages->setCurrentIndex(4);
        loadServicios();
    });
    connect(btnCompras, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Compras");
        pages->setCurrentIndex(5);
        loadCompras();
    });
    connect(btnFacturacion, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Facturacion");
        pages->setCurrentIndex(6);
        loadFacturas();
    });
    connect(btnConfig, &QPushButton::clicked, this, [this]() {
        sectionTitle->setText("Configuracion");
        pages->setCurrentIndex(7);
        loadConfiguracion();
    });
}

QWidget *MainWindow::createDashboardPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    QLabel *welcome = new QLabel("Panel general del sistema de gestion", page);
    welcome->setStyleSheet("font-size: 16px; color: #d1d5db;");

    QGridLayout *cards = new QGridLayout;
    cards->setSpacing(16);

    const QString cardStyle = "QFrame#card { padding: 16px; }";

    auto makeCard = [&](const QString &title, const QString &value) {
        QFrame *card = new QFrame(page);
        card->setObjectName("card");
        card->setStyleSheet(cardStyle);
        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        QLabel *titleLabel = new QLabel(title, card);
        QLabel *valueLabel = new QLabel(value, card);
        titleLabel->setObjectName("cardTitle");
        valueLabel->setObjectName("cardValue");
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(valueLabel);
        return card;
    };

    cards->addWidget(makeCard("Clientes", "Gestion"), 0, 0);
    cards->addWidget(makeCard("Productos", "Stock"), 0, 1);
    cards->addWidget(makeCard("Ventas", "Facturas"), 0, 2);
    cards->addWidget(makeCard("Servicios", "Arreglos"), 1, 0);
    cards->addWidget(makeCard("Compras", "Proveedores"), 1, 1);
    cards->addWidget(makeCard("Base de datos", "MySQL + ODBC"), 1, 2);

    QLabel *note = new QLabel("Panel inicial de la interfaz grafica del sistema My Chemical Music.", page);
    note->setStyleSheet("color: #9ca3af; font-size: 13px;");

    layout->addWidget(welcome);
    layout->addLayout(cards);
    layout->addWidget(note);
    layout->addStretch();
    return page;
}

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

    toolbar->addWidget(search, 1);
    toolbar->addWidget(btnRefresh);
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
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addCliente);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::editCliente);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteCliente);
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
    QPushButton *btnExtra = new QPushButton("Detalles", page);
    btnExtra->setObjectName("secondaryButton");

    toolbar->addWidget(search, 1);
    toolbar->addWidget(btnRefresh);
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDelete);
    toolbar->addWidget(btnExtra);

    if (module == "ventas") {
        btnAdd->setText("Registrar venta");
        btnEdit->hide();
        btnDelete->hide();
        btnExtra->setText("Detalles");
    } else if (module == "servicios") {
        btnAdd->setText("Registrar servicio");
        btnExtra->setText("Cambiar estado");
    } else if (module == "compras") {
        btnAdd->setText("Registrar compra");
        btnEdit->hide();
        btnExtra->setText("Detalles");
    } else if (module == "facturas") {
        btnAdd->setText("Generar factura");
        btnEdit->hide();
        btnExtra->hide();
    } else {
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

    connect(btnExtra, &QPushButton::clicked, this, [this, module]() {
        if (module == "ventas") showVentaDetails();
        else if (module == "servicios") changeServicioEstado();
        else if (module == "compras") showCompraDetails();
    });

    return page;
}

QWidget *MainWindow::createConfiguracionPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    QHBoxLayout *toolbar = new QHBoxLayout;

    QPushButton *btnRefresh = new QPushButton("Actualizar", page);
    btnRefresh->setObjectName("primaryButton");
    QPushButton *btnAdd = new QPushButton("Agregar", page);
    btnAdd->setObjectName("secondaryButton");
    QPushButton *btnEdit = new QPushButton("Modificar", page);
    btnEdit->setObjectName("secondaryButton");
    QPushButton *btnDelete = new QPushButton("Eliminar", page);
    btnDelete->setObjectName("secondaryButton");

    toolbar->addStretch();
    toolbar->addWidget(btnRefresh);
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDelete);

    configTabs = new QTabWidget(page);
    configTabs->addTab(createConfigTab(&categoriasTable, {"ID", "Nombre", "Descripcion"}, true), "Categorias");
    configTabs->addTab(createConfigTab(&marcasTable, {"ID", "Nombre"}, false), "Marcas");
    configTabs->addTab(createConfigTab(&mediosPagoTable, {"ID", "Nombre"}, false), "Medios de pago");
    configTabs->addTab(createConfigTab(&estadosServicioTable, {"ID", "Nombre"}, false), "Estados servicio");

    layout->addLayout(toolbar);
    layout->addWidget(configTabs);

    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshConfigTab);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addConfigItem);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::editConfigItem);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteConfigItem);
    connect(configTabs, &QTabWidget::currentChanged, this, [this]() {
        refreshConfigTab();
    });

    return page;
}

QWidget *MainWindow::createConfigTab(QTableWidget **table, const QStringList &headers, bool hasDescription)
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    QLineEdit *search = new QLineEdit(page);
    search->setPlaceholderText("Buscar...");

    *table = new QTableWidget(page);
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

QPushButton *MainWindow::createMenuButton(const QString &text)
{
    QPushButton *button = new QPushButton(text);
    button->setObjectName("menuButton");
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(42);
    return button;
}

void MainWindow::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC", "mcm_connection");
    db.setDatabaseName("my_chemical_music64");
    db.setUserName("root");
    db.setPassword("");

    if (!db.open()) {
        connectionLabel->setText("Sin conexion ODBC 64: my_chemical_music64");
        connectionLabel->setStyleSheet("color: #f87171;");
        return;
    }

    connectionLabel->setText("Conectado a MySQL por ODBC");
    connectionLabel->setStyleSheet("color: #22c55e;");
}

void MainWindow::fillTable(QTableWidget *table, const QStringList &headers, const QString &sql)
{
    if (!table) {
        return;
    }

    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setRowCount(0);

    if (!db.isOpen()) {
        return;
    }

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        QMessageBox::warning(this, "Base de datos", "No se pudieron cargar los datos:\n" + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}

bool MainWindow::fillCombo(QComboBox *combo, const QString &sql)
{
    if (!combo || !db.isOpen()) {
        return false;
    }

    combo->clear();

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        QMessageBox::warning(this, "Base de datos", "No se pudieron cargar las opciones:\n" + query.lastError().text());
        return false;
    }

    while (query.next()) {
        combo->addItem(query.value(1).toString(), query.value(0).toInt());
    }

    return combo->count() > 0;
}

int MainWindow::selectedComboId(QComboBox *combo) const
{
    if (!combo || combo->currentIndex() < 0) {
        return -1;
    }

    return combo->currentData().toInt();
}

void MainWindow::loadClientes()
{
    fillTable(
        clientesTable,
        {"ID", "Nombre", "Apellido", "Telefono", "Email", "Direccion", "DNI"},
        "SELECT id_cliente, nombre, apellido, telefono, email, direccion, dni FROM clientes ORDER BY id_cliente"
    );
}

void MainWindow::loadProductos()
{
    fillTable(
        productosTable,
        {"ID", "Nombre", "Descripcion", "Precio", "Stock", "Minimo", "Categoria", "Marca", "Proveedor"},
        "SELECT p.id_producto, p.nombre, p.descripcion, p.precio_venta, p.stock, p.stock_minimo, "
        "c.nombre, m.nombre, pr.nombre "
        "FROM productos p "
        "LEFT JOIN categorias c ON p.id_categoria = c.id_categoria "
        "LEFT JOIN marcas m ON p.id_marca = m.id_marca "
        "LEFT JOIN proveedores pr ON p.id_proveedor = pr.id_proveedor "
        "ORDER BY p.id_producto"
    );
}

void MainWindow::loadVentas()
{
    fillTable(
        ventasTable,
        {"ID", "Cliente", "Fecha", "Total", "Medio de pago"},
        "SELECT v.id_venta, CONCAT(c.nombre, ' ', c.apellido), v.fecha, v.total, mp.nombre "
        "FROM ventas v "
        "LEFT JOIN clientes c ON v.id_cliente = c.id_cliente "
        "LEFT JOIN medios_pago mp ON v.id_medio_pago = mp.id_medio_pago "
        "ORDER BY v.id_venta DESC"
    );
}

void MainWindow::loadServicios()
{
    fillTable(
        serviciosTable,
        {"ID", "Cliente", "Instrumento", "Descripcion", "Ingreso", "Entrega", "Precio", "Estado"},
        "SELECT s.id_servicio, CONCAT(c.nombre, ' ', c.apellido), s.instrumento, s.descripcion, "
        "s.fecha_ingreso, s.fecha_entrega, s.precio, es.nombre "
        "FROM servicios s "
        "LEFT JOIN clientes c ON s.id_cliente = c.id_cliente "
        "LEFT JOIN estados_servicio es ON s.id_estado_servicio = es.id_estado_servicio "
        "ORDER BY s.id_servicio DESC"
    );
}

void MainWindow::loadCompras()
{
    fillTable(
        comprasTable,
        {"ID", "Proveedor", "Fecha", "Total"},
        "SELECT cp.id_compra, p.nombre, cp.fecha, cp.total "
        "FROM compras_proveedores cp "
        "LEFT JOIN proveedores p ON cp.id_proveedor = p.id_proveedor "
        "ORDER BY cp.id_compra DESC"
    );
}

void MainWindow::loadFacturas()
{
    fillTable(
        facturasTable,
        {"ID", "Venta", "Numero", "Tipo", "Fecha", "Total"},
        "SELECT id_factura, id_venta, numero_factura, tipo_factura, fecha, total "
        "FROM facturas "
        "ORDER BY id_factura DESC"
    );
}

void MainWindow::loadConfiguracion()
{
    loadCategorias();
    loadMarcas();
    loadMediosPago();
    loadEstadosServicio();
}

void MainWindow::loadCategorias()
{
    fillTable(
        categoriasTable,
        {"ID", "Nombre", "Descripcion"},
        "SELECT id_categoria, nombre, descripcion FROM categorias ORDER BY id_categoria"
    );
}

void MainWindow::loadMarcas()
{
    fillTable(
        marcasTable,
        {"ID", "Nombre"},
        "SELECT id_marca, nombre FROM marcas ORDER BY id_marca"
    );
}

void MainWindow::loadMediosPago()
{
    fillTable(
        mediosPagoTable,
        {"ID", "Nombre"},
        "SELECT id_medio_pago, nombre FROM medios_pago ORDER BY id_medio_pago"
    );
}

void MainWindow::loadEstadosServicio()
{
    fillTable(
        estadosServicioTable,
        {"ID", "Nombre"},
        "SELECT id_estado_servicio, nombre FROM estados_servicio ORDER BY id_estado_servicio"
    );
}

void MainWindow::refreshConfigTab()
{
    if (!configTabs) {
        return;
    }

    switch (configTabs->currentIndex()) {
        case 0: loadCategorias(); break;
        case 1: loadMarcas(); break;
        case 2: loadMediosPago(); break;
        case 3: loadEstadosServicio(); break;
        default: break;
    }
}
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

bool MainWindow::showConfigDialog(QString &nombre, QString &descripcion, bool hasDescription, const QString &title)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QLineEdit *nombreEdit = new QLineEdit(nombre, &dialog);
    QLineEdit *descripcionEdit = new QLineEdit(descripcion, &dialog);

    form->addRow("Nombre:", nombreEdit);
    if (hasDescription) {
        form->addRow("Descripcion:", descripcionEdit);
    }

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
    return true;
}

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

    if (!fillCombo(categoriaCombo, "SELECT id_categoria, nombre FROM categorias ORDER BY nombre") ||
        !fillCombo(marcaCombo, "SELECT id_marca, nombre FROM marcas ORDER BY nombre") ||
        !fillCombo(proveedorCombo, "SELECT id_proveedor, nombre FROM proveedores ORDER BY nombre")) {
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
    QLineEdit *fechaIngresoEdit = new QLineEdit(fechaIngreso, &dialog);
    QLineEdit *fechaEntregaEdit = new QLineEdit(fechaEntrega, &dialog);
    QDoubleSpinBox *precioEdit = new QDoubleSpinBox(&dialog);
    QComboBox *estadoCombo = new QComboBox(&dialog);

    fechaIngresoEdit->setPlaceholderText("YYYY-MM-DD");
    fechaEntregaEdit->setPlaceholderText("YYYY-MM-DD");
    precioEdit->setMaximum(999999999);
    precioEdit->setDecimals(2);
    precioEdit->setValue(precio);

    if (!fillCombo(clienteCombo, "SELECT id_cliente, CONCAT(nombre, ' ', apellido) FROM clientes ORDER BY apellido, nombre") ||
        !fillCombo(estadoCombo, "SELECT id_estado_servicio, nombre FROM estados_servicio ORDER BY id_estado_servicio")) {
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
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idCliente = selectedComboId(clienteCombo);
    instrumento = instrumentoEdit->text().trimmed();
    descripcion = descripcionEdit->text().trimmed();
    fechaIngreso = fechaIngresoEdit->text().trimmed();
    fechaEntrega = fechaEntregaEdit->text().trimmed();
    precio = precioEdit->value();
    idEstadoServicio = selectedComboId(estadoCombo);
    return true;
}

bool MainWindow::showVentaHeaderDialog(int &idCliente, int &idMedioPago, QString &fecha)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Registrar venta");
    dialog.setMinimumWidth(440);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *clienteCombo = new QComboBox(&dialog);
    QComboBox *medioPagoCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fecha, &dialog);
    fechaEdit->setPlaceholderText("YYYY-MM-DD");

    if (!fillCombo(clienteCombo, "SELECT id_cliente, CONCAT(nombre, ' ', apellido) FROM clientes ORDER BY apellido, nombre") ||
        !fillCombo(medioPagoCombo, "SELECT id_medio_pago, nombre FROM medios_pago ORDER BY id_medio_pago")) {
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
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idCliente = selectedComboId(clienteCombo);
    idMedioPago = selectedComboId(medioPagoCombo);
    fecha = fechaEdit->text().trimmed();
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

    if (!fillCombo(productoCombo, "SELECT id_producto, CONCAT(nombre, ' - Stock: ', stock, ' - $', precio_venta) FROM productos ORDER BY nombre")) {
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

bool MainWindow::showCompraHeaderDialog(int &idProveedor, QString &fecha)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Registrar compra");
    dialog.setMinimumWidth(440);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *proveedorCombo = new QComboBox(&dialog);
    QLineEdit *fechaEdit = new QLineEdit(fecha, &dialog);
    fechaEdit->setPlaceholderText("YYYY-MM-DD");

    if (!fillCombo(proveedorCombo, "SELECT id_proveedor, nombre FROM proveedores ORDER BY nombre")) {
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
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idProveedor = selectedComboId(proveedorCombo);
    fecha = fechaEdit->text().trimmed();
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

    if (!fillCombo(productoCombo, "SELECT id_producto, nombre FROM productos ORDER BY nombre")) {
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

bool MainWindow::showFacturaDialog(int &idVenta, QString &numeroFactura, QString &tipoFactura, QString &fecha, double &total)
{
    QDialog dialog(this);
    dialog.setWindowTitle("Generar factura");
    dialog.setMinimumWidth(460);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;

    QComboBox *ventaCombo = new QComboBox(&dialog);
    QLineEdit *numeroEdit = new QLineEdit(numeroFactura, &dialog);
    QLineEdit *tipoEdit = new QLineEdit(tipoFactura, &dialog);
    QLineEdit *fechaEdit = new QLineEdit(fecha, &dialog);
    QLabel *totalLabel = new QLabel("$0", &dialog);
    fechaEdit->setPlaceholderText("YYYY-MM-DD");

    if (!fillCombo(ventaCombo, "SELECT id_venta, CONCAT('Venta ', id_venta, ' - $', total) FROM ventas ORDER BY id_venta DESC")) {
        return false;
    }

    auto updateTotal = [&]() {
        QSqlQuery query(db);
        query.prepare("SELECT total FROM ventas WHERE id_venta = ?");
        query.addBindValue(selectedComboId(ventaCombo));
        if (query.exec() && query.next()) {
            total = query.value(0).toDouble();
            totalLabel->setText("$" + QString::number(total, 'f', 2));
        }
    };
    connect(ventaCombo, &QComboBox::currentIndexChanged, &dialog, updateTotal);
    updateTotal();

    form->addRow("Venta:", ventaCombo);
    form->addRow("Numero factura:", numeroEdit);
    form->addRow("Tipo factura:", tipoEdit);
    form->addRow("Fecha:", fechaEdit);
    form->addRow("Total:", totalLabel);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (numeroEdit->text().trimmed().isEmpty() || tipoEdit->text().trimmed().isEmpty() || fechaEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Datos incompletos", "Numero, tipo y fecha son obligatorios.");
            return;
        }
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    idVenta = selectedComboId(ventaCombo);
    numeroFactura = numeroEdit->text().trimmed();
    tipoFactura = tipoEdit->text().trimmed();
    fecha = fechaEdit->text().trimmed();
    return true;
}

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

void MainWindow::addCliente()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre, apellido, telefono, email, direccion, dni;
    if (!showClienteDialog(nombre, apellido, telefono, email, direccion, dni, "Agregar cliente")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO clientes (nombre, apellido, telefono, email, direccion, dni) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(nombre);
    query.addBindValue(apellido);
    query.addBindValue(telefono);
    query.addBindValue(email);
    query.addBindValue(direccion);
    query.addBindValue(dni);

    if (!query.exec()) {
        QMessageBox::warning(this, "Clientes", "No se pudo agregar el cliente:\n" + query.lastError().text());
        return;
    }

    loadClientes();
}
void MainWindow::editCliente()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    int id = selectedClienteId();
    if (id <= 0) {
        QMessageBox::information(this, "Modificar cliente", "Selecciona un cliente de la tabla.");
        return;
    }

    int row = clientesTable->currentRow();
    QString nombre = clientesTable->item(row, 1)->text();
    QString apellido = clientesTable->item(row, 2)->text();
    QString telefono = clientesTable->item(row, 3)->text();
    QString email = clientesTable->item(row, 4)->text();
    QString direccion = clientesTable->item(row, 5)->text();
    QString dni = clientesTable->item(row, 6)->text();

    if (!showClienteDialog(nombre, apellido, telefono, email, direccion, dni, "Modificar cliente")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE clientes SET nombre = ?, apellido = ?, telefono = ?, email = ?, direccion = ?, dni = ? WHERE id_cliente = ?");
    query.addBindValue(nombre);
    query.addBindValue(apellido);
    query.addBindValue(telefono);
    query.addBindValue(email);
    query.addBindValue(direccion);
    query.addBindValue(dni);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Clientes", "No se pudo modificar el cliente:\n" + query.lastError().text());
        return;
    }

    loadClientes();
}
void MainWindow::deleteCliente()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Clientes", "No hay conexion con la base de datos.");
        return;
    }

    int id = selectedClienteId();
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar cliente", "Selecciona un cliente de la tabla.");
        return;
    }

    QString nombre = clientesTable->item(clientesTable->currentRow(), 1)->text();
    QString apellido = clientesTable->item(clientesTable->currentRow(), 2)->text();

    int answer = QMessageBox::question(
        this,
        "Eliminar cliente",
        "Seguro que queres eliminar a " + nombre + " " + apellido + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM clientes WHERE id_cliente = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Clientes", "No se pudo eliminar el cliente:\n" + query.lastError().text());
        return;
    }

    loadClientes();
}

void MainWindow::addProducto()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre, descripcion;
    double precioVenta = 0;
    int stock = 0;
    int stockMinimo = 0;
    int idCategoria = -1;
    int idMarca = -1;
    int idProveedor = -1;

    if (!showProductoDialog(nombre, descripcion, precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor, "Agregar producto")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO productos (nombre, descripcion, precio_venta, stock, stock_minimo, id_categoria, id_marca, id_proveedor) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(nombre);
    query.addBindValue(descripcion);
    query.addBindValue(precioVenta);
    query.addBindValue(stock);
    query.addBindValue(stockMinimo);
    query.addBindValue(idCategoria);
    query.addBindValue(idMarca);
    query.addBindValue(idProveedor);

    if (!query.exec()) {
        QMessageBox::warning(this, "Productos", "No se pudo agregar el producto:\n" + query.lastError().text());
        return;
    }

    loadProductos();
}

void MainWindow::editProducto()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(productosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar producto", "Selecciona un producto de la tabla.");
        return;
    }

    QSqlQuery load(db);
    load.prepare("SELECT nombre, descripcion, precio_venta, stock, stock_minimo, id_categoria, id_marca, id_proveedor FROM productos WHERE id_producto = ?");
    load.addBindValue(id);
    if (!load.exec() || !load.next()) {
        QMessageBox::warning(this, "Productos", "No se pudo cargar el producto seleccionado.");
        return;
    }

    QString nombre = load.value(0).toString();
    QString descripcion = load.value(1).toString();
    double precioVenta = load.value(2).toDouble();
    int stock = load.value(3).toInt();
    int stockMinimo = load.value(4).toInt();
    int idCategoria = load.value(5).toInt();
    int idMarca = load.value(6).toInt();
    int idProveedor = load.value(7).toInt();

    if (!showProductoDialog(nombre, descripcion, precioVenta, stock, stockMinimo, idCategoria, idMarca, idProveedor, "Modificar producto")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE productos SET nombre = ?, descripcion = ?, precio_venta = ?, stock = ?, stock_minimo = ?, id_categoria = ?, id_marca = ?, id_proveedor = ? WHERE id_producto = ?");
    query.addBindValue(nombre);
    query.addBindValue(descripcion);
    query.addBindValue(precioVenta);
    query.addBindValue(stock);
    query.addBindValue(stockMinimo);
    query.addBindValue(idCategoria);
    query.addBindValue(idMarca);
    query.addBindValue(idProveedor);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Productos", "No se pudo modificar el producto:\n" + query.lastError().text());
        return;
    }

    loadProductos();
}

void MainWindow::deleteProducto()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Productos", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(productosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar producto", "Selecciona un producto de la tabla.");
        return;
    }

    const QString nombre = productosTable->item(productosTable->currentRow(), 1)->text();
    if (QMessageBox::question(this, "Eliminar producto", "Seguro que queres eliminar \"" + nombre + "\"?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM productos WHERE id_producto = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Productos", "No se pudo eliminar el producto:\n" + query.lastError().text());
        return;
    }

    loadProductos();
}

void MainWindow::addVenta()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Ventas", "No hay conexion con la base de datos.");
        return;
    }

    int idCliente = -1;
    int idMedioPago = -1;
    QString fecha;

    if (!showVentaHeaderDialog(idCliente, idMedioPago, fecha)) {
        return;
    }

    if (!db.transaction()) {
        QMessageBox::warning(this, "Ventas", "No se pudo iniciar la venta:\n" + db.lastError().text());
        return;
    }

    QSqlQuery venta(db);
    venta.prepare("INSERT INTO ventas (id_cliente, fecha, total, id_medio_pago) VALUES (?, ?, 0, ?)");
    venta.addBindValue(idCliente);
    venta.addBindValue(fecha);
    venta.addBindValue(idMedioPago);

    if (!venta.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Ventas", "No se pudo registrar la venta:\n" + venta.lastError().text());
        return;
    }

    QSqlQuery ultimoId(db);
    int idVenta = -1;
    if (ultimoId.exec("SELECT LAST_INSERT_ID()") && ultimoId.next()) {
        idVenta = ultimoId.value(0).toInt();
    }

    double totalVenta = 0;
    int detalles = 0;

    while (true) {
        int idProducto = -1;
        int cantidad = 1;
        if (!showVentaDetalleDialog(idProducto, cantidad)) {
            break;
        }

        QSqlQuery producto(db);
        producto.prepare("SELECT precio_venta, stock FROM productos WHERE id_producto = ?");
        producto.addBindValue(idProducto);
        if (!producto.exec() || !producto.next()) {
            db.rollback();
            QMessageBox::warning(this, "Ventas", "No se pudo cargar el producto seleccionado.");
            return;
        }

        const double precioUnitario = producto.value(0).toDouble();
        const int stockActual = producto.value(1).toInt();
        if (stockActual < cantidad) {
            QMessageBox::warning(this, "Ventas", "No hay stock suficiente para ese producto.");
            continue;
        }

        const double subtotal = precioUnitario * cantidad;

        QSqlQuery detalle(db);
        detalle.prepare("INSERT INTO detalle_ventas (id_venta, id_producto, cantidad, precio_unitario, subtotal) VALUES (?, ?, ?, ?, ?)");
        detalle.addBindValue(idVenta);
        detalle.addBindValue(idProducto);
        detalle.addBindValue(cantidad);
        detalle.addBindValue(precioUnitario);
        detalle.addBindValue(subtotal);

        QSqlQuery stockQuery(db);
        stockQuery.prepare("UPDATE productos SET stock = stock - ? WHERE id_producto = ?");
        stockQuery.addBindValue(cantidad);
        stockQuery.addBindValue(idProducto);

        if (!detalle.exec() || !stockQuery.exec()) {
            db.rollback();
            QMessageBox::warning(this, "Ventas", "No se pudo agregar el detalle de venta:\n" + detalle.lastError().text() + stockQuery.lastError().text());
            return;
        }

        totalVenta += subtotal;
        ++detalles;

        if (QMessageBox::question(this, "Venta", "Producto agregado. Queres agregar otro producto?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            break;
        }
    }

    if (detalles == 0) {
        db.rollback();
        QMessageBox::information(this, "Ventas", "La venta se cancelo porque no agregaste productos.");
        return;
    }

    QSqlQuery total(db);
    total.prepare("UPDATE ventas SET total = ? WHERE id_venta = ?");
    total.addBindValue(totalVenta);
    total.addBindValue(idVenta);

    if (!total.exec() || !db.commit()) {
        db.rollback();
        QMessageBox::warning(this, "Ventas", "No se pudo finalizar la venta:\n" + total.lastError().text());
        return;
    }

    QMessageBox::information(this, "Ventas", "Venta registrada.\nID venta: " + QString::number(idVenta) + "\nTotal: $" + QString::number(totalVenta, 'f', 2));
    loadVentas();
    loadProductos();
}

void MainWindow::showVentaDetails()
{
    const int id = selectedId(ventasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalles de venta", "Selecciona una venta de la tabla.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Detalles de venta " + QString::number(id));
    dialog.resize(760, 420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTableWidget *table = new QTableWidget(&dialog);
    layout->addWidget(table);

    fillTable(
        table,
        {"Producto", "Cantidad", "Precio unitario", "Subtotal"},
        "SELECT p.nombre, dv.cantidad, dv.precio_unitario, dv.subtotal "
        "FROM detalle_ventas dv "
        "LEFT JOIN productos p ON dv.id_producto = p.id_producto "
        "WHERE dv.id_venta = " + QString::number(id)
    );

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::addServicio()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    int idCliente = -1;
    QString instrumento, descripcion, fechaIngreso, fechaEntrega;
    double precio = 0;
    int idEstadoServicio = -1;

    if (!showServicioDialog(idCliente, instrumento, descripcion, fechaIngreso, fechaEntrega, precio, idEstadoServicio, "Registrar servicio")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO servicios (id_cliente, instrumento, descripcion, fecha_ingreso, fecha_entrega, precio, id_estado_servicio) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(idCliente);
    query.addBindValue(instrumento);
    query.addBindValue(descripcion);
    query.addBindValue(fechaIngreso);
    query.addBindValue(fechaEntrega);
    query.addBindValue(precio);
    query.addBindValue(idEstadoServicio);

    if (!query.exec()) {
        QMessageBox::warning(this, "Servicios", "No se pudo registrar el servicio:\n" + query.lastError().text());
        return;
    }

    loadServicios();
}

void MainWindow::editServicio()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar servicio", "Selecciona un servicio de la tabla.");
        return;
    }

    QSqlQuery load(db);
    load.prepare("SELECT id_cliente, instrumento, descripcion, fecha_ingreso, fecha_entrega, precio, id_estado_servicio FROM servicios WHERE id_servicio = ?");
    load.addBindValue(id);
    if (!load.exec() || !load.next()) {
        QMessageBox::warning(this, "Servicios", "No se pudo cargar el servicio seleccionado.");
        return;
    }

    int idCliente = load.value(0).toInt();
    QString instrumento = load.value(1).toString();
    QString descripcion = load.value(2).toString();
    QString fechaIngreso = load.value(3).toString();
    QString fechaEntrega = load.value(4).toString();
    double precio = load.value(5).toDouble();
    int idEstadoServicio = load.value(6).toInt();

    if (!showServicioDialog(idCliente, instrumento, descripcion, fechaIngreso, fechaEntrega, precio, idEstadoServicio, "Modificar servicio")) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE servicios SET id_cliente = ?, instrumento = ?, descripcion = ?, fecha_ingreso = ?, fecha_entrega = ?, precio = ?, id_estado_servicio = ? WHERE id_servicio = ?");
    query.addBindValue(idCliente);
    query.addBindValue(instrumento);
    query.addBindValue(descripcion);
    query.addBindValue(fechaIngreso);
    query.addBindValue(fechaEntrega);
    query.addBindValue(precio);
    query.addBindValue(idEstadoServicio);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Servicios", "No se pudo modificar el servicio:\n" + query.lastError().text());
        return;
    }

    loadServicios();
}

void MainWindow::deleteServicio()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar servicio", "Selecciona un servicio de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar servicio", "Seguro que queres eliminar el servicio seleccionado?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM servicios WHERE id_servicio = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Servicios", "No se pudo eliminar el servicio:\n" + query.lastError().text());
        return;
    }

    loadServicios();
}

void MainWindow::changeServicioEstado()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Servicios", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(serviciosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Cambiar estado", "Selecciona un servicio de la tabla.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Cambiar estado del servicio");
    dialog.setMinimumWidth(360);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;
    QComboBox *estadoCombo = new QComboBox(&dialog);

    if (!fillCombo(estadoCombo, "SELECT id_estado_servicio, nombre FROM estados_servicio ORDER BY id_estado_servicio")) {
        return;
    }

    form->addRow("Nuevo estado:", estadoCombo);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE servicios SET id_estado_servicio = ? WHERE id_servicio = ?");
    query.addBindValue(selectedComboId(estadoCombo));
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Servicios", "No se pudo cambiar el estado:\n" + query.lastError().text());
        return;
    }

    loadServicios();
}

void MainWindow::addCompra()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Compras", "No hay conexion con la base de datos.");
        return;
    }

    int idProveedor = -1;
    QString fecha;

    if (!showCompraHeaderDialog(idProveedor, fecha)) {
        return;
    }

    if (!db.transaction()) {
        QMessageBox::warning(this, "Compras", "No se pudo iniciar la compra:\n" + db.lastError().text());
        return;
    }

    QSqlQuery compra(db);
    compra.prepare("INSERT INTO compras_proveedores (id_proveedor, fecha, total) VALUES (?, ?, 0)");
    compra.addBindValue(idProveedor);
    compra.addBindValue(fecha);

    if (!compra.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Compras", "No se pudo registrar la compra:\n" + compra.lastError().text());
        return;
    }

    QSqlQuery ultimoId(db);
    int idCompra = -1;
    if (ultimoId.exec("SELECT LAST_INSERT_ID()") && ultimoId.next()) {
        idCompra = ultimoId.value(0).toInt();
    }

    double totalCompra = 0;
    int detalles = 0;

    while (true) {
        int idProducto = -1;
        int cantidad = 1;
        double precioCompra = 0;
        if (!showCompraDetalleDialog(idProducto, cantidad, precioCompra)) {
            break;
        }

        const double subtotal = precioCompra * cantidad;

        QSqlQuery detalle(db);
        detalle.prepare("INSERT INTO detalle_compras (id_compra, id_producto, cantidad, precio_compra, subtotal) VALUES (?, ?, ?, ?, ?)");
        detalle.addBindValue(idCompra);
        detalle.addBindValue(idProducto);
        detalle.addBindValue(cantidad);
        detalle.addBindValue(precioCompra);
        detalle.addBindValue(subtotal);

        QSqlQuery stockQuery(db);
        stockQuery.prepare("UPDATE productos SET stock = stock + ? WHERE id_producto = ?");
        stockQuery.addBindValue(cantidad);
        stockQuery.addBindValue(idProducto);

        if (!detalle.exec() || !stockQuery.exec()) {
            db.rollback();
            QMessageBox::warning(this, "Compras", "No se pudo agregar el detalle de compra:\n" + detalle.lastError().text() + stockQuery.lastError().text());
            return;
        }

        totalCompra += subtotal;
        ++detalles;

        if (QMessageBox::question(this, "Compra", "Producto agregado. Queres agregar otro producto?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            break;
        }
    }

    if (detalles == 0) {
        db.rollback();
        QMessageBox::information(this, "Compras", "La compra se cancelo porque no agregaste productos.");
        return;
    }

    QSqlQuery total(db);
    total.prepare("UPDATE compras_proveedores SET total = ? WHERE id_compra = ?");
    total.addBindValue(totalCompra);
    total.addBindValue(idCompra);

    if (!total.exec() || !db.commit()) {
        db.rollback();
        QMessageBox::warning(this, "Compras", "No se pudo finalizar la compra:\n" + total.lastError().text());
        return;
    }

    QMessageBox::information(this, "Compras", "Compra registrada.\nID compra: " + QString::number(idCompra) + "\nTotal: $" + QString::number(totalCompra, 'f', 2));
    loadCompras();
    loadProductos();
}

void MainWindow::deleteCompra()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Compras", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(comprasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar compra", "Selecciona una compra de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar compra", "Seguro que queres eliminar la compra seleccionada?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM compras_proveedores WHERE id_compra = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Compras", "No se pudo eliminar la compra:\n" + query.lastError().text());
        return;
    }

    loadCompras();
}

void MainWindow::showCompraDetails()
{
    const int id = selectedId(comprasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalles de compra", "Selecciona una compra de la tabla.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Detalles de compra " + QString::number(id));
    dialog.resize(760, 420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTableWidget *table = new QTableWidget(&dialog);
    layout->addWidget(table);

    fillTable(
        table,
        {"Producto", "Cantidad", "Precio compra", "Subtotal"},
        "SELECT p.nombre, dc.cantidad, dc.precio_compra, dc.subtotal "
        "FROM detalle_compras dc "
        "LEFT JOIN productos p ON dc.id_producto = p.id_producto "
        "WHERE dc.id_compra = " + QString::number(id)
    );

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    buttons->button(QDialogButtonBox::Close)->setText("Cerrar");
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::addFactura()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }

    int idVenta = -1;
    QString numeroFactura, tipoFactura, fecha;
    double total = 0;

    if (!showFacturaDialog(idVenta, numeroFactura, tipoFactura, fecha, total)) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO facturas (id_venta, numero_factura, tipo_factura, fecha, total) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(idVenta);
    query.addBindValue(numeroFactura);
    query.addBindValue(tipoFactura);
    query.addBindValue(fecha);
    query.addBindValue(total);

    if (!query.exec()) {
        QMessageBox::warning(this, "Facturacion", "No se pudo generar la factura:\n" + query.lastError().text());
        return;
    }

    loadFacturas();
}

void MainWindow::deleteFactura()
{
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Facturacion", "No hay conexion con la base de datos.");
        return;
    }

    const int id = selectedId(facturasTable);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar factura", "Selecciona una factura de la tabla.");
        return;
    }

    if (QMessageBox::question(this, "Eliminar factura", "Seguro que queres eliminar la factura seleccionada?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM facturas WHERE id_factura = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Facturacion", "No se pudo eliminar la factura:\n" + query.lastError().text());
        return;
    }

    loadFacturas();
}

void MainWindow::addConfigItem()
{
    if (!db.isOpen() || !configTabs) {
        QMessageBox::warning(this, "Configuracion", "No hay conexion con la base de datos.");
        return;
    }

    QString nombre;
    QString descripcion;
    const int tab = configTabs->currentIndex();
    const bool hasDescription = tab == 0;

    if (!showConfigDialog(nombre, descripcion, hasDescription, "Agregar")) {
        return;
    }

    QSqlQuery query(db);
    switch (tab) {
        case 0:
            query.prepare("INSERT INTO categorias (nombre, descripcion) VALUES (?, ?)");
            query.addBindValue(nombre);
            query.addBindValue(descripcion);
            break;
        case 1:
            query.prepare("INSERT INTO marcas (nombre) VALUES (?)");
            query.addBindValue(nombre);
            break;
        case 2:
            query.prepare("INSERT INTO medios_pago (nombre) VALUES (?)");
            query.addBindValue(nombre);
            break;
        case 3:
            query.prepare("INSERT INTO estados_servicio (nombre) VALUES (?)");
            query.addBindValue(nombre);
            break;
        default:
            return;
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Configuracion", "No se pudo agregar:\n" + query.lastError().text());
        return;
    }

    refreshConfigTab();
}

void MainWindow::editConfigItem()
{
    if (!db.isOpen() || !configTabs) {
        QMessageBox::warning(this, "Configuracion", "No hay conexion con la base de datos.");
        return;
    }

    QTableWidget *table = nullptr;
    QString idColumn;
    QString tableName;
    bool hasDescription = false;

    switch (configTabs->currentIndex()) {
        case 0: table = categoriasTable; idColumn = "id_categoria"; tableName = "categorias"; hasDescription = true; break;
        case 1: table = marcasTable; idColumn = "id_marca"; tableName = "marcas"; break;
        case 2: table = mediosPagoTable; idColumn = "id_medio_pago"; tableName = "medios_pago"; break;
        case 3: table = estadosServicioTable; idColumn = "id_estado_servicio"; tableName = "estados_servicio"; break;
        default: return;
    }

    const int id = selectedId(table);
    if (id <= 0) {
        QMessageBox::information(this, "Modificar", "Selecciona un registro.");
        return;
    }

    const int row = table->currentRow();
    QString nombre = table->item(row, 1)->text();
    QString descripcion = hasDescription && table->item(row, 2) ? table->item(row, 2)->text() : "";

    if (!showConfigDialog(nombre, descripcion, hasDescription, "Modificar")) {
        return;
    }

    QSqlQuery query(db);
    if (hasDescription) {
        query.prepare("UPDATE " + tableName + " SET nombre = ?, descripcion = ? WHERE " + idColumn + " = ?");
        query.addBindValue(nombre);
        query.addBindValue(descripcion);
        query.addBindValue(id);
    } else {
        query.prepare("UPDATE " + tableName + " SET nombre = ? WHERE " + idColumn + " = ?");
        query.addBindValue(nombre);
        query.addBindValue(id);
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Configuracion", "No se pudo modificar:\n" + query.lastError().text());
        return;
    }

    refreshConfigTab();
}

void MainWindow::deleteConfigItem()
{
    if (!db.isOpen() || !configTabs) {
        QMessageBox::warning(this, "Configuracion", "No hay conexion con la base de datos.");
        return;
    }

    QTableWidget *table = nullptr;
    QString idColumn;
    QString tableName;

    switch (configTabs->currentIndex()) {
        case 0: table = categoriasTable; idColumn = "id_categoria"; tableName = "categorias"; break;
        case 1: table = marcasTable; idColumn = "id_marca"; tableName = "marcas"; break;
        case 2: table = mediosPagoTable; idColumn = "id_medio_pago"; tableName = "medios_pago"; break;
        case 3: table = estadosServicioTable; idColumn = "id_estado_servicio"; tableName = "estados_servicio"; break;
        default: return;
    }

    const int id = selectedId(table);
    if (id <= 0) {
        QMessageBox::information(this, "Eliminar", "Selecciona un registro.");
        return;
    }

    const QString nombre = table->item(table->currentRow(), 1)->text();
    int answer = QMessageBox::question(
        this,
        "Eliminar",
        "Seguro que queres eliminar \"" + nombre + "\"?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM " + tableName + " WHERE " + idColumn + " = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Configuracion", "No se pudo eliminar:\n" + query.lastError().text());
        return;
    }

    refreshConfigTab();
}

void MainWindow::showMessage(const QString &title)
{
    QMessageBox::information(
        this,
        title,
        "Esta accion ya esta funcionando en la version consola.\n"
        "La pantalla grafica inicial ya esta preparada para continuar con este formulario."
    );
}

