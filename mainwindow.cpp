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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    openFolderButton = new QPushButton("Открыть папку", this);
    chartTypeLabel = new QLabel("Тип диаграммы:", this);
    chartTypeComboBox = new QComboBox(this);
    chartTypeComboBox->addItem("Столбчатая диаграмма");
    chartTypeComboBox->addItem("Круговая диаграмма");
    colorfulCheckbox = new QCheckBox("Цветная", this);
    exportButton = new QPushButton("Экспорт", this);
    fileListView = new QListView(this);
    chartView = new QChartView(this);
    fileSystemModel = new QFileSystemModel(this);
    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setStretchFactor(0, 35);
    splitter->setStretchFactor(1, 65);
    splitter->setHandleWidth(1); // Задаем ширину разделителя


    fileSystemModel->setRootPath(QDir::homePath());
    fileListView->setModel(fileSystemModel);

    splitter->addWidget(fileListView);
    splitter->addWidget(chartView);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(openFolderButton);
    topLayout->addWidget(chartTypeLabel);
    topLayout->addWidget(chartTypeComboBox);
    topLayout->addWidget(colorfulCheckbox);
    topLayout->addWidget(exportButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(splitter);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setMinimumSize(800, 600); // Задаем минимальный размер окна
    resize(1024, 768); // Задаем стандартный размер окна
}

MainWindow::~MainWindow()
{
    delete openFolderButton;
    delete chartTypeLabel;
    delete chartTypeComboBox;
    delete colorfulCheckbox;
    delete exportButton;
    delete fileListView;
    delete chartView;
    delete fileSystemModel;
    delete splitter;
}
