#pragma once
#ifndef TREX_H
#define TREX_H

#include "Carnivore.h"

class TRex : public Carnivore
{
public:
	TRex(QPointF initialPos, double lifeExpectancy = 20, QBrush const& brush = Qt::red,
		double age = 0, double speed = 1, double acceleration = 2, uint8_t mStarvationLevelEat = 2);

	void advance(int phase) override;
	void giveBirth();
};

#endif