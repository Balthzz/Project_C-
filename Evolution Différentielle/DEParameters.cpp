#include "Definitions.h"
#include "DEParameters.h"
#include "DESolutionBounds.h"
#include "Interval.h"

DEParameters::DEParameters(de_objective_function_t objFunc, de_fitness_function_t fitFunc)
	: DEParameters(200, 1.2, 0.5, 500, 6,
		DESolutionBounds(std::vector<Interval>{ Interval(1.0, 49.50) }),
		objFunc, fitFunc)
{
}

DEParameters::DEParameters(size_t populationSize, double f, double cr,
	size_t maxGenerationCount, unsigned char precision, DESolutionBounds solBounds,
	de_objective_function_t objFunc, de_fitness_function_t fitFunc)
	: mPopulationSize{ populationSize }, mF{ f }, mCR{ cr },
	mMaxGenerationCount{ maxGenerationCount }, mSolutionBounds{ solBounds },
	mObjFunc{ objFunc }, mFitnessFunc{ fitFunc }, mPrecision{ precision }
{

}

bool DEParameters::isReady() const
{
	return !(mF < 0.0 || mF > 2.0 || mCR < 0.0 || mCR > 1.0 ||
		!mFitnessFunc || !mObjFunc);
}

void DEParameters::reset()
{
}

void DEParameters::setPopulationSize(size_t populationSize)
{
	mPopulationSize = populationSize;
}

void DEParameters::setGenerationCount(size_t generationCount)
{
	mMaxGenerationCount = generationCount;
}

void DEParameters::setRealPrecision(size_t realPrecision)
{
	mPrecision = static_cast<unsigned char>(realPrecision);
}

void DEParameters::setSolutionsBounds(DESolutionBounds solBounds)
{
	mSolutionBounds = solBounds;
}

void DEParameters::setObjFunc(de_objective_function_t objFunc)
{
	mObjFunc = objFunc;
}

void DEParameters::setFitFunc(de_fitness_function_t fitFunc)
{
	mFitnessFunc = fitFunc;
}

size_t DEParameters::PopulationSize() const
{
	return mPopulationSize;
}

size_t DEParameters::MaxGenerationCount() const
{
	return mMaxGenerationCount;
}

DESolutionBounds& DEParameters::SolutionBounds()
{
	return mSolutionBounds;
}

double DEParameters::F() const
{
	return mF;
}

double DEParameters::CR() const
{
	return mCR;
}

unsigned char DEParameters::Precision() const
{
	return mPrecision;
}

de_objective_function_t DEParameters::ObjFunc() const
{
	return mObjFunc;
}

de_fitness_function_t DEParameters::FitnessFunc() const
{
	return mFitnessFunc;
}