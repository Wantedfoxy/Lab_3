#include "MainWindow.h"

int IOCContainer::s_nextTypeId = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Инициализируем строковую переменную NULL, эта переменная следит, есть ли выбранный файл;
    selectedFilePath = NULL;
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
    chartTypeComboBox->addItem("Горизонтальная столбчатая диаграмма");
    chartTypeComboBox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем флажок для выбора черно-белой диаграммы
    BWCheckbox = std::make_unique<QCheckBox>("Черно-белая диаграмма", this);
    BWCheckbox->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем кнопку "Экспорт"
    exportButton = std::make_unique<QPushButton>("Экспорт", this);
    exportButton->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем представление файлов в виде списка (QListView)
    fileListView = std::make_unique<QListView>(this);
    fileListView->setMinimumWidth(100);
    fileListView->resize(350,0);
    fileListView->setStyleSheet("border: 1px solid black; border-radius: 5px; padding: 5px;");

    // Создаем виджет для отображения диаграммы
    chartViewWidget = std::make_unique<QWidget>(this);
    chartViewWidget->setMinimumWidth(100);
    chartViewWidget->resize(674, 0);

    // Создаем модель файловой системы для QListView
    fileSystemModel = std::make_shared<QFileSystemModel>(this);

    // Создаем разделитель между QListView и QChartViewWidget
    splitter = std::make_unique<QSplitter>(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(fileListView.get());
    splitter->addWidget(chartViewWidget.get());
    splitter->setHandleWidth(1);

    // Создаем QHBoxLayout и добавляем элементы управления
    std::unique_ptr<QHBoxLayout> topLayout = std::make_unique<QHBoxLayout>();
    topLayout->addWidget(openFolderButton.get());
    topLayout->addWidget(chartTypeLabel.get());
    topLayout->addWidget(chartTypeComboBox.get());
    topLayout->addWidget(BWCheckbox.get());
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

   connect(this, SIGNAL(errorMessageReceived(QString)), this, SLOT(printErrorLabel(QString)));

   connect(chartTypeComboBox.get(), SIGNAL(currentTextChanged(const QString&)), this, SLOT(changeChartType(const QString&)));
   connect(BWCheckbox.get(), &QCheckBox::stateChanged, this, &MainWindow::updateChartColorMode);
   connect(exportButton.get(), &QPushButton::clicked, this, &MainWindow::exportChart);
}

MainWindow::~MainWindow()
{}

void MainWindow::openFolder()
{
   selectedFilePath = NULL;
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

            // Установка модели данных для QListView и устанавливаем корневой индекс на выбранную
            // папку
            fileListView->setModel(fileSystemModel.get());
            fileListView->setRootIndex(fileSystemModel->index(folderPath));

            // Создаем QItemSelectionModel и соединяем его сигнал со слотом handleFileSelectionChanged
            ListSelectionModel = fileListView->selectionModel();
            connect(ListSelectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::handleFileSelectionChanged);
        } else {
            // Если указанная папка пуста, отображаем предупреждение
            emit errorMessageReceived("Указанная папка пуста");
        }
    } else {
        emit errorMessageReceived("Указанная папка не существует");
    }
}

void MainWindow::handleFileSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    fileListView->setModel(fileSystemModel.get());
    QModelIndex index = fileListView->selectionModel()->currentIndex();
    // Получение пути к выбранному файлу
    selectedFilePath = fileSystemModel->filePath(index);

    // Определение типа файла на основе расширения
    QString fileExtension = QFileInfo(selectedFilePath).suffix();

    // Проверяем, есть ли выбранные элементы в QTreeView
    if (!selected.isEmpty())
    {
        // Получаем индекс выбранного элемента
        QModelIndex selectedIndex = selected.indexes().first();

        // Получаем путь к выбранному файлу
        selectedFilePath = fileSystemModel->filePath(selectedIndex);

        // Проверяем тип файла и выполняем соответствующую обработку
        QFileInfo fileInfo(selectedFilePath);
        QString fileExtension = fileInfo.suffix();

    // Создание соответствующего экземпляра DataExtractor в зависимости от типа файла
    if (fileExtension == "sqlite")
    {
        dataExtractor = std::make_unique<SqlDataExtractor>();
            emit errorMessageReceived("Выбран SQL-файл: " + selectedFilePath);
    }
    else if (fileExtension == "json")
    {
        dataExtractor = std::make_unique<JsonDataExtractor>();
        emit errorMessageReceived("Выбран JSON-файл: " + selectedFilePath);
    }
    else if (fileExtension == "csv")
    {
        dataExtractor = std::make_unique<CsvDataExtractor>();
        emit errorMessageReceived("Выбран CSV-файл: " + selectedFilePath);
    }
    else
    {
        dataExtractor = nullptr; // Если тип файла неизвестен или не поддерживается
        emit errorMessageReceived("Неподдерживаемый тип файла");
    }

    // Извлечение данных с использованием DataExtractor
    if (dataExtractor->checkFile(selectedFilePath))
    {
        extractedData = dataExtractor->extractData(selectedFilePath);
        // Вызов слота changeChartType с передачей типа диаграммы
        QString selectedChartType = chartTypeComboBox->currentText();
        changeChartType(selectedChartType);
    } else
        emit errorMessageReceived("Произошла ошибка при проверке файла");
    }
}

void MainWindow::changeChartType(const QString& type)
{
    // Проверяем, был ли выбран файл
    if (selectedFilePath.isEmpty()) {
    // Если файл не выбран, не выполняем никаких действий
    return;
    }
    // Определение типа диаграммы на основе выбранного значения в combobox
    if (type == "Столбчатая диаграмма") {
    container.RegisterFactory<ChartRenderer, BarChartRenderer>();
    chartRenderer = container.GetObject<ChartRenderer>();
    } else if (type == "Круговая диаграмма") {
    container.RegisterFactory<ChartRenderer, PieChartRenderer>();
    chartRenderer = container.GetObject<ChartRenderer>();
    } else if (type == "Горизонтальная столбчатая диаграмма") {
    container.RegisterFactory<ChartRenderer, HorizontalBarChartRenderer>();
    chartRenderer = container.GetObject<ChartRenderer>();
    }

    if (chartRenderer) {
    // Удаление errorLabel из макета

        QLayout* layout = chartViewWidget->layout();
        if (layout) {
            layout->removeWidget(errorLabel.get());
            // Удаление errorLabel и освобождение памяти
            errorLabel.reset();
        }

        chartView = std::make_shared<QChartView>(this);
        // Создаем умный указатель на QVBoxLayout и выделяем память для него
        // Добавляем errorLabel в макет
        layout->addWidget(chartView.get());
        // Устанавливаем макет для chartViewWidget
        chartViewWidget->setLayout(layout);


//    // Создание и настройка QChartView
//    std::shared_ptr<QChartView> chartView = std::make_shared<QChartView>();
//    chartView->setStyleSheet("background-color: brown;");

//    // Добавление chartView в макет
//    layout->addWidget(chartView.get());
//    chartViewWidget->setLayout(layout);

    chartRenderer->renderChart(extractedData, chartView);
    } else {
    // Обработка ошибки: не удалось получить объект ChartRenderer из контейнера
    emit errorMessageReceived("Ошибка: невозможно создать объект диаграммы");
    }
}

void MainWindow::printErrorLabel(QString text)
{

    errorLabel = std::make_unique<QLabel>(this);
    errorLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    // Создаем умный указатель на QVBoxLayout и выделяем память для него
    std::unique_ptr<QVBoxLayout> layout = std::make_unique<QVBoxLayout>();
    // Добавляем errorLabel в макет
    layout->addWidget(errorLabel.get());
    // Устанавливаем макет для chartViewWidget
    chartViewWidget->setLayout(layout.release());
    // Обновляем текст errorLabel
    errorLabel->setText(text);
}

void MainWindow::updateChartColorMode(bool isChecked)
{
    if (chartView) {
    if (isChecked) {
            QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
            effect->setColor(Qt::black);
            chartView->chart()->setGraphicsEffect(effect);
    } else {
            // Цветная цветовая схема (по умолчанию)
            chartView->chart()->setGraphicsEffect(nullptr);
    }
    }
}


void MainWindow::exportChart()
{
    if (!chartView)
    return;

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Экспорт диаграммы", "", "PDF (*.pdf)");
    if (filePath.isEmpty())
    return;

    QPdfWriter pdfWriter(filePath);
    QPainter painter(&pdfWriter);

    chartView->render(&painter);

    painter.end();
}
