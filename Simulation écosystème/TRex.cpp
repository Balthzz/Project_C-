#include <QPainter>
#include <QRandomGenerator>

#include "Carnivore.h"
#include "TRex.h"
#include "Warper.h"

TRex::TRex(QPointF initialPos, double lifeExpectancy, QBrush const& brush,
	double age, double speed, double acceleration, uint8_t mStarvationLevelEat)
	: Carnivore(brush, lifeExpectancy, age, speed, acceleration)
{
	setPos(initialPos);
	mShape << QPointF(0, 0)
		<< QPointF(-0.25, 0.5)
		<< QPointF(1, 0)
		<< QPointF(-0.25, -0.5);
	setScale(7);
}

void TRex::advance(int phase)
{
	mCountForStarvationLevel++;

	if (mCountForStarvationLevel >= mMaxCountForStarvationLevel) {
		mStarvationLevel++;
		mCountForStarvationLevel = 0;
	}


	// TODO algo plus complexe
	if (phase == 0) {
		if (mStarvationLevel > 0) {
			if (!calculateBoundingRectToEat()) {
				searchFor();

				if (!mFoundFood) {
					randomMovement();
				}
			}
		}
		else {
			if (mAge >= 12 && !mGivenBirth) { // Age pour donner naissance 
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

void TRex::giveBirth()
{
	scene()->addItem(new TRex(scenePos()));
}