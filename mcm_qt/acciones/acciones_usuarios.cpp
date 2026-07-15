#include "../ventana_principal.h"
#include "../../dao/UsuarioDAO.h"

#include <QCheckBox>
#include <QComboBox>
#include <QCryptographicHash>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QVBoxLayout>

namespace {
QString hashPassword(const QString &password)
{
    return QString::fromLatin1(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}
}

bool MainWindow::showUsuarioDialog(Usuario &usuario, QString &password, bool nuevoUsuario)
{
    QDialog dialog(this);
    dialog.setWindowTitle(nuevoUsuario ? "Agregar usuario" : "Modificar usuario");
    dialog.setMinimumWidth(430);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout;
    QLineEdit *nombreEdit = new QLineEdit(usuario.nombre, &dialog);
    QLineEdit *apellidoEdit = new QLineEdit(usuario.apellido, &dialog);
    QLineEdit *usuarioEdit = new QLineEdit(usuario.nombreUsuario, &dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    QLineEdit *confirmarEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmarEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText(nuevoUsuario ? "Contraseña" : "Dejar vacío para conservar");
    confirmarEdit->setPlaceholderText(nuevoUsuario ? "Confirmar contraseña" : "Confirmar nueva contraseña");
    QComboBox *rolCombo = new QComboBox(&dialog);
    QCheckBox *activoCheck = new QCheckBox("Activo", &dialog);
    activoCheck->setChecked(nuevoUsuario ? true : usuario.activo);

    UsuarioDAO dao(conexionBD.getConexionQt());
    QSqlQuery roles = dao.rolesQt();
    while (roles.next()) {
        rolCombo->addItem(roles.value(1).toString(), roles.value(0).toInt());
    }
    const int rolIndex = rolCombo->findData(usuario.idRol);
    if (rolIndex >= 0) rolCombo->setCurrentIndex(rolIndex);

    form->addRow("Nombre:", nombreEdit);
    form->addRow("Apellido:", apellidoEdit);
    form->addRow("Nombre de usuario:", usuarioEdit);
    form->addRow("Contraseña:", passwordEdit);
    form->addRow("Confirmar contraseña:", confirmarEdit);
    form->addRow("Rol:", rolCombo);
    form->addRow("", activoCheck);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Save)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        const QString nombre = nombreEdit->text().trimmed();
        const QString apellido = apellidoEdit->text().trimmed();
        const QString user = usuarioEdit->text().trimmed();
        const QString pass = passwordEdit->text();
        const QString confirmacion = confirmarEdit->text();

        if (nombre.isEmpty() || apellido.isEmpty() || user.isEmpty() || rolCombo->currentIndex() < 0) {
            QMessageBox::warning(&dialog, "Usuarios", "Completá todos los campos obligatorios.");
            return;
        }
        if ((nuevoUsuario || !pass.isEmpty()) && pass.length() < 4) {
            QMessageBox::warning(&dialog, "Usuarios", "La contraseña debe tener al menos 4 caracteres.");
            return;
        }
        if ((nuevoUsuario || !pass.isEmpty()) && pass != confirmacion) {
            QMessageBox::warning(&dialog, "Usuarios", "La contraseña y la confirmación no coinciden.");
            return;
        }
        if (dao.existeNombreUsuarioQt(user, nuevoUsuario ? 0 : usuario.idUsuario)) {
            QMessageBox::warning(&dialog, "Usuarios", "Ya existe un usuario con ese nombre de usuario.");
            return;
        }

        usuario.nombre = nombre;
        usuario.apellido = apellido;
        usuario.nombreUsuario = user;
        usuario.idRol = rolCombo->currentData().toInt();
        usuario.nombreRol = rolCombo->currentText();
        usuario.activo = activoCheck->isChecked();
        password = pass;
        dialog.accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

void MainWindow::addUsuario()
{
    if (!usuarioActual.esSupervisor()) return;
    Usuario usuario;
    QString password;
    if (!showUsuarioDialog(usuario, password, true)) return;

    UsuarioDAO dao(conexionBD.getConexionQt());
    if (!dao.agregarUsuarioQt(usuario, hashPassword(password))) {
        QMessageBox::warning(this, "Usuarios", "No se pudo agregar el usuario.");
        return;
    }
    loadUsuarios();
}

void MainWindow::editUsuario()
{
    if (!usuarioActual.esSupervisor()) return;
    const int id = selectedId(usuariosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Usuarios", "Selecciona un usuario de la tabla.");
        return;
    }

    UsuarioDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.obtenerUsuarioQt(id);
    if (!q.next()) {
        QMessageBox::warning(this, "Usuarios", "No se pudo cargar el usuario.");
        return;
    }
    Usuario usuario;
    usuario.idUsuario = q.value(0).toInt();
    usuario.idRol = q.value(1).toInt();
    usuario.nombreRol = q.value(2).toString();
    usuario.nombre = q.value(3).toString();
    usuario.apellido = q.value(4).toString();
    usuario.nombreUsuario = q.value(5).toString();
    usuario.activo = q.value(6).toBool();

    QString password;
    if (!showUsuarioDialog(usuario, password, false)) return;

    if (id == usuarioActual.idUsuario && !usuario.activo) {
        QMessageBox::warning(this, "Usuarios", "No podés desactivar tu propia cuenta desde modificación.");
        return;
    }
    if (q.value(2).toString() == "Supervisor" && (!usuario.activo || usuario.nombreRol != "Supervisor")
        && dao.contarSupervisoresActivosQt(id) == 0) {
        QMessageBox::warning(this, "Usuarios", "No podés dejar el sistema sin supervisores activos.");
        return;
    }
    if (!dao.modificarUsuarioQt(usuario)) {
        QMessageBox::warning(this, "Usuarios", "No se pudo modificar el usuario.");
        return;
    }
    if (!password.isEmpty() && !dao.cambiarPasswordQt(id, hashPassword(password))) {
        QMessageBox::warning(this, "Usuarios", "El usuario se modificó, pero no se pudo cambiar la contraseña.");
    }
    loadUsuarios();
}

void MainWindow::activarUsuario()
{
    if (!usuarioActual.esSupervisor()) return;
    const int id = selectedId(usuariosTable);
    if (id <= 0) return;
    UsuarioDAO dao(conexionBD.getConexionQt());
    if (!dao.activarUsuarioQt(id, true)) {
        QMessageBox::warning(this, "Usuarios", "No se pudo activar el usuario.");
    }
    loadUsuarios();
}

void MainWindow::desactivarUsuario()
{
    if (!usuarioActual.esSupervisor()) return;
    const int id = selectedId(usuariosTable);
    if (id <= 0) return;
    if (id == usuarioActual.idUsuario) {
        QMessageBox::warning(this, "Usuarios", "No podés desactivar tu propia cuenta.");
        return;
    }
    UsuarioDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.obtenerUsuarioQt(id);
    if (q.next() && q.value(2).toString() == "Supervisor" && dao.contarSupervisoresActivosQt(id) == 0) {
        QMessageBox::warning(this, "Usuarios", "No podés dejar el sistema sin supervisores activos.");
        return;
    }
    if (!dao.activarUsuarioQt(id, false)) {
        QMessageBox::warning(this, "Usuarios", "No se pudo desactivar el usuario.");
    }
    loadUsuarios();
}

void MainWindow::showUsuarioDetails()
{
    if (!usuarioActual.esSupervisor()) return;
    const int id = selectedId(usuariosTable);
    if (id <= 0) {
        QMessageBox::information(this, "Detalle de usuario", "Selecciona un usuario de la tabla.");
        return;
    }
    UsuarioDAO dao(conexionBD.getConexionQt());
    QSqlQuery q = dao.detalleUsuarioQt(id);
    if (!q.next()) return;
    showDetalleSimple("Detalle de usuario", {
        {"ID", q.value(0).toString()},
        {"Nombre", q.value(1).toString()},
        {"Apellido", q.value(2).toString()},
        {"Nombre de usuario", q.value(3).toString()},
        {"Rol", q.value(4).toString()},
        {"Estado", q.value(5).toString()},
        {"Fecha de creacion", q.value(6).toString()}
    });
}
