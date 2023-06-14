#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IOCContainer.h"
#include "DataExtractor.h"
#include "ChartDrawer.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include <QItemSelectionModel>
#include <QSplitter>
#include <QFileSystemModel>
#include <QChartView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QList>
#include <QGraphicsColorizeEffect>
#include <QPdfWriter>
#include <QPainter>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void errorMessageReceived(QString text);

public slots:
    // Слот для открытия папки
    void openFolder();
    // Слот для отслеживания изменения выбранного файла
    void handleFileSelectionChanged(const QItemSelection&);
    // Слот для изменения типа диаграммы
    void changeChartType(const QString&);
    // Слот для вывода сообщения с ошибкой
    void printErrorLabel(QString);
    // Слот для обновления цветовой гаммы (цветная или ч/б диаграмма)
    void updateChartColorMode(bool);
    // Слот для эксорта данных в pdf
    void exportChart();

private:
    std::unique_ptr<QPushButton> openFolderButton;       // Кнопка "Открыть папку"
    std::unique_ptr<QLabel> chartTypeLabel;              // Метка "Тип диаграммы"
    std::unique_ptr<QLabel> errorLabel;                  // Метка для отображения ошибок
    std::unique_ptr<QChartView> chartView;
    std::unique_ptr<QComboBox> chartTypeComboBox;        // Выпадающий список с типами диаграмм
    std::unique_ptr<QCheckBox> BWCheckbox;               // Флажок для выбора цветной диаграммы
    std::unique_ptr<QPushButton> exportButton;           // Кнопка "Экспорт"
    std::unique_ptr<QListView> fileListView;             // Представление файлов в виде дерева
    std::unique_ptr<QWidget> chartViewWidget;            // Виджет для отображения диаграммы
    std::shared_ptr<QFileSystemModel> fileSystemModel;   // Модель файловой системы для QListView
    std::unique_ptr<QVBoxLayout> layout;                 // Обертка для QLabel и QChartView
    std::unique_ptr<QSplitter> splitter;                // Разделитель между QListView и QChartView
    std::unique_ptr<AbstractDataExtractor> dataExtractor;// Объект, с помощью которого реализуется
                                                         // паттерн стратегия
    std::shared_ptr<ChartRenderer> chartRenderer;        // Используется для отрисовки диаграмм
    QList<QPair<QString, QString>> extractedData;        // Данные, которые мы получаем из файлов
    QString selectedFilePath;                            // Путь к выбранному файлу
    QItemSelectionModel* ListSelectionModel;             // Модель работы с выбранным файлом
    bool isChartRendered;                        // Флаг, указывающий, была ли отрисована диаграмма
    IOCContainer container;                              // Контейнер для выбора типа диаграммы
};

#endif // MAINWINDOW_H
