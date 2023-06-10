#include "MainWindow.h"

int IOCContainer::s_nextTypeId = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Инициализируем пустую строковую переменную, эта переменная следит, есть ли выбранный файл;
    selectedFilePath = "";
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

    // Создаем виджет, в который вложится QVBoxLayout, уже в который будут вкладываться отображение
    // диаграмм и QLabel для отображения сообщений пользователю
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

    // Создаем laiout, в котором будут отображаться QChartView и QLabel(для сообщений пользователю)
    layout = std::make_unique<QVBoxLayout>();
    chartView = std::make_unique<QChartView>(this);
    errorLabel = std::make_unique<QLabel>(this);
    errorLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    errorLabel->setVisible(false);
    layout->addWidget(errorLabel.get());
    layout->addWidget(chartView.get());
    chartViewWidget->setLayout(layout.get());

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
    // Сбрасываем флаг, так как диаграмма не была отрисована
    isChartRendered = false;

    // Задаем минимальный размер окна
    setMinimumSize(800, 600);
    // Задаем стандартный размер окна
    resize(1024, 768);


    // Устанавливаем соединение между сигналом clicked кнопки openFolderButton и слотом openFolder()
    connect(openFolderButton.get(), &QPushButton::clicked, this, &MainWindow::openFolder);
    // Устанавливаем соединение между сигналом получения ошибки и слотом, отвечающим за вывод этого
    // сообщения пользователю
    connect(this, SIGNAL(errorMessageReceived(QString)), this, SLOT(printErrorLabel(QString)));
    // Устанавливаем соединение между сигналом смены выбранного типа диаграммы и вызываем слот,
    // который отобразит выбранный тип
    connect(chartTypeComboBox.get(), SIGNAL(currentTextChanged(const QString&)), this,
            SLOT(changeChartType(const QString&)));
    // Устанавливаем соединение между сигналом отвечающим за цвета диаграммы и вызываем слот,
    // который задась диаграмме либо цветное отображение, либо черно-белое
    connect(BWCheckbox.get(), &QCheckBox::stateChanged, this, &MainWindow::updateChartColorMode);
    // станавливаем соединение между сигналом clicked кнопки exportButton и слотом exportChart()
    connect(exportButton.get(), &QPushButton::clicked, this, &MainWindow::exportChart);
}

MainWindow::~MainWindow()
{}

void MainWindow::openFolder()
{
    // Открываем диалоговое окно выбора папки
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath());
    // Создаем объект QDir с указанным путем folderPath, который представляет директорию,
    // указанную в folderPath, этот объект будет использоваться для выполнения операций с файлами
    // внутри указанной директории
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

            // Устанавливаем модель данных для QListView
            fileListView->setModel(fileSystemModel.get());
            // Устанавливаем корневой индекс на выбранную папку
            fileListView->setRootIndex(fileSystemModel->index(folderPath));

            // Создаем переменную, которая отвечает за выделенный файл в QListView
            ListSelectionModel = fileListView->selectionModel();
            // Cоединяем сигнал со слотом handleFileSelectionChanged, таким образом, при изменении
            // выделения элементов в списке, сигнал selectionChanged будет генерироваться объектом
            // ЦQItemSelectionModel и вызывать метод, который обработает файл
            connect(ListSelectionModel, &QItemSelectionModel::selectionChanged, this,
                    &MainWindow::handleFileSelectionChanged);
        } else {
            // Если указанная папка пуста, отображаем предупреждение
            emit errorMessageReceived("Указанная папка пуста");
        }
    } else {
        // Если указанная папка не существует, отображаем предупреждение
        emit errorMessageReceived("Указанная папка не существует");
    }
}

void MainWindow::handleFileSelectionChanged(const QItemSelection &selected)
{
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

        // Если файл прошел проверку
        if (dataExtractor->checkFile(selectedFilePath))
        {
            // Вытаскиваем данные из файла (в случае SQL дополнительно группируем по дате)
            extractedData = dataExtractor->extractData(selectedFilePath);
            // Вызываем слот changeChartType с передачей в него типа диаграммы
            // Это делается для мгновенной отрисовки диаграммы выбранного типа при выборе файла
            changeChartType(chartTypeComboBox->currentText());
        } else
            // Выводим сообщение об ошибке при проверке файла
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
    // Определяем тип диаграммы на основе выбранного значения в ComboBox
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
        if (errorLabel) {
            // Скрываем errorLabel
            errorLabel->setVisible(false);
        }
        // Показываем chartView
        chartView->setVisible(true);
        // Отрисовка диаграммы
        chartRenderer->renderChart(extractedData, chartView);
        // Устанавливаем флаг, что диаграмма была отрисована
        isChartRendered = true;
    } else {
        // Обработка ошибки: не удалось получить объект ChartRenderer из контейнера
        emit errorMessageReceived("Ошибка: невозможно создать объект диаграммы");
        // Сбрасываем флаг, так как диаграмма не была отрисована
        isChartRendered = false;
        if (chartView) {
            // Скрываем chartView
            chartView->setVisible(false);
        }
        // Показываем errorLabel
        errorLabel->setVisible(true);
    }
}

void MainWindow::printErrorLabel(QString text)
{
    if (chartView) {
        // Скрываем chartView
        chartView->setVisible(false);
    }
    errorLabel->setText(text);
}

void MainWindow::updateChartColorMode(bool isChecked)
{
    if (chartView) {
        if (isChecked) {
            std::unique_ptr<QGraphicsColorizeEffect> effect = std::make_unique<QGraphicsColorizeEffect>();
            // Устанавливаем цвет эффекта в черный
            effect->setColor(Qt::black);
            // Устанавливаем эффект цветовой схемы на диаграмму, используя get() для получения
            // обычного указателя на объект, а не умного указателя
            chartView->chart()->setGraphicsEffect(effect.get());
            // Управление умным указателем передачей владения
            effect.release();
        } else {
            // Если флаг isChecked равен false, то сбрасываем цветовую схему диаграммы,
            // устанавливая эффект в nullptr
            chartView->chart()->setGraphicsEffect(nullptr);
        }
    }
}


void MainWindow::exportChart()
{
    // Проверяем, существует ли объект chartView, чтобы избежать обращения к нулевому указателю
    if (!chartView || !isChartRendered)
        return;
    // Получаем путь к файлу PDF с помощью диалогового окна сохранения файла
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Экспорт диаграммы", "", "PDF (*.pdf)");
    // Если путь к файлу пустой (например, пользователь отменил сохранение), то прерываем выполнение метода
    if (filePath.isEmpty())
        return;
    // Создаем объект QPdfWriter для записи в файл PDF с использованием указанного пути filePath
    QPdfWriter pdfWriter(filePath);
    // Создаем объект QPainter, связанный с pdfWriter, для рисования на PDF-поверхности
    QPainter painter(&pdfWriter);
    // Рендерим содержимое chartView на поверхность, связанную с painter
    chartView->render(&painter);
    // Завершаем рисование
    painter.end();
}
