#include "GPA434Lab2DEApp.h"
#include "QDEBestResult.h"
#include "QDEController.h"
#include "QDEEngineParameters.h"
#include "QDEHistoryChart.h"
#include "QDEPcbSpaceOptimizer.h"
#include "QDEOpenBoxSolution.h"
#include "QDEPeakSolution.h"
#include "QDESolutionTab.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>


GPA434Lab2DEApp::GPA434Lab2DEApp(QWidget* parent)
	: QMainWindow(parent)
	, mDE{ }
	, mDEParams{ new QDEEngineParameters }
	, mDEController{ new QDEController(mDE) }
	, mOpenBoxSol{ new QDEOpenBoxSolution }
	, mPcbSol{ new QDEPcbSpaceOptimizer }
	, mPeakSol{ new QDEPeakSolution }
	, mSolutionTab{ new QDESolutionTab }
{
	// Définition du titre et de l'icône de la fenêtre.
	setWindowTitle("Évolution différentielle");
	setWindowIcon(QIcon(":/GPA434Lab2DEApp/AppIcon"));

	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* topDownLayout{ new QVBoxLayout };
	QWidget* topHorizontalWidget{ new QWidget };
	topHorizontalWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	topDownLayout->addWidget(topHorizontalWidget);

	QHBoxLayout* horizontalTopLayout{ new QHBoxLayout };
	topHorizontalWidget->setLayout(horizontalTopLayout);
	QWidget* controllerWidget{ new QWidget };
	controllerWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

	QSplitter* historySplitter{ new QSplitter };

	horizontalTopLayout->addWidget(controllerWidget);
	horizontalTopLayout->addWidget(historySplitter);

	QVBoxLayout* verticalTopLayout{ new QVBoxLayout };
	controllerWidget->setLayout(verticalTopLayout);

	// Paramètres et bouttons
	verticalTopLayout->addWidget(mDEParams);
	verticalTopLayout->addWidget(mDEController);

	// Historique
	auto hisChart = new QDEHistoryChart;
	auto bestResult = new QDEBestResult;
	bestResult->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	historySplitter->addWidget(hisChart);
	historySplitter->addWidget(bestResult);

	topDownLayout->addWidget(mSolutionTab);

	centralWidget->setLayout(topDownLayout);
	setCentralWidget(centralWidget);

	// Connections
	connect(mDEParams, &QDEEngineParameters::parameterChanged,
		this, &GPA434Lab2DEApp::changeSimulationParameters);
	connect(mDEParams, &QDEEngineParameters::parameterChanged,
		mDEController, &QDEController::resetSimulation);

	connect(mSolutionTab, &QDESolutionTab::parameterChanged,
		this, &GPA434Lab2DEApp::changeSimulationParameters);
	connect(mSolutionTab, &QDESolutionTab::parameterChanged,
		mDEController, &QDEController::resetSimulation);


	connect(mDEController, &QDEController::evolutionRequired,
		&mDE, &QDEAdapter::stepEvolution);
	connect(mDEController, &QDEController::resetRequired,
		&mDE, &QDEAdapter::reset);

	connect(mDEController, &QDEController::evolutionStarted,
		mDEParams, &QDEEngineParameters::disable);
	connect(mDEController, &QDEController::evolutionStopped,
		mDEParams, &QDEEngineParameters::enable);

	connect(mDEController, &QDEController::evolutionStarted,
		mSolutionTab, &QDESolutionTab::disable);
	connect(mDEController, &QDEController::evolutionStopped,
		mSolutionTab, &QDESolutionTab::enable);

	connect(&mDE, &QDEAdapter::stoppingCriteriaReached,
		mDEController, &QDEController::endSimulation);

	connect(&mDE, &QDEAdapter::oneEvolutionStepped,
		hisChart, &QDEHistoryChart::updateHistory);
	connect(&mDE, &QDEAdapter::oneEvolutionStepped,
		bestResult, &QDEBestResult::updateBestResult);
	connect(&mDE, &QDEAdapter::oneEvolutionStepped,
		mSolutionTab, &QDESolutionTab::updateVisualization);

	connect(&mDE, &QDEAdapter::reseted,
		mSolutionTab, &QDESolutionTab::updateVisualization);
	connect(&mDE, &QDEAdapter::reseted,
		hisChart, &QDEHistoryChart::reset);
	connect(&mDE, &QDEAdapter::reseted,
		bestResult, &QDEBestResult::reset);

	// Partie du bas
	mSolutionTab->addSolutionPanel(mPeakSol);
	mSolutionTab->addSolutionPanel(mOpenBoxSol);
	mSolutionTab->addSolutionPanel(mPcbSol);

	mDEController->resetSimulation();
	changeSimulationParameters();
}

GPA434Lab2DEApp::~GPA434Lab2DEApp()
{
}

void GPA434Lab2DEApp::changeSimulationParameters()
{
	// mDE, mDEEngineParameters et mSolutionTab indiquent les objets
	// des classes QDEAdapter, QDEEngineParameters et QDESolutionTab
	mDE.setup(mDEParams->parameters(),
		mSolutionTab->newSolution());
}