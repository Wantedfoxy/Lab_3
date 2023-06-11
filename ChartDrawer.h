#ifndef CHARTDRAWER_H
#define CHARTDRAWER_H

#include <QChartView>
#include <memory>
#include <string>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QLineSeries>
#include <QHorizontalBarSeries>
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>

// Абстрактный класс для отрисовки диаграммы
class ChartRenderer
{
public:
    virtual ~ChartRenderer() {}

    /**
     * renderChart - Метод для отрисовки диаграммы
     * extractedData - Извлеченные данные для отрисовки
     * chartView - Указатель на объект QChartView, в котором будет отрисована диаграмма
     */

    virtual void renderChart(QList<QPair<QString, QString>> extractedData, std::unique_ptr<QChartView>& chartView) = 0;
};

// Конкретный класс для отрисовки круговой диаграммы
class PieChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::unique_ptr<QChartView>& chartView) override
    {
        chartView->chart()->removeAllSeries();

        std::unique_ptr<QPieSeries> series = std::make_unique<QPieSeries>();
        for (const QPair<QString, QString>& pair : extractedData) {
            QString time = pair.first;
            qreal value = pair.second.toDouble();
            series->append(time, value);
        }
        // Освобождаем владение указателем перед добавлением серии
        chartView->chart()->addSeries(series.release());
        chartView->chart()->setTitle("Круговая диаграмма");
        chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->update();
    }
};

// Конкретный класс для отрисовки столбчатой диаграммы
class BarChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::unique_ptr<QChartView>& chartView) override
    {
        chartView->chart()->removeAllSeries();
        std::unique_ptr<QBarSeries> series(new QBarSeries());
        for (const QPair<QString, QString>& pair : extractedData) {
            QString time = pair.first;
            qreal value = pair.second.toDouble();

            std::unique_ptr<QBarSet> barSet(new QBarSet(time));
            *barSet << value;
            series->append(barSet.release());
        }
        // Освобождаем владение указателем перед добавлением серии
        chartView->chart()->addSeries(series.release());
        chartView->chart()->setTitle("Столбчатая диаграмма");
        chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->update();
    }
};

// Конкретный класс для отрисовки столбчатой горизонтальной диаграммы
class HorizontalBarChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::unique_ptr<QChartView>& chartView) override
    {
        chartView->chart()->removeAllSeries();
        std::unique_ptr<QHorizontalBarSeries> series(new QHorizontalBarSeries());
        for (const QPair<QString, QString>& pair : extractedData) {
            QString time = pair.first;
            qreal value = pair.second.toDouble();

            std::unique_ptr<QBarSet> barSet(new QBarSet(time));
            *barSet << value;
            series->append(barSet.release());
        }
        // Освобождаем владение указателем перед добавлением серии
        chartView->chart()->addSeries(series.release());
        chartView->chart()->setTitle("Столбчатая горизонтальная диаграмма");
        chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->update();
    }
};

#endif // CHARTDRAWER_H
