#ifndef UTILIDADES_FECHA_H
#define UTILIDADES_FECHA_H

#include <QDate>
#include <QString>

inline QDate leerFecha(const QString &text)
{
    QDate fecha = QDate::fromString(text.trimmed(), "dd/MM/yyyy");
    if (!fecha.isValid()) {
        fecha = QDate::fromString(text.trimmed().left(10), Qt::ISODate);
    }
    return fecha;
}

inline QString fechaVisual(const QString &text)
{
    if (text.trimmed().isEmpty()) return {};
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString("dd/MM/yyyy") : text;
}

inline QString fechaParaBase(const QString &text)
{
    const QDate fecha = leerFecha(text);
    return fecha.isValid() ? fecha.toString(Qt::ISODate) : text;
}

#endif // UTILIDADES_FECHA_H
