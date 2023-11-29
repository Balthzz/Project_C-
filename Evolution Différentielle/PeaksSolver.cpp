#include <cmath>
#include <vector>

#include "DEParameters.h"
#include "DESolutionBounds.h"
#include "DifferentialEvolution.h"
#include "Interval.h"
#include "PeaksSolver.h"

constexpr double epsilon = 2.7182818284;

double PeaksSolver::objectiveFuncPeaks(DESolution const& sol) {
	// Formule retranscrite de l'énoncé qui a été divisé en petites parties.
	double firstPart = 3.0 * pow(1.0 - sol[0], 2.0)
		* pow(epsilon, -(pow(sol[0], 2.0) + pow(sol[1] + 1.0, 2.0)));
	double middlestartPart = 10.0 * ((sol[0] / 5.0) - pow(sol[0], 3.0) - pow(sol[1], 5.0));
	double middleendPart = pow(epsilon, -(pow(sol[0], 2.0) + pow(sol[1], 2.0)));
	double endPart = (1.0 / 3.0) * pow(epsilon, -(pow(sol[1] + 1, 2) + pow(sol[1], 2)));
	return firstPart - (middlestartPart * middleendPart) - endPart;
}

double PeaksSolver::fitnessFuncPeaksMax(double score)
{
	return score;
}

double PeaksSolver::fitnessFuncPeaksMin(double score)
{
	return -score;
}

void PeaksSolver::solve(size_t populationSize, size_t generationCount, size_t realPrecision)
{
	DEParameters mParameters{ DEParameters(populationSize, 0.8, 0.9, generationCount, static_cast<unsigned char>(realPrecision),
		DESolutionBounds(std::vector<Interval>{ Interval(-3.0, 3.0), Interval(-3.0, 3.0)}), 
		PeaksSolver::objectiveFuncPeaks, PeaksSolver::fitnessFuncPeaksMin) };
	DifferentialEvolution mDEEngine{ mParameters };

	// On va chercher le minimum.
	mDEEngine.evolve();
	mFinalSolutions.emplace_back(std::move(mDEEngine.getFinalResult()));

	// On remet à zéro l'algorithme pour trouver le maximum par la suite.
	mDEEngine.reset();
	mDEEngine.Parameters().setFitFunc(PeaksSolver::fitnessFuncPeaksMax);

	// On va chercher le maximum.
	mDEEngine.evolve();
	mFinalSolutions.emplace_back(std::move(mDEEngine.getFinalResult()));
}

std::vector<DESolution>& PeaksSolver::FinalSolution()
{
	return mFinalSolutions;
}
