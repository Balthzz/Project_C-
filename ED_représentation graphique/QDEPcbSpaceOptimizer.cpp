#include "QDEPcbSpaceOptimizer.h"

#include <QVector>
#include <QPolygonF>
#include <QPointF>
#include <QTransform>

#include <QFrame>
#include <QGroupBox>
#include <QScrollBar>
#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPainter>

#include <algorithm>
#include <random>

#include <stdint.h>

const QString QDEPcbSpaceOptimizer::sDescription("Le problème de la boîte ouverte consiste à déterminer la taille de la coupe au quatre coins d'une surface rectangulaire permettant d'optimizer le volume du cube issu du pliage de cette feuille.");


QDEPcbSpaceOptimizer::QDEPcbSpaceOptimizer(QWidget* parent)
	: QDESolutionPanel(parent),
	mVisualizationLabel{ new QLabel },
	mNumberOfObstacles{ nullptr },
	mHeightScrollBar{ nullptr },
	mBoard{ {0.0, 0.0}, {100.0, 0.0}, {100.0, 100.0}, {0.0, 100.0}, {0.0, 0.0} },
	mInitialPolySquare{ {0.0, 0.0}, {20.0, 0.0}, {20.0, 20.0}, {0.0, 20.0}, {0.0, 0.0} },
	mInitialPolyTriangle{ {0.0, 0.0}, {10.0, 15.0}, {20.0, 0.0}, {0.0, 0.0} },
	mTriangle{ new QRadioButton },
	mSquare{ new QRadioButton }
{
	QLabel* title{ new QLabel(sDescription) };

	QFrame* sepFrame{ new QFrame };
	sepFrame->setFrameStyle(static_cast<int>(QFrame::HLine) | static_cast<int>(QFrame::Raised));

	mVisualizationLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	mVisualizationLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QFormLayout* shapeLayout{ new QFormLayout };
	shapeLayout->addRow("Triangle", mTriangle);
	shapeLayout->addRow("Carré", mSquare);

	QGroupBox* parameterGroupBox{ new QGroupBox("Paramètres") };
	QFormLayout* parameterLayout{ new QFormLayout };
	parameterLayout->addRow("Nombre d'obstacles", buildScrollBarLayout(mNumberOfObstacles, 1, 6));
	parameterLayout->addRow("Nombres de côtés", shapeLayout);
	parameterGroupBox->setLayout(parameterLayout);

	QWidget* inputWidget{ new QWidget };
	QVBoxLayout* inputLayout{ new QVBoxLayout };
	inputLayout->addWidget(parameterGroupBox);
	inputLayout->addStretch();
	inputWidget->setLayout(inputLayout);

	QGroupBox* visualizationGroupBox{ new QGroupBox("Visualisation") };
	QVBoxLayout* visualizationLayout{ new QVBoxLayout };
	visualizationLayout->addWidget(mVisualizationLabel);
	visualizationGroupBox->setLayout(visualizationLayout);

	QWidget* ioWidget{ new QWidget };
	QHBoxLayout* ioLayout{ new QHBoxLayout };
	ioLayout->addWidget(parameterGroupBox);
	ioLayout->addWidget(visualizationGroupBox);
	ioWidget->setLayout(ioLayout);

	QVBoxLayout* mainLayout{ new QVBoxLayout };
	mainLayout->addWidget(title);
	mainLayout->addWidget(sepFrame);
	mainLayout->addWidget(ioWidget);

	setLayout(mainLayout);

	mElements.append({ {5, 5}, {30, 5}, {20, 45}, {5, 5} });
	mElements.append({ {15, 50}, {15, 80}, {80, 80}, {80, 50}, {15, 50} });
	mElements.append({ {90, 5}, {70, 5}, {60, 15}, {70, 25}, {90, 25}, {90, 5} });
	mElements.append({ {35, 45}, {55, 45}, {55, 30}, {35, 45} });
	mElements.append({ {45, 5}, {45, 15}, {55, 15}, {55, 5}, {45, 5} });
	mElements.append({ {5, 90}, {5, 85}, {95, 85}, {95, 90}, {5, 90} });

	connect(mTriangle, &QRadioButton::clicked, this, &QDESolutionPanel::parameterChanged);
	connect(mSquare, &QRadioButton::clicked, this, &QDESolutionPanel::parameterChanged);

	mTriangle->setChecked(true);
}

QString QDEPcbSpaceOptimizer::tabTitle() const
{
	return QString("PCB Optimization problem");
}

de::SolutionStrategy* QDEPcbSpaceOptimizer::buildSolution() const
{
	if (mTriangle->isChecked())
		return new PcbStrategy(mElements, mBoard, mInitialPolyTriangle, mNumberOfObstacles->value(), 4);
	else
		return new PcbStrategy(mElements, mBoard, mInitialPolySquare, mNumberOfObstacles->value(), 5);
}

QHBoxLayout* QDEPcbSpaceOptimizer::buildScrollBarLayout(QScrollBar*& sb, uint8_t minSize, uint8_t maxSize)
{
	const int defSize{ 5 };
	const int minWidth{ 150 };

	sb = new QScrollBar;
	sb->setOrientation(Qt::Horizontal);
	sb->setRange(minSize, maxSize);
	sb->setValue(5);
	sb->setMinimumWidth(minWidth);

	QLabel* label{ new QLabel(QString::number(defSize)) };

	connect(sb, &QScrollBar::valueChanged, label, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));
	connect(sb, &QScrollBar::valueChanged, this, &QDESolutionPanel::parameterChanged);

	QHBoxLayout* layout{ new QHBoxLayout };
	layout->addWidget(sb);
	layout->addWidget(label);

	return layout;
}

void QDEPcbSpaceOptimizer::updateVisualization(QDEAdapter const& de)
{
	static const QColor pcbColor(211, 150, 200);
	static const QColor elementsColor(100, 100, 100);
	static const QColor solutionColor(200, 50, 156);
	static const QColor historyColor(192, 192, 192);
	static const QColor backgroundColor(255, 255, 255);

	QSize labelImageSize(mVisualizationLabel->size().width() - 1, mVisualizationLabel->size().height() - 1);

	double ratioW{ static_cast<double>(labelImageSize.width()) / mBoard.boundingRect().width() };
	double ratioH{ static_cast<double>(labelImageSize.height()) / mBoard.boundingRect().height() };
	double ratio{ std::min(ratioW, ratioH) };

	QPixmap pixmap(labelImageSize);
	pixmap.fill(backgroundColor);

	QPainter painter(&pixmap);

	// Le PCB
	drawPolygon(painter, mBoard, pcbColor, ratio);

	// Les autres parties du PCB
	for (size_t i{ 0 }; i < mNumberOfObstacles->value(); ++i) {
		drawPolygon(painter, mElements[i], elementsColor, ratio);
	}

	// Les meilleures solutions
	if (de.statisticsHistory().size()) {
		if (mSquare->isChecked()) {
			handleSolution(mInitialPolySquare, 5, solutionColor, historyColor, ratio, painter, de);
		}
		else {
			handleSolution(mInitialPolyTriangle, 4, solutionColor, historyColor, ratio, painter, de);
		}
	}


	mVisualizationLabel->setPixmap(pixmap);
}

void QDEPcbSpaceOptimizer::handleSolution(QPolygonF mInitialPoly, size_t numberOfPoints, const QColor colorSol,
	const QColor historyColor, double ratio, QPainter& painter, QDEAdapter const& de)
{
	// 10 solutions les plus récentes
	for (int i = de.statisticsHistory().size() - 2; i >= 0 && i >= de.statisticsHistory().size() - 11; i--) {

		const auto& sol{ de.statisticsHistory()[i].bestSolution() };

		QPolygonF solutionPolygon{ QDEPcbSpaceOptimizer::getModifiedPolygon(mInitialPoly, sol[0], sol[1], sol[2], sol[3], numberOfPoints) };
		drawPolygon(painter, solutionPolygon, historyColor, ratio);
	}

	// Solution la plus récente, doit être par dessus
	const auto& sol{ de.statisticsHistory().last().bestSolution() };
	QPolygonF solutionPolygon{ QDEPcbSpaceOptimizer::getModifiedPolygon(mInitialPoly, sol[0], sol[1], sol[2], sol[3], numberOfPoints) };
	drawPolygon(painter, solutionPolygon, colorSol, ratio);
}


void QDEPcbSpaceOptimizer::drawPolygon(QPainter& painter, QPolygonF& poly, QColor const& color, double ratio)
{
	QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter.setPen(pen);
	painter.setBrush(color);
	QPolygonF final = QTransform().scale(ratio, ratio).map(poly);
	painter.drawPolygon(final);
}

QPolygonF QDEPcbSpaceOptimizer::getModifiedPolygon(QPolygonF poly, float x, float y, float scale, float rotation, size_t numberOfPoints)
{
	for (size_t i{ 0 }; i < numberOfPoints; i++) {
		poly[i].setX(poly[i].x() + x);
		poly[i].setY(poly[i].y() + y);
	}
	poly = QTransform().scale(scale, scale).map(poly);
	QPointF center{ poly.boundingRect().center() };
	poly = QTransform().translate(center.x(), center.y()).rotate(rotation).translate(-center.x(), -center.y()).map(poly);
	return poly;
}




#include <OptimizationMaximization.h>
#include <FitnessIdentity.h>
#include <sstream>
#include <iomanip>

QDEPcbSpaceOptimizer::PcbStrategy::PcbStrategy(const QVector<QPolygonF>& elements, QPolygonF pcbBoard, QPolygonF initialPoly,
	int numberOfObstacles, size_t numberOfPoints)
	: mBoard{ pcbBoard }, mElements{ elements }, mInitialPoly{ initialPoly }, mNumberOfObstacles{ numberOfObstacles },
	mNumberOfPoints{ numberOfPoints }
{
	// Problème à quatre dimensions: x, y, scale, rotation
	mSolutionBounds.resize(4);
	mSolutionBounds[0].set(0.0, pcbBoard.boundingRect().width() - mInitialPoly.boundingRect().width());
	mSolutionBounds[1].set(0.0, pcbBoard.boundingRect().height() - mInitialPoly.boundingRect().height());
	mSolutionBounds[2].set(0.0, 25.0);
	mSolutionBounds[3].set(0.0, 360.0);
	setOptimizationStrategy(new de::OptimizationMaximization);
	setFitnessStrategy(new de::FitnessIdentity);
}

std::string QDEPcbSpaceOptimizer::PcbStrategy::toString(de::Solution const& ag) const
{
	std::stringstream sStream;
	sStream << "Translation x: " << ag[0] << "\nTranslation y: " << ag[1]
		<< "\nScale: " << ag[2] << "\nRotation: " << ag[3]
		<< "\nFitness: " << ag.fitness();

	return sStream.str();
}

double QDEPcbSpaceOptimizer::PcbStrategy::process(de::Solution const& ag)
{
	// On construit le polygone qui représente la solution.
	QPolygonF solutionPolygon;
	if (mNumberOfPoints == 5)
		solutionPolygon = QDEPcbSpaceOptimizer::getModifiedPolygon(mInitialPoly, ag[0], ag[1], ag[2], ag[3], 5);
	else
		solutionPolygon = QDEPcbSpaceOptimizer::getModifiedPolygon(mInitialPoly, ag[0], ag[1], ag[2], ag[3], 4);

	QPolygon r = solutionPolygon.toPolygon();

	// TODO: Extraire diff entre points et rect et prendre le point le plus loin
	for (int i{ 0 }; i < 4; i++)
		if (r.point(i).x() < 0 || r.point(i).x() > mBoard.boundingRect().width() ||
			r.point(i).y() < 0 || r.point(i).y() > mBoard.boundingRect().height())
			return -std::abs(ag[2] * 1000.0);

	for (size_t i{}; i < mNumberOfObstacles; i++) {
		if (solutionPolygon.intersects(mElements[i])) {
			auto fullPoly = solutionPolygon.intersected(mElements[i]);
			return -(fullPoly.boundingRect().width() * fullPoly.boundingRect().height());
		}
	}

	return ag[2] * 1000.0;
}

