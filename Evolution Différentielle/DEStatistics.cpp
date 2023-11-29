#include "DESolution.h"
#include "DEStatistics.h"

void DEStatistics::add(DESolution const& solution)
{
	mStatistics.emplace_back(solution);
}

void DEStatistics::reset()
{
	mStatistics.clear();
}

size_t DEStatistics::size()
{
	return mStatistics.size();
}

DESolution DEStatistics::last()
{
	return mStatistics.back();
}
