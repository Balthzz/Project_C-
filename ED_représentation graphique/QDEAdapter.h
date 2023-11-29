#pragma once


#include <QObject>
#include <DifferentialEvolution.h>
#include <EngineParameters.h>


//! 
//! \class QDEAdapter
//! 
//! \brief Cette classe sert d'interface entre l'algorithme d'évolution 
//! différentielle et le reste de l'application.
//! 
//! QDEAdapter hérite de QObject et fournit un ensemble de méthodes et de 
//! signaux pour manipuler et surveiller une instance de l'algorithme 
//! d'évolution différentielle. Elle offre des fonctionnalités pour 
//! configurer l'algorithme, réinitialiser et faire avancer l'évolution, 
//! obtenir des informations sur la solution actuelle et la population, 
//! ainsi que la vérification de l'état de l'évolution.
//! 
//! Cette classe permet d'utiliser simplement avec Qt une classe qui n'est 
//! pas conçue spécifiquement pour Qt. La librairie de est volontairement 
//! conçue pour ne pas être contrainte par les particularités de Qt. Ainsi, 
//! QDEAdapter permet le meilleur des deux mondes en acceptant le compromis 
//! d'un faible coût de performance.
//! 
class QDEAdapter : public QObject
{
	Q_OBJECT

public:
	QDEAdapter(QObject* parent = nullptr);				//!< Constructeur par défaut, prend un QObject parent en option.
	QDEAdapter(QDEAdapter const&) = default;			//!< Constructeur de copie par défaut.
	QDEAdapter(QDEAdapter&&) = default;					//!< Constructeur de déplacement par défaut.
	QDEAdapter& operator=(QDEAdapter const&) = default;	//!< Opérateur d'affectation par copie par défaut.
	QDEAdapter& operator=(QDEAdapter&&) = default;		//!< Opérateur d'affectation par déplacement par défaut.
	~QDEAdapter() override = default;					//!< Destructeur par défaut.

	bool isReady();										//!< Vérifie si l'adaptateur est prêt, c'est-à-dire, si le moteur d'évolution différentielle est prêt.
	size_t currentGeneration() const;					//!< Renvoie le numéro de la génération actuelle.
	bool isStoppingReached() const;						//!< Vérifie si le critère d'arrêt a été atteint.

	bool setup(de::EngineParameters&& engineParameters, de::SolutionStrategy* sol); //!< Configure l'adaptateur avec des paramètres de moteur et une stratégie de solution.
	de::SolutionStrategy const& solution();				//!< Renvoie la solution actuelle. Cette fonction lancera une exception si aucune solution n'est définie.

	QString bestSolutionInfo() const;					//!< Renvoie une représentation textuelle de la meilleure solution obtenue.
	de::History const& statisticsHistory() const;		//!< Renvoie l'historique des statistiques en lecture seule.

	de::Population actualPopulation() const;			//!< Renvoie une copie de la population actuelle.

public slots:
	void stepEvolution();								//!< Effectue une étape de l'évolution.
	void reset();										//!< Réinitialise la simulation à son état initial (tel que défini par EngineParameters).

signals:
	void oneEvolutionStepped(QDEAdapter const& de);		//!< Signal émis après une étape de l'évolution.
	void reseted(QDEAdapter const& de);					//!< Signal émis après la réinitialisation de l'adaptateur.
	void stoppingCriteriaReached();						//!< Signal émis lorsque le critère d'arrêt est atteint.

private:
	de::DifferentialEvolution mDE;						//!< Instance de l'algorithme d'évolution différentielle.

};

