#include <QPainter>
#include <QRandomGenerator>

#include "Warper.h"
#include "Pterodactyl.h"

Pterodactyl::Pterodactyl(QPointF initialPos, double lifeExpectancy, QBrush const& brush,
	double age, double speed, double acceleration, uint8_t mStarvationLevelEat)
	: Carnivore(brush, lifeExpectancy, age, speed, acceleration)
{
	setPos(initialPos);
	mShape << QPointF(0, 0)
		<< QPointF(-0.25, 0.5)
		<< QPointF(1, 0)
		<< QPointF(-0.25, -0.5);
	setScale(9);
}

void Pterodactyl::advance(int phase)
{
	mCountForStarvationLevel++;

	if (mCountForStarvationLevel >= mMaxCountForStarvationLevel) {
		mStarvationLevel++;
		mCountForStarvationLevel = 0;
	}

	// TODO algo plus complexe
	if (phase == 0) {
		if (mStarvationLevel > mStarvationLevelEat) { // Niveau de faim minimum pour pouvoir manger
			if (!calculateBoundingRectToEat()) {
				searchFor();

				if (!mFoundFood) {
					randomMovement();
				}
			}
		}
		else {
			if (mAge >= 15 && !mGivenBirth) { // Age pour donner vie 
				giveBirth();
				mGivenBirth = true;
			}

			// Aléatoire
			randomMovement();
		}
	}
	else if (phase == 1) {
		setRotation(mNewOrientation);
		setPos(mNewPosition);

		mAge += 0.030;
	}
}

void Pterodactyl::giveBirth()
{
	scene()->addItem(new Pterodactyl(scenePos()));
}