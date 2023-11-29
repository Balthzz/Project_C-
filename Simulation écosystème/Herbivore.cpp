#include "Herbivore.h"
#include "Warper.h"

#include <qgraphicsscene.h>

Herbivore::Herbivore(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration)
	: DynamicEntity(brush, lifeExpectancy, age, speed, acceleration)
{
}

void Herbivore::eaten()
{
	mLifeExpectancy = 0;	// La prochaine boucle principale va retirer cet herbivore, puisque
							// mAge >= mLifeExpectancy
	createManure();
}

void Herbivore::eat(Tree* tree)
{
	if (tree) {
		mStarvationLevel = 0; // Herbivore vient de manger, alors il est plein
		mCountForStarvationLevel = 0;

		tree->eaten();
	}
}

bool Herbivore::calculateBoundingRectToEat()
{
	if (mStarvationLevel > 0) {
		auto* scnPtr = scene();

		if (scnPtr) {
			for (auto& itemTrees : scnPtr->items()) {
				Tree* treeToEat{ dynamic_cast<Tree*>(itemTrees) };

				if (treeToEat && treeToEat->getSize() >= 0
					&& sceneBoundingRect().intersects(treeToEat->sceneBoundingRect())) {
					eat(treeToEat);
					return true;
				}
			}
		}
	}
	return false;
}

void Herbivore::searchFor()
{
	mFoundFood = false;

	auto center = sceneBoundingRect().center();
	QPolygonF p;
	p << QPointF(center.x() - 15, center.y() - 15)
		<< QPointF(center.x() + 15, center.y() - 15)
		<< QPointF(center.x() + 15, center.y() + 15)
		<< QPointF(center.x() - 15, center.y() + 15);

	for (auto& i : scene()->items()) {
		auto tree = dynamic_cast<Tree*>(i);
		if (tree && p.boundingRect().intersects(tree->sceneBoundingRect())) {
			auto i = tree->sceneBoundingRect();
			auto d = p.boundingRect();
			mFoundFood = true;

			auto posDiff = scenePos() - tree->scenePos();
			double degDiff = qAtan2(posDiff.x(), posDiff.y());

			mNewOrientation = rotation() + degDiff;
			qreal newOrientationRadians{ qDegreesToRadians(mNewOrientation) };

			mNewPosition = pos() + QPointF(qCos(newOrientationRadians), qSin(newOrientationRadians)) * mSpeed;
			warper::warpP(mNewPosition, scene());
			break;
		}
	}
}