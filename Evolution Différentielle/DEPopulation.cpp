#include <vector>

#include "DEPopulation.h"
#include "DESolutionBounds.h"

size_t DEPopulation::size() const {
	return mSolutions.size();
}

DESolution const& DEPopulation::operator[](int i) const
{
	return mSolutions[i];
}

DESolution& DEPopulation::operator[](int i)
{
	return mSolutions[i];
}

DESolution const& DEPopulation::operator[](size_t i) const
{
	return mSolutions[i];
}

DESolution& DEPopulation::operator[](size_t i)
{
	return mSolutions[i];
}

void DEPopulation::setup(size_t populationSize, DESolutionBounds solutionBounds)
{
	mSolutions.resize(populationSize);

	for (auto& solution : mSolutions) {
		solution.setup(solutionBounds);
		solution.randomize(solutionBounds);
	}
}

void DEPopulation::resizeBounds(size_t boundsSize)
{
	for (auto& solution : mSolutions) {
		solution.resizeBounds(boundsSize);
	}
}

void DEPopulation::randomize(DESolutionBounds& solutionBounds)
{
	for (auto& i : mSolutions)
		i.randomize(solutionBounds);
}

std::vector<DESolution>& DEPopulation::solutions()
{
	return mSolutions;
}
