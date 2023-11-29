#pragma once
#ifndef BROTONSAURUS_H
#define BROTONSAURUS_H

#include "Herbivore.h"

class Brotonsaurus : public Herbivore
{
public:
	Brotonsaurus(QPointF initialPos, double lifeExpectancy = 30, QBrush const& brush = Qt::green, double age = 0,
		double speed = 1.5, double acceleration = 3);

	void advance(int phase) override;
	void giveBirth();
};

#endif