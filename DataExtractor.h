#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QString>
#include <QMap>

// Абстрактный интерфейс для извлечения данных типа ключ-значение
class AbstractDataExtractor
{
public:
    virtual ~AbstractDataExtractor() {}
    virtual QList<QPair<QString, QString>> extractData(const QString& filePath) = 0;
    virtual bool checkFile(const QString &filePath) = 0;
};

// Конкретная реализация DataExtractor для формата SQL
class SqlDataExtractor : public AbstractDataExtractor
{
public:
    bool checkFile(const QString& filePath)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);
        // Attempt to open the database
        if (!db.open()) {
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            db.close();
            return false;
        }
        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {

        QList<QPair<QString, QString>> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            return extractedData;
        }

        QStringList tables = database.tables();
        QString tableName = tables.first();
        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT * FROM " + tableName + " ")) {
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData.append(qMakePair(key, value));
        }

        // Закрываем соединение с базой данных
        database.close();

        return extractedData;
    }
};

// Конкретная реализация DataExtractor для формата JSON
class JsonDataExtractor : public AbstractDataExtractor
{
public:
    bool checkFile(const QString& filePath)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);

        // Attempt to open the database
        if (!db.open()) {
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            db.close();
            return false;
        }

        // Additional checks can be added here as needed

        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {
        QList<QPair<QString, QString>> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            return extractedData;
        }

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT key, value FROM my_table")) {
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData.append(qMakePair(key, value));
        }

        // Закрываем соединение с базой данных
        database.close();

        return extractedData;
    }
};

// Конкретная реализация DataExtractor для формата CSV
class CsvDataExtractor : public AbstractDataExtractor
{
public:
    bool checkFile(const QString& filePath)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);

        // Attempt to open the database
        if (!db.open()) {
            QString errorMessage = "Ошибка открытия базы данных: " + db.lastError().text();
                                   //errorLabel->setText(errorMessage);
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            QString errorMessage = "База данных пуста: " + filePath;
            //errorLabel->setText(errorMessage);
            db.close();
            return false;
        }

        // Additional checks can be added here as needed

        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {
        QList<QPair<QString, QString>> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            return extractedData;
        }

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT key, value FROM my_table")) {
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData.append(qMakePair(key, value));
        }

        // Закрываем соединение с базой данных
        database.close();

        return extractedData;
    }
};

#endif // DATAEXTRACTOR_H
