#pragma once
#ifndef DEPARAMETERS_H
#define DEPARAMETERS_H

/*
	Fichier : DEParameters.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Paramètres qui se font passer à la classe Differential Evolution.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "Definitions.h"
#include "DESolutionBounds.h"

class DEParameters
{
public:
	DEParameters(de_objective_function_t objFunc, de_fitness_function_t fitFunc);
	DEParameters(size_t populationSize, double f, double cr, size_t maxGenerationCount, 
		unsigned char precision, DESolutionBounds solBounds, de_objective_function_t objFunc,
		de_fitness_function_t fitFunc);
	bool isReady() const;
	void reset();
	size_t PopulationSize() const;
	size_t MaxGenerationCount() const;
	DESolutionBounds& SolutionBounds();
	double F() const;
	double CR() const;
	unsigned char Precision() const;
	de_objective_function_t ObjFunc() const;
	de_fitness_function_t FitnessFunc() const;
	void setPopulationSize(size_t populationSize);
	void setGenerationCount(size_t generationCount);
	void setRealPrecision(size_t realPrecision);
	void setSolutionsBounds(DESolutionBounds solBounds);

	void setObjFunc(de_objective_function_t objFunc);
	void setFitFunc(de_fitness_function_t fitFunc);

private:
	size_t mPopulationSize;
	double mF;
	double mCR;
	size_t mMaxGenerationCount;
	unsigned char mPrecision;
	de_objective_function_t mObjFunc;
	de_fitness_function_t mFitnessFunc;
	DESolutionBounds mSolutionBounds;
};

#endif // DEPARAMETERS_H