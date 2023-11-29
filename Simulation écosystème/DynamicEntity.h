#pragma once
#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <stdint.h>
#include "Entity.h"

class DynamicEntity : public Entity
{
public:
	DynamicEntity(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration);

	double mLifeExpectancy;
	double mAge;
	double mSpeed;
	double mAcceleration;
	uint8_t mStarvationLevel;
	uint8_t mStarvationLevelMax;
	uint8_t mStarvationLevelEat;
	int mCountForStarvationLevel, mMaxCountForStarvationLevel;
	qreal mNewOrientation;
	QPointF mNewPosition;
	bool mFoundFood;
	bool mGivenBirth;

	bool isAlive();
	void createManure();
	void randomMovement();
};

#endif