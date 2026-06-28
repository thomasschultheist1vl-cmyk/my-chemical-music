#ifndef CONEXIONBD_H
#define CONEXIONBD_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class ConexionBD {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;

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

    void desconectar() {
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