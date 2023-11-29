#include "QDEController.h"


#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>


const qreal QDEController::mSlowerTimeout{ 500.0 };
const size_t QDEController::mMinimumSpeedRange{ 1 };
const size_t QDEController::mMaximumSpeedRange{ 10 };
const size_t QDEController::mDefaultSpeedRange{ 7 };


QDEController::QDEController(QDEAdapter const & dEAdapter, QWidget * parent)
	:	QGroupBox(parent),
		mDEAdapter{ dEAdapter },
		mStepButton{ new QPushButton("Un pas de simulation") },
		mResetButton{ new QPushButton("Réinitialiser la simulation") },
		mStartButton{ new QPushButton("Démarrer") },
		mSpeedSlider{ new QSlider },
		mSpeedLabel{ new QLabel(QString::number(mDefaultSpeedRange)) },
		mCurrentGenerationLabel{ new QLabel("0") }
{
	mSpeedSlider->setRange(mMinimumSpeedRange, mMaximumSpeedRange);
	mSpeedSlider->setValue(mDefaultSpeedRange);
	mSpeedSlider->setOrientation(Qt::Horizontal);
	mSpeedSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QHBoxLayout * sliderLayout{ new QHBoxLayout };
	sliderLayout->addWidget(new QLabel("Vitesse de simulation"));
	sliderLayout->addWidget(mSpeedSlider);
	sliderLayout->addWidget(mSpeedLabel);

	QFrame * separatorFrame{ new QFrame };
	separatorFrame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	separatorFrame->setLineWidth(0);
	separatorFrame->setMidLineWidth(1);

	QHBoxLayout * currentGenerationLayout{ new QHBoxLayout };
	currentGenerationLayout->addWidget(new QLabel("Génération courante : "));
	currentGenerationLayout->addWidget(mCurrentGenerationLabel);
	currentGenerationLayout->addStretch();
	mCurrentGenerationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	QVBoxLayout * boxLayout{ new QVBoxLayout };
	boxLayout->addWidget(mStepButton);
	boxLayout->addWidget(mResetButton);
	boxLayout->addWidget(separatorFrame);
	boxLayout->addWidget(mStartButton);
	boxLayout->addLayout(sliderLayout);
	boxLayout->addLayout(currentGenerationLayout);
	boxLayout->addStretch();

	setTitle("Simulation");
	setLayout(boxLayout);

	connect(mSpeedSlider, &QSlider::valueChanged, mSpeedLabel, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));
	connect(mStepButton, &QPushButton::pressed, this, &QDEController::evolutionRequired);
	connect(mResetButton, &QPushButton::pressed, this, &QDEController::resetRequired);
	connect(mResetButton, &QPushButton::pressed, this, &QDEController::resetSimulation);
	connect(mStartButton, &QPushButton::pressed, this, &QDEController::startSimulation);

	connect(&mSimulationTimer, &QTimer::timeout, this, &QDEController::evolutionRequired);
	connect(&mSimulationTimer, &QTimer::timeout, this, &QDEController::updateCurrentGeneration);

	connect(mSpeedSlider, &QSlider::valueChanged, this, &QDEController::updateSpeed);
	connect(mStepButton, &QPushButton::pressed, this, &QDEController::updateGui);
	connect(mResetButton, &QPushButton::pressed, this, &QDEController::updateGui);
	connect(mStartButton, &QPushButton::pressed, this, &QDEController::updateGui); 
}

void QDEController::updateGui()
{
	mStepButton->setEnabled(!mSimulationTimer.isActive() && !mDEAdapter.isStoppingReached());
	mResetButton->setEnabled(!mSimulationTimer.isActive());

	mStartButton->setEnabled(true);
	mSpeedSlider->setEnabled(true);

	mStartButton->setText(mSimulationTimer.isActive() ? "Arrêter" : "Démarrer");

	updateCurrentGeneration();
}

void QDEController::updateSpeed()
{
	mSimulationTimer.setInterval(getTimerValue());
}

qreal QDEController::getTimerValue()
{
	return (1.0 - static_cast<qreal>(mSpeedSlider->value()) / mSpeedSlider->maximum()) * mSlowerTimeout;
}

void QDEController::updateCurrentGeneration()
{
	mCurrentGenerationLabel->setText(QString::number(mDEAdapter.currentGeneration()));
}

void QDEController::startSimulation()
{
	if (mDEAdapter.isStoppingReached()) {
		emit resetRequired();
	}

	if (mSimulationTimer.isActive()) {
		mSimulationTimer.stop();
		emit evolutionStopped();
	} else {
		mSimulationTimer.start(getTimerValue());
		emit evolutionStarted();
	}
}

void QDEController::resetSimulation()
{
	emit resetRequired();
	updateGui();
}

void QDEController::endSimulation()
{
	mSimulationTimer.stop();
	emit evolutionStopped();
	updateGui();
}

