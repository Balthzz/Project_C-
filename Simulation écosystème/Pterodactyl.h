#pragma once
#ifndef PTERODACTYL_H
#define PTERODACTYL_H

#include "Carnivore.h"

class Pterodactyl : public Carnivore
{
public:
	Pterodactyl(QPointF initialPos, double lifeExpectancy = 25, QBrush const& brush = Qt::yellow,
		double age = 0, double speed = 2, double acceleration = 2, uint8_t mStarvationLevelEat = 1);

	void advance(int phase) override;
	void giveBirth();
};

#endif