#pragma once
#ifndef FACTORYPRODUCTIONSOLVERINTERFACE_H
#define FACTORYPRODUCTIONSOLVERINTERFACE_H

/*
	Fichier : FactoryProductionSolverInterface.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe facilitant l'interaction avec l'utilisateur.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <string>

#include "FactoryProductionSolver.h"

class FactoryProductionSolverInterface
{
public:
	std::string problemPresentation();
	std::string solutionPresentation();
	void dataRecuperation();
	void solve();
private:
	FactoryProductionSolver solution;
	size_t mScenario;
	std::string response;
};

#endif // FACTORYPRODUCTIONSOLVERINTERFACE_H