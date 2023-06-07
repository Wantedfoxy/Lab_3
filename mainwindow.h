#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QTreeView>
#include <QSplitter>
#include <QFileSystemModel>
#include <QChartView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>
#include <QPieSlice>
#include <QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFolder();                        // Слот для открытия папки
    void changeChartType(const QString&);      // Слот для изменения типа диаграммы

private:
    std::unique_ptr<QPushButton> openFolderButton;       // Кнопка "Открыть папку"
    std::unique_ptr<QLabel> chartTypeLabel;              // Метка "Тип диаграммы"
    std::unique_ptr<QLabel> errorLabel;                  // Метка для отображения ошибок
    std::unique_ptr<QComboBox> chartTypeComboBox;        // Выпадающий список с типами диаграмм
    std::unique_ptr<QCheckBox> colorfulCheckbox;         // Флажок для выбора цветной диаграммы
    std::unique_ptr<QPushButton> exportButton;           // Кнопка "Экспорт"
    std::unique_ptr<QTreeView> fileTreeView;             // Представление файлов в виде дерева
    std::unique_ptr<QChartView> ChartView;               // Виджет для отображения диаграммы
    std::unique_ptr<QFileSystemModel> fileSystemModel;   // Модель файловой системы для QTreeView
    std::unique_ptr<QSplitter> splitter;                 // Разделитель между QTreeView и QChartView
};

#endif // MAINWINDOW_H
