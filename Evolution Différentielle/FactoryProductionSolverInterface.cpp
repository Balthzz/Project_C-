#include <iostream>
#include <string>

#include "FactoryProductionSolverInterface.h"

void FactoryProductionSolverInterface::solve()
{
	std::cout << problemPresentation();
	dataRecuperation();
	solution.solve(mScenario);
	std::cout << solutionPresentation();
}

std::string FactoryProductionSolverInterface::problemPresentation()
{
	std::string str("Voici le probleme des usines: \nOn desire optimiser la production d'une entreprise");
	str.append(" manufacturiere en fonction de parametres pour avoir le meilleur profit possible. Il faut respecter");
	str.append(" la disponibilite des ressources et de rendre les nombres de produits positifs.\n");
	return str;
}

std::string FactoryProductionSolverInterface::solutionPresentation()
{
	auto& result{ solution.FinalSolution() };
	std::string str("Voici la solution optimale:\n");
	for (int i{}; i < result.data().size(); i++) {
		str.append("i");
		str.append(std::to_string(i));
		str.append(": ");
		str.append(std::to_string(result[i]));
		str.append("\n");
	}
	return str;
}

void FactoryProductionSolverInterface::dataRecuperation()
{
	// TODO: Fonctionne avec des mauvais inputs (try, catch).
	std::cout << "Quel scenario voulez-vous? [1-3] (defaut = 3): ";
	std::getline(std::cin, response);
	if (!response.empty())
		mScenario = static_cast<size_t>(std::stoull(response));
	else
		mScenario = 3;
	
	if(mScenario <= 0 || mScenario >= 4)
		mScenario = 3;
}

