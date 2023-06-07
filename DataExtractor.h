#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

#include <QString>
#include <QMap>

// Абстрактный интерфейс для извлечения данных типа ключ-значение
class AbstractDataExtractor
{
public:
    virtual ~AbstractDataExtractor() {}
    virtual QMap<QString, QString> extractData(const QString& filePath) = 0;
};

// Конкретная реализация DataExtractor для формата SQL
class SqlDataExtractor : public AbstractDataExtractor
{
public:
    QMap<QString, QString> extractData(const QString& filePath)
    {
        // Логика для извлечения данных типа ключ-значение из SQL-файла
        // Возвращаем извлеченные данные в виде QMap<QString, QString>
    }
};

// Конкретная реализация DataExtractor для формата JSON
class JsonDataExtractor : public AbstractDataExtractor
{
public:
    QMap<QString, QString> extractData(const QString& filePath)
    {
        // Логика для извлечения данных типа ключ-значение из SQL-файла
        // Возвращаем извлеченные данные в виде QMap<QString, QString>
    }
};

// Конкретная реализация DataExtractor для формата CSV
class CsvDataExtractor : public AbstractDataExtractor
{
public:
    QMap<QString, QString> extractData(const QString& filePath)
    {
        // Логика для извлечения данных типа ключ-значение из SQL-файла
        // Возвращаем извлеченные данные в виде QMap<QString, QString>
    }
};

#endif // DATAEXTRACTOR_H
