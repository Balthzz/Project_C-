#include "Carnivore.h"
#include "Warper.h"

Carnivore::Carnivore(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration)
	: DynamicEntity(brush, lifeExpectancy, age, speed, acceleration)
{
}

void Carnivore::eat(Herbivore* herbivore)
{
	if (herbivore) {
		mStarvationLevel = 0; // Carnivore vient de manger, alors il est plein
		mCountForStarvationLevel = 0;
		mLifeExpectancy += 0.5; // Gagne de la vie

		herbivore->eaten();
	}
}

bool Carnivore::calculateBoundingRectToEat()
{
	if (mStarvationLevel > 0) {  
		auto* scnPtr = scene();

		if (scnPtr) {
			for (auto& itemTrees : scnPtr->items()) {
				Herbivore* herbToEat{ dynamic_cast<Herbivore*>(itemTrees) };

				if (herbToEat && herbToEat->isAlive()
					&& sceneBoundingRect().intersects(herbToEat->sceneBoundingRect())) {
					eat(herbToEat);
					return true;
				}
			}
		}
	}
	return false;
}

void Carnivore::searchFor()
{
	mFoundFood = false;

	auto center = sceneBoundingRect().center();
	QPolygonF p;
	p << QPointF(center.x() - 15, center.y() - 15)
		<< QPointF(center.x() + 15, center.y() - 15)
		<< QPointF(center.x() + 15, center.y() + 15)
		<< QPointF(center.x() - 15, center.y() + 15);

	for (auto& i : scene()->items()) {
		auto herb = dynamic_cast<Herbivore*>(i);
		if (herb && p.boundingRect().intersects(herb->sceneBoundingRect())) {
			auto i = herb->sceneBoundingRect();
			auto d = p.boundingRect();
			mFoundFood = true;

			auto posDiff = scenePos() - herb->scenePos();
			double degDiff = qAtan2(posDiff.x(), posDiff.y());

			mNewOrientation = rotation() + degDiff;
			qreal newOrientationRadians{ qDegreesToRadians(mNewOrientation) };

			mNewPosition = pos() + QPointF(qCos(newOrientationRadians), qSin(newOrientationRadians)) * mSpeed;
			warper::warpP(mNewPosition, scene());
			break;
		}
	}
}