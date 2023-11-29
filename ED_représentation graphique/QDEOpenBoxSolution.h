#pragma once
#include "QDESolutionPanel.h"
#include <SolutionStrategy.h>

// Le problème de la boîte ouverte.
// 
// Vous avez une surface rectangulaire d'une largeur et d'une hauteur déterminée (width x height).
//
//    +--------------------------------------+     -+
//    |                                      |      |
//    |                                      |      |
//    |                                      |      |
//    |                                      |      |
//    |                                      |      \_ height
//    |                                      |      /
//    |                                      |      |
//    |                                      |      |
//    |                                      |      |
//    |                                      |      |
//    +--------------------------------------+     -+
//
//    +------------------v-------------------+
//                       |
//                     width
// 
// On vous demande de couper les 4 coins selon un carré de taille : x
//
//    +-------+----------------------+-------+     -+
//    |       |                      |       |      |
//    |       |                      |       |      >- x
//    |       |                      |       |      |
//    +-------+                      +-------+     -+
//    |                                      |
//    |                                      |
//    +-------+                      +-------+     -+
//    |       |                      |       |      |
//    |       |                      |       |      >- x
//    |       |                      |       |      |
//    +-------+----------------------+-------+     -+
//
//    +---v---+                      +---v---+ 
//        |							     |
//        x							     x
//
// Pour que finalement, après avoir plié les 4 panneaux, vous otenez la boîte ouverte ayant le plus grand volume possible.
//       
//                                       ________ 
//                       ________--------       |-_  
//       ________--------                       |  -_
//      |-_                                     |    -_
//      |  -_                                   |________
//      |    -_                  ________--------       |
//      |      - ________--------						|
//      |       |                                       |
//      -_      |                                       |
//        -_    |                               ________|
//          -_  |                ________--------
//            -_|________--------
//
//

class QLabel;
class QScrollBar;
class QHBoxLayout;
class QPainter;


class QDEOpenBoxSolution : public QDESolutionPanel
{
	Q_OBJECT

public:
	QDEOpenBoxSolution(QWidget * parent = nullptr);
	QDEOpenBoxSolution(QDEOpenBoxSolution const &) = default;
	QDEOpenBoxSolution(QDEOpenBoxSolution &&) = default;
	QDEOpenBoxSolution& operator=(QDEOpenBoxSolution const &) = default;
	QDEOpenBoxSolution& operator=(QDEOpenBoxSolution &&) = default;
	~QDEOpenBoxSolution() override = default;

	QString tabTitle() const override;
	de::SolutionStrategy* buildSolution() const override;

public slots:
	void updateVisualization(QDEAdapter const & de) override;

private:
	QLabel * mVisualizationLabel;
	QScrollBar * mWidthScrollBar;
	QScrollBar * mHeightScrollBar;
	QHBoxLayout * buildScrollBarLayout(QScrollBar * & sb);
	void drawBox(QPainter & painter, QPointF const & boxCenterPos, double boxSize, double cutSize, QColor const & boxColor, QColor const & cornerColor);

	class OpenBoxStrategy;

	static const QString sDescription;
};



class QDEOpenBoxSolution::OpenBoxStrategy : public de::SolutionStrategy
{
public:
	OpenBoxStrategy(double width = 1.0, double height = 1.0);
	OpenBoxStrategy(OpenBoxStrategy const &) = default;
	OpenBoxStrategy(OpenBoxStrategy &&) = default;
	OpenBoxStrategy& operator=(OpenBoxStrategy const &) = default;
	OpenBoxStrategy& operator=(OpenBoxStrategy &&) = default;
	~OpenBoxStrategy() override = default;

	std::string toString(de::Solution const & ag) const override;

protected:
	double process(de::Solution const & ag) override;

private:
	double mWidth;
	double mHeight;
};

