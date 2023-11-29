#include <iostream>

#include "DEParameters.h"
#include "DEPopulation.h"
#include "DESolution.h"
#include "DifferentialEvolution.h"
#include "ThreeSamplesWithOneExclusion.h"

DifferentialEvolution::DifferentialEvolution(DEParameters& params)
	: mParameters{ params },
	mCurrentGeneration{ 1 },
	separatorInterval{ 1, static_cast<int>(mParameters.SolutionBounds().size()) },
	comparatorInterval{ 0.0, 1.0 }
{
	setup(mParameters);
}

bool DifferentialEvolution::isReady()
{
	return mParameters.isReady();
}

void DifferentialEvolution::resize(size_t size)
{
	separatorInterval.setUpper(static_cast<int>(size));
	mPopulation.resizeBounds(size);
	mMutant.resizeBounds(size);
	mTrial.resizeBounds(size);
}

void DifferentialEvolution::setup(DEParameters& parameters)
{
	mPopulation.setup(parameters.PopulationSize(), parameters.SolutionBounds());
	mMutant.setup(parameters.PopulationSize(), parameters.SolutionBounds());
	mTrial.setup(parameters.PopulationSize(), parameters.SolutionBounds());
	mSamplingTool.prepare(mParameters.PopulationSize());
}

void DifferentialEvolution::reset()
{
	setup(mParameters);
	mCurrentGeneration = 1;
	mStatistics.reset();
}

bool DifferentialEvolution::evolveOne()
{
	if (!isReady())
		return false;

	processMutation();
	processCrossover();
	processSelection();
	processStatistics();

	return true;
}

bool DifferentialEvolution::evolve()
{
	if (!isReady())
		return false;

	while (mCurrentGeneration < mParameters.MaxGenerationCount()) {
		processMutation();
		processCrossover();
		processSelection();
		processStatistics();

		mCurrentGeneration++;
	}
	return true;
}

void DifferentialEvolution::printCurrentGeneration()
{
	std::cout << "Les solutions:\n";
	for (auto& i : mPopulation.solutions()) {
		for (auto& j : i.data())
			std::cout << j << " ";
		std::cout << '\n';
	}
	std::cout << '\n';
}

DEParameters& DifferentialEvolution::Parameters()
{
	return mParameters;
}

DESolution DifferentialEvolution::getFinalResult()
{
	return mStatistics.last();
}

DESolution DifferentialEvolution::getFinalResultRounded()
{
	auto temp{ getFinalResult() };

	for (auto& i : temp.data()) 
	{
		i = std::round(i);
	}
	return temp;
}

DESolution DifferentialEvolution::getMaxFitness()
{
	double fit{ 0.0 };
	DESolution final;
	for (auto& i : mPopulation.solutions()) {

		if (i.Fitness() > fit) {
			fit = i.Fitness();
			final = i;
		}
	}
	return final;
}

void DifferentialEvolution::processFitness(DEPopulation& population)
{
	for (auto& i : population.solutions()) {
		double objValue = mParameters.ObjFunc()(i);
		i.setObjective(objValue);
		i.setFitness(mParameters.FitnessFunc()(objValue));
	}
}

void DifferentialEvolution::processMutation()
{
	size_t sol1{}, sol2{}, sol3{};

	for (int i{}; i < mMutant.solutions().size(); i++) {
		mSamplingTool.select(i, sol1, sol2, sol3);

		for (int j{}; j < mMutant[i].size(); j++) {
			mMutant[i][j] = mPopulation[sol1][j]
				+ mParameters.F() * (mPopulation[sol2][j] - mPopulation[sol3][j]);
		}
	}
}

void DifferentialEvolution::processCrossover()
{
	for (int i{}; i < mTrial.solutions().size(); i++) {
		int randomValue = separatorInterval.randomize();

		for (int j{}; j < mTrial[i].size(); j++) {
			double comparator = comparatorInterval.randomize();

			if (j == randomValue || comparator < mParameters.CR())
				mTrial[i][j] = mMutant[i][j];
			else
				mTrial[i][j] = mPopulation[i][j];
		}
	}
}

void DifferentialEvolution::processSelection()
{
	processFitness(mPopulation);
	processFitness(mTrial);

	for (int i{}; i < mPopulation.solutions().size(); i++) {

		if (mTrial[i].Fitness() >= mPopulation[i].Fitness())
			mPopulation[i] = mTrial[i];
	}
}

void DifferentialEvolution::processStatistics()
{
	mStatistics.add(getMaxFitness());
}
