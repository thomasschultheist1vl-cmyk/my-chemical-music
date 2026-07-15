#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include "../database/ConexionBD.h"
#include "../models/Usuario.h"

#include <QDialog>

class QLineEdit;
class QLabel;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    Usuario usuarioAutenticado() const;

private:
    ConexionBD conexionBD;
    QLineEdit *usuarioEdit;
    QLineEdit *passwordEdit;
    QLabel *errorLabel;
    Usuario usuarioActual;

    void intentarLogin();
    static QString sha256Hex(const QString &texto);
};

#endif // LOGIN_DIALOG_H
