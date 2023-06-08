#ifndef CHARTDRAWER_H
#define CHARTDRAWER_H

#include <memory>
#include <string>
#include <QDebug>

// Абстрактный класс для отрисовки диаграммы
class ChartRenderer
{
public:
    virtual ~ChartRenderer() {}
    virtual void renderChart() = 0;
};

// Конкретный класс для отрисовки круговой диаграммы
class PieChartRenderer : public ChartRenderer
{
public:
    void renderChart() override
    {
        // Реализация отрисовки круговой диаграммы
        qDebug() << "Rendering Pie Chart";
    }
};

// Конкретный класс для отрисовки столбчатой диаграммы
class BarChartRenderer : public ChartRenderer
{
public:
    void renderChart() override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Bar Chart";
    }
};

// Конкретный класс для отрисовки линейной диаграммы
class LineChartRenderer : public ChartRenderer
{
public:
    void renderChart() override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Line Chart";
    }
};

// Конкретный класс для отрисовки гистограммы
class HistogramChartRenderer : public ChartRenderer
{
public:
    void renderChart() override
    {
        // Реализация отрисовки столбчатой диаграммы
        qDebug() << "Rendering Histogram Chart";
    }
};

// Класс ChartDrawer
class ChartDrawer
{
private:
    std::shared_ptr<ChartRenderer> chartRenderer;

public:
    ChartDrawer()
    {



    }

    void drawChart()
    {
        // Вызов метода отрисовки диаграммы через ChartRenderer
        chartRenderer->renderChart();
    }
};

#endif // CHARTDRAWER_H
