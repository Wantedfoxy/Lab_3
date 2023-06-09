#ifndef CHARTDRAWER_H
#define CHARTDRAWER_H

#include <QChartView>
#include <memory>
#include <string>
#include <QDebug>

// Абстрактный класс для отрисовки диаграммы
class ChartRenderer
{
public:
    virtual ~ChartRenderer() {}
    virtual void renderChart(QList<QPair<QString, QString>> extractedData, std::shared_ptr<QChartView> chartView) = 0;
};

// Конкретный класс для отрисовки круговой диаграммы
class PieChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::shared_ptr<QChartView> chartView) override
    {
        // Реализация отрисовки круговой диаграммы
        qDebug() << "Rendering Pie Chart";
    }
};

// Конкретный класс для отрисовки столбчатой диаграммы
class BarChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::shared_ptr<QChartView> chartView) override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Bar Chart";
    }
};

// Конкретный класс для отрисовки линейной диаграммы
class LineChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::shared_ptr<QChartView> chartView) override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Line Chart";
    }
};

// Конкретный класс для отрисовки гистограммы
class HistogramChartRenderer : public ChartRenderer
{
public:
    void renderChart(QList<QPair<QString, QString>> extractedData, std::shared_ptr<QChartView> chartView) override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Histogram Chart";
    }
};

#endif // CHARTDRAWER_H
