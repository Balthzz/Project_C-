#pragma once


#include <QChartView>
#include <QChart>
#include <QLineSeries>

class QDEAdapter;


class QDEHistoryChart : public QChartView
{
	Q_OBJECT

public:
	QDEHistoryChart(QWidget * parent = nullptr);
	QDEHistoryChart(QDEHistoryChart const &) = default;
	QDEHistoryChart(QDEHistoryChart &&) = default;
	QDEHistoryChart& operator=(QDEHistoryChart const &) = default;
	QDEHistoryChart& operator=(QDEHistoryChart &&) = default;
	~QDEHistoryChart() override = default;

public slots:
	void updateHistory(QDEAdapter const & de);
	void reset();

private:
	QChart * mChart{};
	QLineSeries * mBestData{};
	QLineSeries * mMedianData{};
	QLineSeries * mWorstData{};
};
