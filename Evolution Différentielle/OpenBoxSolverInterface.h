#pragma once
#ifndef OPENBOXSOLVERINTERFACE_H
#define OPENBOXSOLVERINTERFACE_H

/*
	Fichier : OpenBoxSolverInterface.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe facilitant l'interaction avec l'utilisateur.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <string>

#include "OpenBoxSolver.h"

class OpenBoxSolverInterface
{
public:
	std::string problemPresentation();
	std::string solutionPresentation();
	void dataRecuperation();
	void solve();
private:
	OpenBoxSolver solution;
	double mF;
	double mCR;
	std::string response;
};


#endif // OPENBOXSOLVERINTERFACE_H