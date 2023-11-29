#pragma once
#ifndef GPA434_LAB2_DE_APP_H
#define GPA434_LAB2_DE_APP_H


#include <QtWidgets/QMainWindow>


#include "QDEAdapter.h"
#include "QDEController.h"
#include "QDEEngineParameters.h"
#include "QDEPeakSolution.h"
#include "QDEOpenBoxSolution.h"
#include "QDEPcbSpaceOptimizer.h"
#include "QDESolutionTab.h"


// Classe de l'application principale.
class GPA434Lab2DEApp : public QMainWindow
{
	Q_OBJECT

public:
	GPA434Lab2DEApp(QWidget* parent = nullptr);
	~GPA434Lab2DEApp();

private slots:

	// Tel que stipulé dans l'énoncé, cette slot est nécessaire à la 
	// réalisation de ce laboratoire.
	void changeSimulationParameters();

private:

	// Tel que stipulé dans l'énoncé, cet objet encapsule l'objet de 
	// l'évolution différentielle : de::DifferentialEvolution.
	QDEAdapter mDE;
	QDEEngineParameters* mDEParams;
	QDEController* mDEController;
	QDESolutionTab* mSolutionTab;

	QDEOpenBoxSolution* mOpenBoxSol;
	QDEPeakSolution* mPeakSol;
	QDEPcbSpaceOptimizer* mPcbSol;
};


#endif // GPA434_LAB2_DE_APP_H