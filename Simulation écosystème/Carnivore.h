#pragma once
#ifndef CARNIVORE_H
#define CARNIVORE_H

#include "DynamicEntity.h"
#include "Herbivore.h"

class Carnivore : public DynamicEntity
{
public:
	Carnivore(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration);
	
	void eat(Herbivore* herbivore);
	bool calculateBoundingRectToEat();
	void searchFor();
};

#endif