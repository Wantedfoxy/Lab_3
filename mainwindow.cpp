#include "MainWindow.h"

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

    std::unique_ptr<QLabel> errorLabel = std::make_unique<QLabel>(this);
    errorLabel->setText("Warning");
    errorLabel->setAlignment(Qt::AlignCenter);
    // Устанавливаем ширину QLabel равной ширине ChartView
    errorLabel->setFixedWidth(ChartView->width());
    // Создаем QVBoxLayout и добавляем QLabel в него
    std::unique_ptr<QVBoxLayout> errorLayout = std::make_unique<QVBoxLayout>();
    errorLayout->addWidget(errorLabel.get());
    errorLayout->setAlignment(Qt::AlignCenter);
    // Устанавливаем QVBoxLayout в качестве layout для QChartView
    ChartView->setLayout(errorLayout.get());
    errorLabel->setVisible(false);

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

    std::unique_ptr<QHBoxLayout> topLayout = std::make_unique<QHBoxLayout>();
    topLayout->addWidget(openFolderButton.get());
    topLayout->addWidget(chartTypeLabel.get());
    topLayout->addWidget(chartTypeComboBox.get());
    topLayout->addWidget(colorfulCheckbox.get());
    topLayout->addWidget(exportButton.get());

    std::unique_ptr<QVBoxLayout> mainLayout = std::make_unique<QVBoxLayout>();
    mainLayout->addLayout(topLayout.release());
    mainLayout->addWidget(splitter.release());

    std::unique_ptr<QWidget> centralWidget = std::make_unique<QWidget>(this);
    centralWidget->setLayout(mainLayout.release());
    setCentralWidget(centralWidget.release());

    setMinimumSize(800, 600); // Задаем минимальный размер окна
    resize(1024, 768); // Задаем стандартный размер окна

   connect(openFolderButton.get(), &QPushButton::clicked, this, &MainWindow::openFolder);
}

MainWindow::~MainWindow()
{}

void MainWindow::openFolder()
{
   QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath());
   QDir folderDir(folderPath);
   if (folderDir.exists()) {
        if (!folderDir.isEmpty()) {
           // Установка фильтра на файлы и установка выбранной папки как корневого пути
            fileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
            fileSystemModel->setRootPath(folderPath);

            // Установка списка допустимых расширений файлов
            QStringList nameFilters;
            nameFilters << "*.sqlite" << "*.json" << "*.csv";
            fileSystemModel->setNameFilters(nameFilters);
            fileSystemModel->setNameFilterDisables(false);

            // Установка модели данных для QTreeView
            fileTreeView->setModel(fileSystemModel.get());
            fileTreeView->setRootIndex(fileSystemModel->index(folderPath));
        } else {
            errorLabel->setText("Указанная папка пуста");
            errorLabel->setVisible(true);
        }
    } else {
        errorLabel->setText("Указанная папка не существует");
        errorLabel->setVisible(true);
    }
}

void MainWindow::changeChartType(const QString& type)
{
    // Определение типа диаграммы на основе выбранного значения в combobox
    if (type == "Столбчатая диаграмма") {

    } else if (type == "Круговая диаграмма") {

    } else if (type == "Линейная диаграмма") {

    } else if (type == "Точечная диаграмма") {

    } else if (type == "Гистограмма") {

    }
}
