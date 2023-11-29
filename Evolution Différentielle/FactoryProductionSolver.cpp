#include <cmath>
#include <vector>

#include "FactoryProductionSolver.h"
#include "DEParameters.h"
#include "DESolution.h"
#include "DESolutionBounds.h"
#include "DifferentialEvolution.h"
#include "Interval.h"

constexpr int factory1TotalRessources = 10;
constexpr int factory2TotalRessources = 5000;
constexpr int factory3TotalRessources = 200;

double FactoryProductionSolver::objectiveFuncFactory1(DESolution const& sol)
{
	return (std::round(sol[0]) * 0.5 <= factory1TotalRessources
		&& std::round(sol[0]) > 0.0)
		? std::round(sol[0]) * 2.0 : 0.0;
}

// Voir commentaires dans objectiveFuncFactory3
double FactoryProductionSolver::objectiveFuncFactory2(DESolution const& sol)
{
	if (sol[0] < 0.0 || sol[1] < 0.0) {
		return -(std::fabs(sol[0]) + std::fabs(sol[1]));
	}

	double score = std::round(sol[0]) * 200.0 + std::round(sol[1]) * 400.0;
	double gains = std::min(std::round(sol[0]), std::floor(factory2TotalRessources / 4.0)) * 950.0
		+ std::min(std::round(sol[1]), std::floor(factory2TotalRessources / 2.0)) * 2000.0;

	if (score <= factory2TotalRessources)
		return gains;
	else
		return (factory2TotalRessources / (score * 10.0e3)) * gains;
}

// Il faut faire une fonction continue pour permettre à l'aglorithme de trouver la bonne solution.
double FactoryProductionSolver::objectiveFuncFactory3(DESolution const& sol)
{
	// Les valeurs négatives sont inacceptables.
	if (sol[0] < 0.0 || sol[1] < 0.0 || sol[2] < 0.0 || sol[3] < 0.0) {
		return -(std::fabs(sol[0]) + std::fabs(sol[1]) + std::fabs(sol[2]) + std::fabs(sol[3]));
	}
	// Le score est la quantité de ressources utilisées.
	double score = std::round(sol[0]) * 4.0 + std::round(sol[1]) * 2.0
		+ std::round(sol[2]) * 6.0 + std::round(sol[3]) * 5.0;

	// Les gains sont les profits.
	// On choisi le minimum entre la solution et la quantité théorique maximale. Ça permet
	// de pénaliser passivement lorsqu'on dépasse cette quantité.
	double gains = std::min(std::round(sol[0]), std::floor(factory3TotalRessources / 4.0)) * 100.0
		+ std::min(std::round(sol[1]), std::floor(factory3TotalRessources / 2.0)) * 60.0
		+ std::min(std::round(sol[2]), std::floor(factory3TotalRessources / 6.0)) * 160.0
		+ std::min(std::round(sol[3]), std::floor(factory3TotalRessources / 5.0)) * 120.0;

	if (score <= factory3TotalRessources)
		return gains;
	else {
		// On donne un gros déficite si on dépasse les ressources.
		return (factory3TotalRessources / (score * 10.0e3)) * gains;
	}
}

double FactoryProductionSolver::fitnessFuncFactory(double score)
{
	return score;
}

void FactoryProductionSolver::solve(size_t scenario)
{
	// 3.3 
	// Les valeurs 300, 150, 0.6, 0.8 ont été optimisées pour 200 ressources.
	// Avec différentes quantités de ressources, il faudrait peut-être les varier.
	DEParameters mParameters{ DEParameters(
		300,	// Grandeur de population
		0.6,	// F
		0.8,	// CR
		150,	// Générations
		6,		// Précision
		DESolutionBounds(std::vector<Interval>{ Interval(0.0, std::floor(factory3TotalRessources / 4.0)),
			Interval(0.0, std::floor(factory3TotalRessources / 2.0)),
			Interval(0.0, std::floor(factory3TotalRessources / 6.0)),
			Interval(0.0, std::floor(factory3TotalRessources / 5.0)) })
		, FactoryProductionSolver::objectiveFuncFactory3, FactoryProductionSolver::fitnessFuncFactory) };
	DifferentialEvolution mDEEngine{ mParameters };

	// 3.1
	if (scenario == 1) {
		mDEEngine.Parameters().setSolutionsBounds(std::vector<Interval>
		{ Interval(0.0, std::floor(factory1TotalRessources / 0.5)) });
		mDEEngine.Parameters().setObjFunc(FactoryProductionSolver::objectiveFuncFactory1);
		mDEEngine.resize(mDEEngine.Parameters().SolutionBounds().size());
	}
	// 3.2
	else if (scenario == 2) {

		mDEEngine.Parameters().setSolutionsBounds(std::vector<Interval>
		{	Interval(0.0, std::floor(factory2TotalRessources / 200.0)),
			Interval(0.0, std::floor(factory2TotalRessources / 400.0)) });
		mDEEngine.Parameters().setObjFunc(FactoryProductionSolver::objectiveFuncFactory2);
		mDEEngine.resize(mDEEngine.Parameters().SolutionBounds().size());
	}


	mDEEngine.evolve();
	mFinalSolution = mDEEngine.getFinalResultRounded();
}

DESolution& FactoryProductionSolver::FinalSolution()
{
	return mFinalSolution;
}