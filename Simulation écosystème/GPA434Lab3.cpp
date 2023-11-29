#include "GPA434Lab3.h"


#include <QRandomGenerator>
#include <QGraphicsView>
#include "QControlBar.h"
#include "QParameters.h"
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>

#include "Brotonsaurus.h"
#include "DynamicEntity.h"
#include "Entity.h"
#include "Pterodactyl.h"
#include "Tree.h"
#include "Trex.h"

size_t	const GPA434Lab3::sMaxNbrOfItems{ 500 };
QSize	const GPA434Lab3::sSceneSize(1000, 600);
QColor	const GPA434Lab3::sSceneBackgroundColor(QColor::fromHsl(215, 80, 222));
QString	const GPA434Lab3::sControlStartText("Démarrer la simulation");
QString	const GPA434Lab3::sControlStopText("Terminer la simulation");
QString	const GPA434Lab3::sControlPauseText("Mettre la simulation sur pause");
QString	const GPA434Lab3::sControlResumeText("Reprendre la simulation");
QString	const GPA434Lab3::sControlStepText("Faire un pas de simulation");
QColor	const GPA434Lab3::sControlRunningColor(0, 255, 0);
QColor	const GPA434Lab3::sControlPauseColor(255, 196, 0);
QColor	const GPA434Lab3::sControlStoppedColor(255, 0, 0);
QColor	const GPA434Lab3::sControlNotReadyColor(230, 230, 230);
qreal	const GPA434Lab3::sMinLifeExpectancy{ 1.0 };
qreal	const GPA434Lab3::sMaxLifeExpectancy{ 10.0 };
qreal	const GPA434Lab3::sCenterRadius{ 25 };
qreal	const GPA434Lab3::sMinOrientationDegrees{ 0.0 };
qreal	const GPA434Lab3::sMaxOrientationDegrees{ 360.0 };
qreal	const GPA434Lab3::sMinSpeed{ 1.0 };
qreal	const GPA434Lab3::sMaxSpeed{ 5.0 };
qreal	const GPA434Lab3::sMinSize{ 7.5 };
qreal	const GPA434Lab3::sMaxSize{ 15.0 };

const QString GPA434Lab3::sAboutButtonText("À propos...");
const QString GPA434Lab3::sAboutText(
	R"..(GPA434 
Ingénierie des systèmes orientés objets

Laboratoire  3
Simulation d'un écosystème 

Ce projet consiste à simuler un écosystème de dinosaures.

Légende:
   - Carré noir: Arbre
   - Flèche verte: Brotonsaure
   - Flèche rouge: TRex
   - Flèche jaune: Ptérodactyle

Réalisé par :
   - Timothée Leclaire-Fournier
   - Pacôme Gasnier
   - Balthazar Supiot
)..");


GPA434Lab3::GPA434Lab3(QWidget* parent)
	: QMainWindow(parent)
	, mSimulationView{ new QGraphicsView(&mGraphicsScene) }
	, mParameters{ new QParameters(sMaxNbrOfItems) }
	, mControlBar{ new QControlBar(Qt::Vertical) }
	, mAboutButton{ new QPushButton(sAboutButtonText) }
{
	setWindowTitle("Startup project demo");
	setWindowIcon(QIcon(":/StartUpProjectDemo/icon"));

	ui.setupUi(this);

	mControlBar->setReady();
	mControlBar->setActionText(QControlBar::Action::Start, sControlStartText);
	mControlBar->setActionText(QControlBar::Action::Stop, sControlStopText);
	mControlBar->setActionText(QControlBar::Action::Pause, sControlPauseText);
	mControlBar->setActionText(QControlBar::Action::Resume, sControlResumeText);
	mControlBar->setActionText(QControlBar::Action::Step, sControlStepText);
	mControlBar->setStateColor(QControlBar::State::NotReady, sControlNotReadyColor);
	mControlBar->setStateColor(QControlBar::State::Paused, sControlPauseColor);
	mControlBar->setStateColor(QControlBar::State::Running, sControlRunningColor);
	mControlBar->setStateColor(QControlBar::State::Stopped, sControlStoppedColor);

	mGraphicsScene.setSceneRect(-sSceneSize.width() / 2, -sSceneSize.height() / 2, sSceneSize.width() / 2, sSceneSize.height() / 2);

	QWidget* controlWidget{ new QWidget };
	QVBoxLayout* controlLayout{ new QVBoxLayout };
	controlWidget->setLayout(controlLayout);
	controlLayout->addWidget(mParameters);
	controlLayout->addStretch();
	controlLayout->addWidget(mControlBar);
	controlLayout->addWidget(mAboutButton);

	QWidget* centralWidget{ new QWidget };
	QHBoxLayout* centralLayout{ new QHBoxLayout };
	centralWidget->setLayout(centralLayout);

	centralLayout->addWidget(mSimulationView);
	centralLayout->addWidget(controlWidget);

	setCentralWidget(centralWidget);

	connect(mControlBar, &QControlBar::started, this, &GPA434Lab3::startSimulation);
	connect(mControlBar, &QControlBar::stopped, this, &GPA434Lab3::stopSimulation);
	connect(mControlBar, &QControlBar::paused, this, &GPA434Lab3::pauseSimulation);
	connect(mControlBar, &QControlBar::resumed, this, &GPA434Lab3::resumeSimulation);
	connect(mControlBar, &QControlBar::stepped, this, &GPA434Lab3::stepSimulation);

	connect(&mTimer, &QTimer::timeout, this, &GPA434Lab3::advance);

	connect(mAboutButton, &QPushButton::clicked, this, &GPA434Lab3::about);
}

GPA434Lab3::~GPA434Lab3()
{}


void GPA434Lab3::advance()
{
	mGraphicsScene.update();
	mGraphicsScene.advance();
	// Si la conversion ne fonctionne pas, le pointeur sera nullptr
	for (auto& item : mGraphicsScene.items()) {
		DynamicEntity* dynamEnt{ dynamic_cast<DynamicEntity*>(item) };
		if (handleDynamicDeath(dynamEnt)) {
			continue;
		}

		Tree* tree{ dynamic_cast<Tree*>(item) };
		handleTreeDeath(tree);
	}
}

void GPA434Lab3::startSimulation()
{
	// Vide la scène pour démarrer une nouvelle démo
	mGraphicsScene.clear();

	// Met un item rectangulaire pour bien voir les limites de la scène
	QGraphicsRectItem* background{ new QGraphicsRectItem(mGraphicsScene.sceneRect()) };
	background->setPen(Qt::NoPen);
	background->setBrush(sSceneBackgroundColor);
	mGraphicsScene.addItem(background);

	// Ajoute les n flèches
	for (int i{ 0 }; i < mParameters->nbrOfItems(); ++i) {
		auto rand = random(0, 3.0);
		if (rand >= 2) {
			rand += 0.5;
		}
		int choice{ static_cast<int>(floor(rand)) };

		switch (choice) {
		case 0:
			mGraphicsScene.addItem(new Tree(randomPoint(-300, 0)));
			break;
		case 1:
			mGraphicsScene.addItem(new Brotonsaurus(randomPoint(-300, 0), static_cast<int>(floor(random(25, 30)))));
			break;
		case 2:
			mGraphicsScene.addItem(new TRex(randomPoint(-300, 0), static_cast<int>(floor(random(25, 30)))));
			break;
		case 3:
			mGraphicsScene.addItem(new Pterodactyl(randomPoint(-300, 0), static_cast<int>(floor(random(25, 30)))));
			break;
		}
	}

	// Démarre la simulation
	mTimer.start(30);

	mParameters->setEnabled(false);
}

void GPA434Lab3::stepSimulation()
{
	mGraphicsScene.advance();
}

void GPA434Lab3::pauseSimulation()
{
	mTimer.stop();
}

void GPA434Lab3::resumeSimulation()
{
	mTimer.start(30);
}

void GPA434Lab3::stopSimulation()
{
	mTimer.stop();
	mParameters->setEnabled(true);
}

void GPA434Lab3::about()
{
	QMessageBox::about(this, "À propos...", sAboutText);
}


double GPA434Lab3::random(qreal maxValue)
{
	return QRandomGenerator::global()->bounded(maxValue);
}

double GPA434Lab3::random(qreal minValue, qreal maxValue)
{
	return random(maxValue - minValue) + minValue;
}

QColor GPA434Lab3::randomColor()
{
	return QColor::fromRgb(QRandomGenerator::global()->generate());
}

QPointF GPA434Lab3::randomPoint(qreal min, qreal max)
{
	return QPointF(random(min, max), random(min, max));
}

bool GPA434Lab3::handleTreeDeath(Tree* tree)
{
	if (tree && (tree->getSize() <= 0)) {
		mGraphicsScene.removeItem(tree);
		delete tree;
		return true;
	}
	return false;
}

bool GPA434Lab3::handleDynamicDeath(DynamicEntity* dynam)
{
	if (dynam && !(dynam->isAlive())) {
		mGraphicsScene.addItem(new Tree(dynam->scenePos()));
		mGraphicsScene.removeItem(dynam);
		delete dynam;
		return true;
	}
	return false;
}

