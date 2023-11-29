#include <string>
#include <vector>

#include "DEParameters.h"
#include "DESolution.h"
#include "DESolutionBounds.h"
#include "DifferentialEvolution.h"
#include "Interval.h"
#include "OpenBoxSolver.h"

double OpenBoxSolver::objectiveFuncBox(DESolution const& sol)
{
	if (sol[0] < 0 || sol[0] > 50)
		return 0.0;
	return (100 - (2.0 * sol[0])) *
		(50 - (2.0 * sol[0])) * sol[0];
}

double OpenBoxSolver::fitnessFuncBox(double score)
{
	return score;
}

void OpenBoxSolver::solve(double F, double CR)
{
	DEParameters mParameters{ DEParameters(100, F, CR, 80, 6,
		DESolutionBounds(std::vector<Interval>{ Interval(1.0, 49.9) })
		, OpenBoxSolver::objectiveFuncBox, OpenBoxSolver::fitnessFuncBox) };
	DifferentialEvolution mDEEngine{ mParameters };

	mDEEngine.evolve();
	mFinalSolution = mDEEngine.getFinalResult();
}

DESolution& OpenBoxSolver::FinalSolution()
{
	return mFinalSolution;
}