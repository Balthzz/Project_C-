#pragma once
#ifndef PEAKSSOLVERINTERFACE_H
#define PEAKSSOLVERINTERFACE_H

/*
	Fichier : PeaksSolverInterface.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe facilitant l'interaction avec l'utilisateur.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <string>

#include "PeaksSolver.h"

class PeaksSolverInterface
{
public:
	std::string problemPresentation();
	std::string solutionPresentation();
	void dataRecuperation();
	void solve();
private:
	PeaksSolver solution;
	size_t mPopulationSize;
	size_t mGenerationSize;
	size_t mFloatingPointPrecision;
	std::string response;
};

#endif // PEAKSSOLVERINTERFACE_H