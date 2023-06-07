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
#include <QChart>
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
    void openFolder();
    void changeChartType(const QString&);

private:
    std::unique_ptr<QPushButton> openFolderButton;
    std::unique_ptr<QLabel> chartTypeLabel;
    std::unique_ptr<QLabel> errorLabel;
    std::unique_ptr<QComboBox> chartTypeComboBox;
    std::unique_ptr<QCheckBox> colorfulCheckbox;
    std::unique_ptr<QPushButton> exportButton;
    std::unique_ptr<QTreeView> fileTreeView;
    std::unique_ptr<QChartView> ChartView;
    std::unique_ptr<QFileSystemModel> fileSystemModel;
    std::unique_ptr<QSplitter> splitter;
};

#endif // MAINWINDOW_H
