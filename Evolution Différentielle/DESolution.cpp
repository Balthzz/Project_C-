#include <vector>

#include "DESolution.h"
#include "DESolutionBounds.h"

size_t DESolution::size() const
{
	return mData.size();
}

void DESolution::setup(DESolutionBounds const& solutionBounds)
{
	mData.resize(solutionBounds.size());
}

void DESolution::resizeBounds(size_t size)
{
	mData.resize(size);
}

void DESolution::randomize(DESolutionBounds& solutionBounds)
{
	for (int i{}; i < mData.size(); i++) {
		mData.at(i) = solutionBounds.getAtPositionRandom(i);
	}
}

std::vector<double>& DESolution::data()
{
	return mData;
}

std::vector<double> DESolution::data() const
{
	return mData;
}

void DESolution::setObjective(double obj)
{
	mObjective = obj;
}

void DESolution::setFitness(double fit)
{
	mFitness = fit;
}

double DESolution::Objective()
{
	return mObjective;
}

double DESolution::Fitness()
{
	return mFitness;
}

double const& DESolution::operator[](int i) const
{
	return mData[i];
}

double& DESolution::operator[](int i)
{
	return mData[i];
}
