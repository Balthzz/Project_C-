#include "QDEAdapter.h"


QDEAdapter::QDEAdapter(QObject *parent)
	: QObject(parent)
{
}

bool QDEAdapter::isReady()
{
	return mDE.isReady();
}

size_t QDEAdapter::currentGeneration() const
{
	return mDE.currentGeneration();
}

bool QDEAdapter::isStoppingReached() const
{
	return mDE.isStoppingReached();
}

bool QDEAdapter::setup(de::EngineParameters && engineParameters, de::SolutionStrategy * sol)
{
	if (sol) {
		return mDE.setup(std::move(engineParameters), sol);
	}
	
	return false;
}

de::SolutionStrategy const & QDEAdapter::solution()
{
	return mDE.solution();
}

QString QDEAdapter::bestSolutionInfo() const
{
	return QString::fromStdString(mDE.bestSolutionInfo());
}

de::History const & QDEAdapter::statisticsHistory() const
{
	return mDE.statisticsHistory();
}

de::Population QDEAdapter::actualPopulation() const
{
	return mDE.actualPopulation();
}

void QDEAdapter::stepEvolution()
{
	if (!mDE.isStoppingReached()) {
		mDE.processToNextGeneration();
		emit oneEvolutionStepped(*this);
	}

	if (mDE.isStoppingReached()) {
		emit stoppingCriteriaReached();
	}
}

void QDEAdapter::reset()
{
	mDE.reset();
	emit reseted(*this);
}


