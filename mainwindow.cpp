#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем кнопку "Открыть папку"
    openFolderButton = std::make_unique<QPushButton>("Открыть папку", this);
    openFolderButton->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем метку "Тип диаграммы"
    chartTypeLabel = std::make_unique<QLabel>("Тип диаграммы:", this);
    chartTypeLabel->setAlignment(Qt::AlignHCenter);;
    chartTypeLabel->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем выпадающий список с типами диаграмм
    chartTypeComboBox = std::make_unique<QComboBox>(this);
    chartTypeComboBox->addItem("Столбчатая диаграмма");
    chartTypeComboBox->addItem("Круговая диаграмма");
    chartTypeComboBox->addItem("Линейная диаграмма");
    chartTypeComboBox->addItem("Точечная диаграмма");
    chartTypeComboBox->addItem("Гистограмма ");
    chartTypeComboBox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем флажок для выбора черно-белой диаграммы
    colorfulCheckbox = std::make_unique<QCheckBox>("Черно-белая диаграмма", this);
    colorfulCheckbox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем кнопку "Экспорт"
    exportButton = std::make_unique<QPushButton>("Экспорт", this);
    exportButton->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем представление файлов в виде дерева
    fileTreeView = std::make_unique<QTreeView>(this);
    fileTreeView->setMinimumWidth(100);
    fileTreeView->resize(424,0);
    fileTreeView->setColumnWidth(0, 142);
    fileTreeView->setColumnWidth(1, 141);
    fileTreeView->setColumnWidth(2, 141);
    fileTreeView->setColumnHidden(3, true);

    // Создаем виджет для отображения диаграммы
    ChartView = std::make_unique<QChartView>(this);
    ChartView->setMinimumWidth(100);
    ChartView->resize(650,0);

    // Создаем QLabel для отображения ошибок
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

    // Создаем модель файловой системы для QTreeView
    fileSystemModel = std::make_unique<QFileSystemModel>(this);
    fileSystemModel->setRootPath(QDir::homePath());
    fileSystemModel = std::make_unique<QFileSystemModel>(this);

    // Создаем разделитель между QTreeView и QChartView
    splitter = std::make_unique<QSplitter>(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(fileTreeView.get());
    splitter->addWidget(ChartView.get());
    splitter->setHandleWidth(1);

    // Создаем QHBoxLayout и добавляем элементы управления
    std::unique_ptr<QHBoxLayout> topLayout = std::make_unique<QHBoxLayout>();
    topLayout->addWidget(openFolderButton.get());
    topLayout->addWidget(chartTypeLabel.get());
    topLayout->addWidget(chartTypeComboBox.get());
    topLayout->addWidget(colorfulCheckbox.get());
    topLayout->addWidget(exportButton.get());

    // Создаем QVBoxLayout и добавляем QHBoxLayout и splitter
    std::unique_ptr<QVBoxLayout> mainLayout = std::make_unique<QVBoxLayout>();
    mainLayout->addLayout(topLayout.release());
    mainLayout->addWidget(splitter.release());

    // Создаем QWidget в качестве центрального виджета и устанавливаем QVBoxLayout в него
    std::unique_ptr<QWidget> centralWidget = std::make_unique<QWidget>(this);
    centralWidget->setLayout(mainLayout.release());
    setCentralWidget(centralWidget.release());

    // Задаем минимальный размер окна
    setMinimumSize(800, 600);
    // Задаем стандартный размер окна
    resize(1024, 768);

   // Устанавливаем соединение между сигналом clicked кнопки openFolderButton и слотом openFolder()
   connect(openFolderButton.get(), &QPushButton::clicked, this, &MainWindow::openFolder);
}

MainWindow::~MainWindow()
{}

void MainWindow::openFolder()
{
   // Открываем диалоговое окно выбора папки
   QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath());
    // Создание объекта QDir с указанным путем folderPath, который представляет директорию,
   // указанную в folderPath. Этот объект будет использоваться для выполнения операций с файлами
   // внутри указанной директории.
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

            // Установка модели данных для QTreeView и устанавливаем корневой индекс на выбранную
            // папку
            fileTreeView->setModel(fileSystemModel.get());
            fileTreeView->setRootIndex(fileSystemModel->index(folderPath));
        } else {
            // Если указанная папка пуста, отображаем предупреждение
            errorLabel->setText("Указанная папка пуста");
            errorLabel->setVisible(true);
        }
    } else {
        // Если указанная папка не существует, отображаем предупреждение
        errorLabel->setText("Указанная папка не существует");
        errorLabel->setVisible(true);
    }
}

void MainWindow::changeChartType(const QString& type)
{
    // Определение типа диаграммы на основе выбранного значения в combobox
    if (type == "Столбчатая диаграмма") {
        // Обработка для столбчатой диаграммы
    } else if (type == "Круговая диаграмма") {
        // Обработка для круговой диаграммы
    } else if (type == "Линейная диаграмма") {
        // Обработка для линейной диаграммы
    } else if (type == "Точечная диаграмма") {
        // Обработка для точечной диаграммы
    } else if (type == "Гистограмма") {
        // Обработка для гистограммы
    }
}
