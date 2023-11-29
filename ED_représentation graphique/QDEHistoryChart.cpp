#include "QDEHistoryChart.h"


#include "QDEAdapter.h"
#include <QValueAxis>


QDEHistoryChart::QDEHistoryChart(QWidget *parent)
	: QChartView(parent),
		mChart{ new QChart },
		mBestData{ new QLineSeries },
		mMedianData{ new QLineSeries },
		mWorstData{ new QLineSeries }
{
	mChart->legend()->hide();
	mChart->addSeries(mMedianData);
	mChart->addSeries(mBestData);
	mChart->addSeries(mWorstData);
	mChart->createDefaultAxes();
	mChart->setTitle("Évolution");
	mChart->axes(Qt::Horizontal)[0]->setTitleText("Génération");
	mChart->axes(Qt::Vertical)[0]->setTitleText("'Fitness'");

	static_cast<QValueAxis*>(mChart->axes(Qt::Horizontal)[0])->setLabelFormat("%i");

	mBestData->setColor(QColor(128, 128, 255));
	mMedianData->setColor(QColor(196, 196, 255));
	mWorstData->setColor(QColor(222, 196, 255));

	setChart(mChart);
	setRenderHint(QPainter::Antialiasing);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void QDEHistoryChart::updateHistory(QDEAdapter const & de)
{
	mBestData->append(mBestData->count(), de.statisticsHistory().last().bestSolution().fitness());
	mMedianData->append(mMedianData->count(), de.statisticsHistory().last().medianSolution().fitness());
	mWorstData->append(mWorstData->count(), de.statisticsHistory().last().worstSolution().fitness());

	double xMin, xMax, yMin, yMax;
	de.statisticsHistory().getRange(xMin, xMax, yMin, yMax);
	mChart->axes(Qt::Horizontal)[0]->setRange(xMin + 1.0, xMax + 1.0);
	mChart->axes(Qt::Vertical)[0]->setRange(yMin, yMax);
}

void QDEHistoryChart::reset()
{
	mBestData->clear();
	mMedianData->clear();
	mWorstData->clear();
	mChart->axes(Qt::Horizontal)[0]->setRange(0.0, 1.0);
	mChart->axes(Qt::Vertical)[0]->setRange(0.0, 1.0);
}
