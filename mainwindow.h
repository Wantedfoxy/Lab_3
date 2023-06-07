#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include <QSplitter>
#include <QFileSystemModel>
#include <QChartView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *openFolderButton; // Кнопка для открытия окна выбора папки
    QLabel *chartTypeLabel; // Надпись "Тип диаграммы:"
    QComboBox *chartTypeComboBox; // Выпадающий список для выбора типа диаграммы
    QCheckBox *colorfulCheckbox; // Флажок для выбора цветной диаграммы
    QPushButton *exportButton; // Кнопка для экспорта диаграммы
    QListView *fileListView; // Список для отображения файлов
    QChartView *chartView; // Виджет для отображения диаграммы
    QFileSystemModel *fileSystemModel; // Модель файловой системы
    QSplitter *splitter; // Разделитель между списком файлов и диаграммой
};

#endif // MAINWINDOW_H
