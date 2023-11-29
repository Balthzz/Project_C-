#pragma once
#ifndef DESOLUTIONBOUNDS_H
#define DESOLUTIONBOUNDS_H

/*
	Fichier : DESolutionBounds.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant les limites des solutions possibles.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <vector>

#include "Interval.h"

class DESolutionBounds
{
public:
	DESolutionBounds(std::vector<Interval> dimensions);
	size_t size() const;
	double getAtPositionRandom(unsigned int position);
	std::vector<Interval>& getBounds();

private:
	std::vector<Interval> mSolutionBounds;
};

#endif // DESOLUTIONBOUNDS_H
