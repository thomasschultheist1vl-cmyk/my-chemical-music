#include "ventana_principal.h"
#include "login_dialog.h"

#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    while (true) {
        LoginDialog login;
        if (login.exec() != QDialog::Accepted) {
            return 0;
        }

        MainWindow w(login.usuarioAutenticado());
        w.show();

        const int resultado = QApplication::exec();
        if (resultado != MainWindow::CodigoCerrarSesion) {
            return resultado;
        }
    }
}
