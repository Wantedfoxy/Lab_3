#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QString>
#include <QMap>
#include <QFile>

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
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        // Устанавливаем путь, по которому будем подключаться к базе данных
        database.setDatabaseName(filePath);
        // Пробуем открыть
        if (!database.open()) {
            return false;
        }

        // Проверка на пустоту
        QStringList tables = database.tables();
        if (tables.isEmpty()) {
            database.close();
            return false;
        }
        // Закрытия подключения к базе данных
        database.close();
        return true;
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {

        QList<QPair<QString, QString>> extractedData;
        // Открываем соединение с базой данных
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(filePath);
        database.open();

        QStringList tables = database.tables();
        QString tableName = tables.first();

        // Выполняем SQL-запрос для извлечения данных
        QSqlQuery query;
        query.exec("SELECT * FROM " + tableName + " ");

        // Извлекаем данные из результата запроса
        while (query.next()) {
            QString key = query.value(0).toString();
            QString value = query.value(1).toString();
            // Добавляем пары ключ-значение в список extractedData
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
        QFile file(filePath);
        // Проверяем, существует ли файл и может ли он быть открыт для чтения
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            return false;
        }

        // Читаем содержимое файла в QByteArray
        QByteArray jsonData = file.readAll();
        file.close();
        // Создаем объект QJsonDocument из полученных данных
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        // Проверяем, является ли jsonDoc пустым или не объектом
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            return false;
        }
        // Получаем объект QJsonObject из jsonDoc
        QJsonObject jsonObj = jsonDoc.object();
        // Проверяем, содержит ли jsonObj ключ "data"
        if (!jsonObj.contains("data")) {
            return false;
        }
        // Получаем значение ключа "data" из jsonObj
        QJsonValue dataValue = jsonObj.value("data");
        // Проверяем, является ли dataValue массивом
        if (!dataValue.isArray()) {
            return false;
        }

        file.close();
        return true;
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {
        QList<QPair<QString, QString>> extractedData;
        // Открытие файла для чтения
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        // Чтение содержимого файла в виде JSON-данных
        QByteArray jsonData = file.readAll();
        // Закрытие файла
        file.close();
        // Создание JSON-документа из прочитанных данных
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        // Получение корневого объекта JSON
        QJsonObject jsonObj = jsonDoc.object();
        // Получение значения "data" из корневого объекта
        QJsonValue dataValue = jsonObj.value("data");
        // Преобразование значения "data" в массив JSON
        QJsonArray dataArray = dataValue.toArray();
        // Итерация по элементам массива
        for (const QJsonValue& itemValue : dataArray) {
            // Проверка, является ли элемент объектом
            if (itemValue.isObject()) {
                // Преобразование элемента в объект JSON
                QJsonObject itemObj = itemValue.toObject();
                // Проверка наличия ключа "key" и значения "value" в объекте
                if (itemObj.contains("key") && itemObj.contains("value")) {
                    // Извлечение значения ключа и значения из объекта
                    QString key = itemObj.value("key").toString();
                    QString value = itemObj.value("value").toVariant().toString();
                    // Добавляем пары ключ-значение в список extractedData
                    extractedData.append(qMakePair(key, value));
                }
            }
        }

        return extractedData;
    }
};

// Конкретная реализация DataExtractor для формата CSV
class CsvDataExtractor : public AbstractDataExtractor
{
public:
    bool checkFile(const QString& filePath)
    {
        // Создаем объект QFile для работы с файлом по указанному пути
        QFile file(filePath);
        // Открываем файл в режиме чтения и текстовом режиме
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Если не удалось открыть файл, возвращаем false
            return false;
        }
        // Создаем объект QTextStream для чтения данных из файла
        QTextStream in(&file);
        // Считываем первую строку файла, содержащую заголовки столбцов
        QString headerLine = in.readLine();
        // Разбиваем строку на отдельные заголовки с помощью разделителя ','
        QStringList headers = headerLine.split(',');
        // Находим индекс столбца "Key" в списке заголовков
        int keyIndex = headers.indexOf("Key");
        // Находим индекс столбца "Value" в списке заголовков
        int valueIndex = headers.indexOf("Value");
        // Если не удалось найти индекс одного из столбцов
        if (keyIndex == -1 || valueIndex == -1) {
            // Закрываем файл и возвращаем false
            file.close();
            return false;
        }

        return true;
    };

    QList<QPair<QString, QString>> extractData(const QString& filePath)
    {
        QList<QPair<QString, QString>> extractedData;
        // Создаем объект QFile для работы с файлом по указанному пути
        QFile file(filePath);
        // Открываем файл в режиме чтения и текстовом режиме
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        // Создаем объект QTextStream для чтения данных из файла
        QTextStream in(&file);
        // Считываем первую строку файла, содержащую заголовки столбцов
        QString headerLine = in.readLine();
        // Разбиваем строку на отдельные заголовки с помощью разделителя ','
        QStringList headers = headerLine.split(',');
        // Находим индекс столбца "Key" в списке заголовков
        int keyIndex = headers.indexOf("Key");
        // Находим индекс столбца "Value" в списке заголовков
        int valueIndex = headers.indexOf("Value");
        // Пока не достигнут конец файла
        while (!in.atEnd()) {
            // Считываем строку из файла
            QString line = in.readLine();
            // Разбиваем строку на отдельные значения с помощью разделителя ','
            QStringList values = line.split(',');
            // Проверяем, что в строке достаточно значений для столбцов "Key" и "Value"
            if (values.size() > keyIndex && values.size() > valueIndex) {
                // Получаем значение столбца "Key" и удаляем лишние пробелы в начале и конце
                QString key = values[keyIndex].trimmed();
                // Получаем значение столбца "Value" и удаляем лишние пробелы в начале и конце
                QString value = values[valueIndex].trimmed();
                // Добавляем пары ключ-значение в список extractedData
                extractedData.append(qMakePair(key, value));
            }
        }

        file.close();
        return extractedData;
    }
};

#endif // DATAEXTRACTOR_H
