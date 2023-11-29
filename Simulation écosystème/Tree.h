#pragma once
#ifndef TREE_H
#define TREE_H

#include "Entity.h"

class Tree : public Entity
{
public:
	Tree(QPointF initialPos, QBrush const& brush = Qt::black);

	int mSize; // permet de déterminer combien de fois l'arbre peut être mangé
	int mMaxSize;
	double mSizePx;
	int mCountBeforeGrowth, mCurrentCount; // Ticks pour savoir quand grandir
	void disperseSeeds(); // Disperse des graînes sur la grille qui donneront d'autres arbres
	void grow(); // L'arbre grandit de façon régulière
	void calculateSizePx();
	void updateShape();

	void advance(int phase) override;
	void eaten();
	int getSize();
};

#endif