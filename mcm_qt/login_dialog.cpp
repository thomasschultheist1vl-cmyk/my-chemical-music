#include "login_dialog.h"
#include "../dao/UsuarioDAO.h"

#include <QCheckBox>
#include <QCryptographicHash>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , usuarioEdit(new QLineEdit(this))
    , passwordEdit(new QLineEdit(this))
    , errorLabel(new QLabel(this))
{
    setWindowTitle("Iniciar sesión - My Chemical Music");
    setMinimumWidth(420);
    setModal(true);

    setStyleSheet(R"(
        QDialog { background: #111111; }
        QLabel { color: #f8fafc; }
        QLabel#loginTitle { color: #f87171; font-size: 20px; font-weight: 800; }
        QLabel#errorLabel { color: #f87171; font-weight: 600; }
        QLineEdit {
            background: #18181b;
            color: #f8fafc;
            border: 1px solid #3f3f46;
            border-radius: 6px;
            padding: 9px 12px;
        }
        QLineEdit:focus { border: 1px solid #dc2626; }
        QPushButton {
            background: #dc2626;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 14px;
            font-weight: 700;
        }
        QPushButton:hover { background: #b91c1c; }
        QCheckBox { color: #d1d5db; }
    )");

    QLabel *titulo = new QLabel("MY CHEMICAL MUSIC", this);
    titulo->setObjectName("loginTitle");
    QLabel *subtitulo = new QLabel("Ingresá con tu usuario para continuar", this);
    subtitulo->setStyleSheet("color: #d1d5db;");

    usuarioEdit->setPlaceholderText("Nombre de usuario");
    passwordEdit->setPlaceholderText("Contraseña");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QCheckBox *mostrarPassword = new QCheckBox("Mostrar contraseña", this);
    connect(mostrarPassword, &QCheckBox::toggled, this, [this](bool visible) {
        passwordEdit->setEchoMode(visible ? QLineEdit::Normal : QLineEdit::Password);
    });

    errorLabel->setObjectName("errorLabel");
    errorLabel->setWordWrap(true);

    QFormLayout *form = new QFormLayout;
    form->addRow("Usuario:", usuarioEdit);
    form->addRow("Contraseña:", passwordEdit);

    QPushButton *ingresar = new QPushButton("Ingresar", this);
    connect(ingresar, &QPushButton::clicked, this, &LoginDialog::intentarLogin);
    connect(usuarioEdit, &QLineEdit::returnPressed, this, &LoginDialog::intentarLogin);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::intentarLogin);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(12);
    layout->addWidget(titulo);
    layout->addWidget(subtitulo);
    layout->addSpacing(8);
    layout->addLayout(form);
    layout->addWidget(mostrarPassword);
    layout->addWidget(errorLabel);
    layout->addWidget(ingresar);

    QString errorConexion;
    if (!conexionBD.conectarQt(&errorConexion)) {
        errorLabel->setText("No se pudo conectar a la base de datos:\n" + errorConexion);
        ingresar->setEnabled(false);
    }
}

Usuario LoginDialog::usuarioAutenticado() const
{
    return usuarioActual;
}

QString LoginDialog::sha256Hex(const QString &texto)
{
    return QString::fromLatin1(
        QCryptographicHash::hash(texto.toUtf8(), QCryptographicHash::Sha256).toHex()
    );
}

void LoginDialog::intentarLogin()
{
    const QString nombreUsuario = usuarioEdit->text().trimmed();
    const QString password = passwordEdit->text();

    if (nombreUsuario.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Usuario y contraseña son obligatorios.");
        return;
    }

    UsuarioDAO dao(conexionBD.getConexionQt());
    QString error;
    Usuario usuario;
    if (!dao.validarLoginQt(nombreUsuario, sha256Hex(password), usuario, &error)) {
        errorLabel->setText(error);
        passwordEdit->clear();
        passwordEdit->setFocus();
        return;
    }

    usuarioActual = usuario;
    accept();
}
