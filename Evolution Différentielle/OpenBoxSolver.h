#pragma once
#ifndef OPENBOXSOLVER_H
#define OPENBOXSOLVER_H

/*
	Fichier : Interval.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant le problème d'optimisation de boîte dans le laboratoire.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "DifferentialEvolution.h"

class OpenBoxSolver
{
public:
	void solve(double F, double CR);
	DESolution& FinalSolution();
private:
	DESolution mFinalSolution;
	static double objectiveFuncBox(DESolution const& sol);
	static double fitnessFuncBox(double score);
};

#endif // OPENBOXSOLVER_H
