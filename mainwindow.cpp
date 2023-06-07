#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>
#include <QPieSlice>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    openFolderButton = std::make_unique<QPushButton>("Открыть папку", this);
    openFolderButton->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    chartTypeLabel = std::make_unique<QLabel>("Тип диаграммы:", this);
    chartTypeLabel->setAlignment(Qt::AlignHCenter);;
    chartTypeLabel->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    chartTypeComboBox = std::make_unique<QComboBox>(this);
    chartTypeComboBox->addItem("Столбчатая диаграмма");
    chartTypeComboBox->addItem("Круговая диаграмма");
    chartTypeComboBox->addItem("Линейная диаграмма");
    chartTypeComboBox->addItem("Точечная диаграмма");
    chartTypeComboBox->addItem("Гистограмма ");
    chartTypeComboBox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    colorfulCheckbox = std::make_unique<QCheckBox>("Цветная", this);
    colorfulCheckbox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    exportButton = std::make_unique<QPushButton>("Экспорт", this);
    exportButton->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    fileTreeView = std::make_unique<QTreeView>(this);
    ChartView = std::make_unique<QChartView>(this);
    ChartView->setMinimumWidth(100);
    ChartView->resize(650,0);
    fileSystemModel = std::make_unique<QFileSystemModel>(this);
    fileSystemModel->setRootPath(QDir::homePath());
    fileSystemModel = std::make_unique<QFileSystemModel>(this);
    fileTreeView->setMinimumWidth(100);
    fileTreeView->resize(424,0);
    fileTreeView->setColumnWidth(0, 142);
    fileTreeView->setColumnWidth(1, 141);
    fileTreeView->setColumnWidth(2, 141);
    fileTreeView->setColumnHidden(3, true);

    splitter = std::make_unique<QSplitter>(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(fileTreeView.get());
    splitter->addWidget(ChartView.get());
    splitter->setHandleWidth(1);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(openFolderButton.get());
    topLayout->addWidget(chartTypeLabel.get());
    topLayout->addWidget(chartTypeComboBox.get());
    topLayout->addWidget(colorfulCheckbox.get());
    topLayout->addWidget(exportButton.get());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(splitter.get());

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setMinimumSize(800, 600); // Задаем минимальный размер окна
    resize(1024, 768); // Задаем стандартный размер окна

   connect(openFolderButton.get(), &QPushButton::clicked, this, &MainWindow::openFolder);
}

MainWindow::~MainWindow()
{}

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath());
    if (!folderPath.isEmpty()) {
        // Установка фильтра на файлы и установка выбранной папки как корневого пути
        fileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
        fileSystemModel->setRootPath(folderPath);

        // Установка модели данных для QListView
        fileTreeView->setModel(fileSystemModel.get());
        fileTreeView->setRootIndex(fileSystemModel->index(folderPath));
    }
}

void MainWindow::changeChartType(const QString& type)
{
    // Определение типа диаграммы на основе выбранного значения в combobox
    if (type == "Столбчатая диаграмма") {
        // Создание экземпляра класса для столбчатой диаграммы
        //BarChart* barChart = new BarChart(ChartView);

        // Передача экземпляра DrawChart в ChartView для отображения
        //ChartView->setDrawChart(barChart);
    } else if (type == "Круговая диаграмма") {
        // Создание экземпляра класса для круговой диаграммы
       // PieChart* pieChart = new PieChart(ChartView);

        // Передача экземпляра DrawChart в ChartView для отображения
       // ChartView->setDrawChart(pieChart);
    } else if (type == "Линейная диаграмма") {
        // Выбран тип "Линейная диаграмма"
        // Отрисовка линейной диаграммы
        //drawLineChart();
    } else if (type == "Точечная диаграмма") {
        // Выбран тип "Точечная диаграмма"
        // Отрисовка точечной диаграммы
        //drawScatterChart();
    } else if (type == "Гистограмма") {
        // Выбран тип "Гистограмма"
        // Отрисовка гистограммы
        //drawHistogram();
    }
}
