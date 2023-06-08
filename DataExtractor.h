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
    virtual QMap<QString, QString> extractData(const QString& filePath) = 0;
    virtual bool checkFile(const QString &filePath, QLabel* errorLabe) = 0;

};

// Конкретная реализация DataExtractor для формата SQL
class SqlDataExtractor : public AbstractDataExtractor
{
public:
    bool checkFile(const QString& filePath, QLabel* errorLabel)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);

        // Attempt to open the database
        if (!db.open()) {
            QString errorMessage = "Ошибка открытия базы данных: " + db.lastError().text();
            errorLabel->setText(errorMessage);
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            QString errorMessage = "База данных пуста: " + filePath;
            errorLabel->setText(errorMessage);
            db.close();
            return false;
        }

        // Additional checks can be added here as needed

        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QMap<QString, QString> extractData(const QString& filePath)
    {

        QMap<QString, QString> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            qDebug() << "Failed to open database:" << database.lastError().text();
            return extractedData;
        }

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT key, value FROM my_table")) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData[key] = value;
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
    bool checkFile(const QString& filePath, QLabel* errorLabel)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);

        // Attempt to open the database
        if (!db.open()) {
            QString errorMessage = "Ошибка открытия базы данных: " + db.lastError().text();
                                   errorLabel->setText(errorMessage);
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            QString errorMessage = "База данных пуста: " + filePath;
            errorLabel->setText(errorMessage);
            db.close();
            return false;
        }

        // Additional checks can be added here as needed

        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QMap<QString, QString> extractData(const QString& filePath)
    {
        QMap<QString, QString> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            qDebug() << "Failed to open database:" << database.lastError().text();
            return extractedData;
        }

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT key, value FROM my_table")) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData[key] = value;
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
    bool checkFile(const QString& filePath, QLabel* errorLabel)
    {
        // Create a QSqlDatabase object
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        // Set the database name
        db.setDatabaseName(filePath);

        // Attempt to open the database
        if (!db.open()) {
            QString errorMessage = "Ошибка открытия базы данных: " + db.lastError().text();
                                   errorLabel->setText(errorMessage);
            return false;
        }

        // Check if the database is empty
        QStringList tables = db.tables();
        if (tables.isEmpty()) {
            QString errorMessage = "База данных пуста: " + filePath;
            errorLabel->setText(errorMessage);
            db.close();
            return false;
        }

        // Additional checks can be added here as needed

        // Close the database connection
        db.close();

        return true; // File is valid
    };

    QMap<QString, QString> extractData(const QString& filePath)
    {
        QMap<QString, QString> extractedData;

        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        if (!database.open()) {
            qDebug() << "Failed to open database:" << database.lastError().text();
            return extractedData;
        }

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        if (!query.exec("SELECT key, value FROM my_table")) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            database.close();
            return extractedData;
        }

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            extractedData[key] = value;
        }

        // Закрываем соединение с базой данных
        database.close();

        return extractedData;
    }
};

#endif // DATAEXTRACTOR_H
