#pragma once
#include "QDESolutionPanel.h"
#include <SolutionStrategy.h>

#include <QVector>
#include <QPolygon>
#include <QRadioButton>

#include <stdint.h>

class QLabel;
class QScrollBar;
class QHBoxLayout;
class QPainter;

//! 
//! \class QDEPcbSpaceOptimizer
//! 
//! \brief Cette classe représente le panneau qt qui est lié au problème d'optimisation
//! d'espace sur un pcb.
//! 
//! QDEPcbSpaceOptimizer et QDEPcbSpaceOptimizer::PcbStrategy permettent de décrire
//! un problème dans lequel nous tentons d'optimiser la grosseur d'un polygone dans
//! un pcb qui contient des pièces. 
//! 


class QDEPcbSpaceOptimizer : public QDESolutionPanel
{
	Q_OBJECT

public:
	QDEPcbSpaceOptimizer(QWidget* parent = nullptr);
	QDEPcbSpaceOptimizer(QDEPcbSpaceOptimizer const&) = default;
	QDEPcbSpaceOptimizer(QDEPcbSpaceOptimizer&&) = default;
	QDEPcbSpaceOptimizer& operator=(QDEPcbSpaceOptimizer const&) = default;
	QDEPcbSpaceOptimizer& operator=(QDEPcbSpaceOptimizer&&) = default;
	~QDEPcbSpaceOptimizer() override = default;

	QString tabTitle() const override;
	de::SolutionStrategy* buildSolution() const override;

public slots:
	void updateVisualization(QDEAdapter const& de) override;

private:
	QLabel* mVisualizationLabel;
	QScrollBar* mNumberOfObstacles;
	QScrollBar* mHeightScrollBar;
	QHBoxLayout* buildScrollBarLayout(QScrollBar*& sb, uint8_t nbObs, uint8_t nbSides);
	void handleSolution(QPolygonF mInitialPoly, size_t numberOfPoints, const QColor colorSol, 
		const QColor historyColor, double ratio, QPainter& painter, QDEAdapter const& de);

	void drawPolygon(QPainter& painter, QPolygonF& poly, QColor const& color, double ratio);
	static QPolygonF getModifiedPolygon(QPolygonF poly, float x, float y, float scale, float rotation, size_t numberOfPoints);

	class PcbStrategy;

	QVector<QPolygonF> mElements;
	QPolygonF mBoard;
	QPolygonF mInitialPolySquare;
	QPolygonF mInitialPolyTriangle;

	QRadioButton* mTriangle;
	QRadioButton* mSquare;

	static const QString sDescription;
};



class QDEPcbSpaceOptimizer::PcbStrategy : public de::SolutionStrategy
{
public:
	PcbStrategy(const QVector<QPolygonF>& elements, QPolygonF pcbBoard, QPolygonF initialPoly, int numberOfObstacles, size_t numberOfPoints);
	PcbStrategy(PcbStrategy const&) = default;
	PcbStrategy(PcbStrategy&&) = default;
	PcbStrategy& operator=(PcbStrategy const&) = default;
	PcbStrategy& operator=(PcbStrategy&&) = default;
	~PcbStrategy() override = default;

	std::string toString(de::Solution const& ag) const override;

protected:
	double process(de::Solution const& ag) override;

private:
	QVector<QPolygonF> mElements;
	QPolygonF mBoard;
	QPolygonF mInitialPoly;
	int mNumberOfObstacles;
	size_t mNumberOfPoints;
};

