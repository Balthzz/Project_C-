#include <DifferentialEvolution.h>
#include <PeakFunctionMinMaxSolution.h>

void delibBasicDemo()
{
	de::DifferentialEvolution deEngine;
	de::EngineParameters deParameters;
	de::PeakFunctionMinMaxSolution deSolution;

	deParameters.setPopulationSize(25);
	deParameters.setMaxGeneration(250);

	deEngine.setup(std::move(deParameters), new de::PeakFunctionMinMaxSolution);

	deEngine.process();

	std::string bestSolution(deEngine.bestSolutionInfo());
}