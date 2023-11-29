#pragma once


#include <QWidget>
#include <SolutionStrategy.h>
#include "QDEAdapter.h"


//! 
//! \class QDESolutionPanel
//! 
//! \brief Classe abstraite pour créer et gérer une interface utilisateur pour 
//! la visualisation de solutions.
//! 
//! Cette classe hérite de QWidget et fournit des méthodes virtuelles pures 
//! pour la gestion des solutions.
//! 
//! La classe est conçue pour être héritée par d'autres classes qui 
//! implémentent des stratégies spécifiques de visualisation de solutions.
//! 
class QDESolutionPanel : public QWidget
{
	Q_OBJECT

public:
	QDESolutionPanel(QWidget* parent = nullptr);					//!< Constructeur par défaut, prend optionnel un widget parent.

	QDESolutionPanel(QDESolutionPanel const&) = default;			//!< Constructeur de copie par défaut.
	QDESolutionPanel(QDESolutionPanel&&) = default;					//!< Constructeur de déplacement par défaut.
	QDESolutionPanel& operator=(QDESolutionPanel const&) = default;	//!< Opérateur d'affectation par copie par défaut.
	QDESolutionPanel& operator=(QDESolutionPanel&&) = default;		//!< Opérateur d'affectation par déplacement par défaut.
	~QDESolutionPanel() override = default;							//!< Destructeur (virtuel) par défaut.

	virtual QString tabTitle() const = 0;							//!< Méthode virtuelle pure (abstraite) retournant le titre de l'onglet. Cette méthode est à implémenter dans les classes dérivées.
	virtual de::SolutionStrategy* buildSolution() const = 0;		//!< Méthode abstraite (virtuelle pure) construisant une solution. À implémenter dans les classes dérivées.

signals:
	void parameterChanged();										//!< Signal émis lorsque les paramètres changent.

public slots:
	virtual void updateVisualization(QDEAdapter const& de);			//!< Slot mettant à jour la visualisation. Reçoit un adaptateur en lecture seule pour rafraîchir la visualisation si nécessaire.
};

