#pragma once
#ifndef HERBIVORE_H
#define HERBIVORE_H

#include "DynamicEntity.h"
#include "Tree.h"

class Herbivore : public DynamicEntity
{
public:
	Herbivore(QBrush const& brush, double lifeExpectancy, double age, double speed, double acceleration);

	void eaten();
	void eat(Tree* tree);
	bool calculateBoundingRectToEat();
	void searchFor();
};

#endif