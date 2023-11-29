#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "PeaksSolverInterface.h"

void PeaksSolverInterface::solve()
{
	std::cout << problemPresentation();
	dataRecuperation();
	solution.solve(mPopulationSize, mGenerationSize, mFloatingPointPrecision);
	std::cout << solutionPresentation();
}

std::string PeaksSolverInterface::problemPresentation()
{
	std::string str("Voici le probleme de la fonction peaks: \nOn desire trouver le minimum et maximum");
	str.append(" de la courbe peaks de Matlab.\n");
	return str;
}

std::string PeaksSolverInterface::solutionPresentation()
{
	auto& result{ solution.FinalSolution() };
	std::stringstream ss1x, ss1y, ss2x, ss2y;
	ss1x << std::fixed << std::setprecision(mFloatingPointPrecision) << result[0][0];
	ss1y << std::fixed << std::setprecision(mFloatingPointPrecision) << result[0][1];
	ss2x << std::fixed << std::setprecision(mFloatingPointPrecision) << result[1][0];
	ss2y << std::fixed << std::setprecision(mFloatingPointPrecision) << result[1][1];

	// Min
	std::string str("Les coordonnees du minimum sont: x(");
	str.append(ss1x.str());
	str.append("), y(");
	str.append(ss1y.str());
	str.append(")\n");

	// Max
	str.append("Les coordonnees du maximum sont: x(");
	str.append(ss2x.str());
	str.append("), y(");
	str.append(ss2y.str());
	str.append(")\n\n");
	return str;
}

void PeaksSolverInterface::dataRecuperation()
{
	// TODO: Fonctionne avec des mauvais inputs (try, catch).
	std::cout << "Quelle est la taille de la population P (defaut = 10): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mPopulationSize = static_cast<size_t>(std::stoull(response));
	else
		mPopulationSize = 10;

	std::cout << "Quel est le nombre de generations Gmax (defaut = 100): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mGenerationSize = static_cast<size_t>(std::stoull(response));
	else
		mGenerationSize = 100;

	std::cout << "Quelle est la precision des points flottants a afficher (defaut = 6): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mFloatingPointPrecision = static_cast<size_t>(std::stoull(response));
	else
		mFloatingPointPrecision = 6;
}

