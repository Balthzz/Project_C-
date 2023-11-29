#include "QDEEngineParameters.h"


#include <QSpinBox>
#include <QComboBox>
#include <QScrollBar>
#include <QLabel>
#include <QFormLayout>

#include <EngineParameters.h>
#include <MutationRand1.h>
#include <MutationBest1.h>
#include <CrossoverBinomial.h>
#include <CrossoverExponential.h>
#include <SelectionBestFitness.h>


QDEEngineParameters::QDEEngineParameters(QWidget * parent)
	:	QGroupBox(parent),
		mPopulationSize{ new QSpinBox },
		mGenerationCount{ new QSpinBox },
		mMutationStrategy{ new QComboBox },
		mMutationFactorValue{ new QScrollBar },
		mMutationFactorLabel{ new QLabel },
		mCrossoverStrategy{ new QComboBox },
		mCrossoverRateValue{ new QScrollBar },
		mCrossoverRateLabel{ new QLabel },
		mSelectionStrategy{ new QComboBox },
		mStoppingCriteria{ new QComboBox }
{
	mPopulationSize->setRange(de::EngineParameters::minimumPopulationSize(), de::EngineParameters::maximumPopulationSize());
	mPopulationSize->setValue(de::EngineParameters::defaultPopulationSize());
	mPopulationSize->setToolTip(QString("Détermine la taille de la population [" + QString::number(de::EngineParameters::minimumPopulationSize()) + ", " + QString::number(de::EngineParameters::maximumPopulationSize()) + "]."));

	mGenerationCount->setRange(de::EngineParameters::minimumGenerationCount(), de::EngineParameters::maximumGenerationCount());
	mGenerationCount->setValue(de::EngineParameters::defaultGenerationCount());
	mGenerationCount->setToolTip(QString("Détermine le nombre maximum de génération pour la simulation [" + QString::number(de::EngineParameters::minimumGenerationCount()) + ", " + QString::number(de::EngineParameters::maximumGenerationCount()) + "]."));

	mMutationStrategy->addItem("Rand1 : aléatoire", 0);
	mMutationStrategy->addItem("Best1 : meilleur", 1);
	mMutationStrategy->setToolTip("Détermine la stratégie de mutation à utiliser pour la simulation.");

	int const labelSize{ 120 };
	mMutationFactorLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	mMutationFactorLabel->setMinimumWidth(labelSize);
	mMutationFactorValue->setOrientation(Qt::Horizontal);
	mMutationFactorValue->setRange(0, 200);
	connect(mMutationFactorValue, &QScrollBar::valueChanged, [this](int value) { mMutationFactorLabel->setText(QString("f : %1").arg((double)value / 100.0, 0, 'f', 2)); });
	connect(mMutationFactorValue, &QScrollBar::valueChanged, this, &QDEEngineParameters::parameterChanged);
	mMutationFactorValue->setValue(100);

	mCrossoverStrategy->addItem("Bin : binomial", 0);
	mCrossoverStrategy->addItem("Exp : exponentiel", 1);
	mCrossoverStrategy->setToolTip("Détermine la stratégie de croisement à utiliser pour la simulation.");

	mCrossoverRateLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	mCrossoverRateLabel->setMinimumWidth(labelSize);
	mCrossoverRateValue->setOrientation(Qt::Horizontal);
	mCrossoverRateValue->setRange(0, 100);
	connect(mCrossoverRateValue, &QScrollBar::valueChanged, [this](int value) { mCrossoverRateLabel->setText(QString("R : %1").arg((double)value / 100.0, 0, 'f', 2)); });
	connect(mCrossoverRateValue, &QScrollBar::valueChanged, this, &QDEEngineParameters::parameterChanged);
	mCrossoverRateValue->setValue(50);

	mSelectionStrategy->addItem("BestFit : meilleure 'fitness'", 0);
	mSelectionStrategy->setToolTip("Détermine la stratégie de sélection à utiliser pour la simulation.");

	mStoppingCriteria->setEnabled(false);
	mStoppingCriteria->setToolTip("...not implemented yet...");

	QFormLayout * formLayout{ new QFormLayout };
	formLayout->addRow("Taille de la population", mPopulationSize);
	formLayout->addRow("Nombre maximum de génération", mGenerationCount);
	formLayout->addRow("Stratégie de mutation", mMutationStrategy);
	formLayout->addRow(mMutationFactorLabel, mMutationFactorValue);
	formLayout->addRow("Stratégie de croisement", mCrossoverStrategy);
	formLayout->addRow(mCrossoverRateLabel, mCrossoverRateValue);
	formLayout->addRow("Stratégie de sélection", mSelectionStrategy);
	formLayout->addRow("Critères d'arrêt", mStoppingCriteria);

	setTitle("Paramètres de l'engin d'évolution différentielle");
	setLayout(formLayout);
	
	connect(mPopulationSize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &QDEEngineParameters::changeParameters);
	connect(mGenerationCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &QDEEngineParameters::changeParameters);
	connect(mMutationStrategy,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QDEEngineParameters::changeParameters);
	connect(mCrossoverStrategy, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QDEEngineParameters::changeParameters);
	connect(mSelectionStrategy, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QDEEngineParameters::changeParameters);
}

de::EngineParameters QDEEngineParameters::parameters() const
{
	de::EngineParameters param;

	param.setPopulationSize(mPopulationSize->value());
	param.setMaxGeneration(mGenerationCount->value());

	switch (mMutationStrategy->currentData().toInt()) {
		case 0: // Rand1
			param.setMutationStrategy(new de::MutationRand1);
			break;
		case 1: // Best1
			param.setMutationStrategy(new de::MutationBest1);
			break;
	}
	param.setMutationFactor(mMutationFactorValue->value() / 100.0);
	switch (mCrossoverStrategy->currentData().toInt()) {
		case 0: // Bin
			param.setCrossoverStrategy(new de::CrossoverBinomial);
			break;
		case 1: // Exp
			param.setCrossoverStrategy(new de::CrossoverExponential);
			break;
	}
	param.setCrossoverRate(mCrossoverRateValue->value() / 100.0);
	switch (mSelectionStrategy->currentData().toInt()) {
		case 0: // Best_fit
			param.setSelectionStrategy(new de::SelectionBestFitness);
			break;
	}

	return param;
}

void QDEEngineParameters::enable()
{
	setEnabled(true);
}

void QDEEngineParameters::disable()
{
	setEnabled(false);
}

void QDEEngineParameters::changeParameters()
{
	emit parameterChanged();
}


