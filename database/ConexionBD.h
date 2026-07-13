#ifndef CONEXIONBD_H
#define CONEXIONBD_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#ifdef MCM_QT_APP
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#endif

class ConexionBD {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
#ifdef MCM_QT_APP
    QSqlDatabase conexionQt;
#endif

public:
    ConexionBD() {
        hEnv = NULL;
        hDbc = NULL;
    }

    bool conectar() {
        SQLRETURN ret;

        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

        SQLCHAR dsn[] = "my_chemical_music32";
        SQLCHAR user[] = "root";
        SQLCHAR pass[] = "";

        ret = SQLConnectA(
            hDbc,
            dsn, SQL_NTS,
            user, SQL_NTS,
            pass, SQL_NTS
        );

        if (SQL_SUCCEEDED(ret)) {
            return true;
        } else {
            return false;
        }
    }

    SQLHDBC getConexion() {
        return hDbc;
    }

#ifdef MCM_QT_APP
    bool conectarQt(QString *error = nullptr) {
        const QString nombre = "mcm_connection";
        if (QSqlDatabase::contains(nombre)) {
            conexionQt = QSqlDatabase::database(nombre);
        } else {
            conexionQt = QSqlDatabase::addDatabase("QODBC", nombre);
            conexionQt.setDatabaseName("my_chemical_music64");
            conexionQt.setUserName("root");
            conexionQt.setPassword("");
        }
        if (conexionQt.isOpen() || conexionQt.open()) return true;
        if (error) *error = conexionQt.lastError().text();
        return false;
    }

    QSqlDatabase getConexionQt() const { return conexionQt; }
    bool estaConectadoQt() const { return conexionQt.isValid() && conexionQt.isOpen(); }
    bool iniciarTransaccionQt() { return conexionQt.transaction(); }
    bool confirmarTransaccionQt() { return conexionQt.commit(); }
    void cancelarTransaccionQt() { conexionQt.rollback(); }
#endif

    void desconectar() {
#ifdef MCM_QT_APP
        if (conexionQt.isOpen()) conexionQt.close();
#endif
        if (hDbc != NULL) {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            hDbc = NULL;
        }

        if (hEnv != NULL) {
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            hEnv = NULL;
        }
    }
};

#endif
