#pragma once
#ifndef PEAKSOLVER_H
#define PEAKSOLVER_H

/*
	Fichier : PeaksSolver.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant le problème de fonction "peaks" dans le laboratoire.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "vector"

#include "DESolution.h"

class PeaksSolver
{
public:
	void solve(size_t populationSize, size_t generationCount, size_t realPrecision);
	std::vector<DESolution>& FinalSolution();
private:
	std::vector<DESolution> mFinalSolutions;
	static double objectiveFuncPeaks(DESolution const& sol);
	static double fitnessFuncPeaksMax(double score);
	static double fitnessFuncPeaksMin(double score);
};

#endif // PEAKSOLVER_H
