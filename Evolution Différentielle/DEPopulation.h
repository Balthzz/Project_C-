#pragma once
#ifndef DEPOPULATION_H
#define DEPOPULATION_H

/*
	Fichier : DEPopulation.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Population représentant une génération de l'algorithme.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <vector>

#include "DESolution.h"
#include "DESolutionBounds.h"

class DEPopulation
{
public:
	void setup(size_t populationSize, DESolutionBounds solutionBounds);
	void resizeBounds(size_t populationSize);
	void randomize(DESolutionBounds& solutionBounds);
	std::vector<DESolution>& solutions();
	size_t size() const;

	DESolution const& operator[](int) const;
	DESolution& operator[](int);
	DESolution const& operator[](size_t) const;
	DESolution& operator[](size_t);

private:
	std::vector<DESolution> mSolutions;
};

#endif // DEPOPULATION_H