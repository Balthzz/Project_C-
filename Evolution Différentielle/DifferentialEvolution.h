#pragma once
#ifndef DIFFERENTIALEVOLUTION_H
#define DIFFERENTIALEVOLUTION_H

/*
	Fichier : DifferentialEvolution.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant la grosse partie de l'algorithme d'évolution
	différentielle. Il faut simplement appeler evolve() pour laisser
	la classe exécuter les fonctions nécessaires.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "DEParameters.h"
#include "DEPopulation.h"
#include "DESolution.h"
#include "DEStatistics.h"
#include "Interval.h"
#include "IntervalWholeNumbers.h"
#include "ThreeSamplesWithOneExclusion.h"


class DifferentialEvolution
{
public:
	DifferentialEvolution(DEParameters& params);
	bool isReady();
	void resize(size_t size);
	void setup(DEParameters& parameters);
	void reset();
	bool evolveOne();
	bool evolve();

	DESolution getFinalResult();
	DESolution getFinalResultRounded();
	DESolution getMaxFitness();
	DEParameters& Parameters();

private:
	DEParameters mParameters;
	DEPopulation mPopulation;
	DEPopulation mMutant;
	DEPopulation mTrial;
	size_t mCurrentGeneration;
	DEStatistics mStatistics;
	ThreeSamplesWithOneExclusion mSamplingTool;
	IntervalWholeNumbers separatorInterval;
	Interval comparatorInterval;

	void processFitness(DEPopulation& population);
	void processMutation();
	void processCrossover();
	void processSelection();
	void processStatistics();
	void printCurrentGeneration(); // Pour debug seulement
};

#endif // DIFFERENTIALEVOLUTION_H
