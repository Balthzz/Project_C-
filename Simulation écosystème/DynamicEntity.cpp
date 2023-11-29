#include "DynamicEntity.h"
#include "Tree.h"
#include "Warper.h"

#include <qgraphicsscene.h>
#include <qrandom.h>

DynamicEntity::DynamicEntity(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration)
	: Entity(brush), mLifeExpectancy{ lifeExpectancy }, mAge{ age }, mSpeed{ speed },
	mAcceleration{ acceleration }, mNewOrientation{ 0 }, mStarvationLevel{ 0 },
	mStarvationLevelMax{ 8 }, mCountForStarvationLevel{ 0 }, mMaxCountForStarvationLevel{ 12 * 30 }, mFoundFood{ true }, mGivenBirth{ false }
{
}

bool DynamicEntity::isAlive()
{
	return mLifeExpectancy > mAge || mStarvationLevel >= mStarvationLevelMax;
}

void DynamicEntity::createManure()
{
	scene()->addItem(new Tree(scenePos()));
}

void DynamicEntity::randomMovement()
{
	static qreal const maxDeltaOrientation{ 12.5 }; // Deg
	mNewOrientation = rotation() + QRandomGenerator::global()->bounded(2.0 * maxDeltaOrientation) - maxDeltaOrientation;
	qreal newOrientationRadians{ qDegreesToRadians(mNewOrientation) };
	mNewPosition = pos() + QPointF(qCos(newOrientationRadians), qSin(newOrientationRadians)) * mSpeed;
	warper::warpP(mNewPosition, scene());
}