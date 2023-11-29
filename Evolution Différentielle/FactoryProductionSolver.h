#pragma once
#ifndef FACTORYPRODUCTIONSOLVER_H
#define FACTORYPRODUCTIONSOLVER_H

/*
	Fichier : FactoryProductionSolver.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant le problème d'usines données dans le laboratoire.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "DEsolution.h"

class FactoryProductionSolver
{
public:
	void solve(size_t scenario);
	DESolution& FinalSolution();
private:
	DESolution mFinalSolution;
	static double objectiveFuncFactory1(DESolution const& sol);
	static double objectiveFuncFactory2(DESolution const& sol);
	static double objectiveFuncFactory3(DESolution const& sol);
	static double fitnessFuncFactory(double score);
};

#endif // FACTORYPRODUCTIONSOLVER_H