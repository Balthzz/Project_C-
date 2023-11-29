#include <vector>

#include "DESolutionBounds.h"
#include "Interval.h"

DESolutionBounds::DESolutionBounds(std::vector<Interval> dimensions)
	:mSolutionBounds{ dimensions }
{
}

size_t DESolutionBounds::size() const
{
	return mSolutionBounds.size();
}

double DESolutionBounds::getAtPositionRandom(unsigned int position)
{
	return mSolutionBounds.at(position).randomize();
}

std::vector<Interval>& DESolutionBounds::getBounds()
{
	return mSolutionBounds;
}