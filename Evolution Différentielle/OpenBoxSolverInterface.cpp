#include <iostream>
#include <string>

#include "OpenBoxSolverInterface.h"

void OpenBoxSolverInterface::solve() 
{
	std::cout << problemPresentation();
	dataRecuperation();
	solution.solve(mF, mCR);
	std::cout << solutionPresentation();
}

std::string OpenBoxSolverInterface::problemPresentation()
{
	std::string str("Voici le probleme de la boite: \nOn desire decouper une zone carree de longueur x");
	str.append(" dans chacun des quatre coins d'une page de facon a ce que lorsque les quatre ");
	str.append("zones laterales sont repliees, la feuille de carton forme une boite.\n");
	return str;
}

std::string OpenBoxSolverInterface::solutionPresentation()
{
	auto& result{ solution.FinalSolution() };
	std::string str("La dimension du carre qu'il faudrait couper pour avoir le plus grand volume serait: ");
	str.append(std::to_string(solution.FinalSolution().data()[0]));
	str.append(" \n\n");
	return str;
}

void OpenBoxSolverInterface::dataRecuperation()
{
	// TODO: Fonctionne avec des mauvais inputs (try, catch).
	std::cout << "Veuillez choisir un F (defaut: 2): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mF = std::stod(response);

	if (mF <= 0.0 || mF >= 2.0)
		mF = 2.0;

	std::cout << "Veuillez choisir un CR (defaut: 1): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mCR = std::stod(response);

	if (mCR <= 0.0 || mCR >= 1.0)
		mCR = 1.0;
}
