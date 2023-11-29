#pragma once
#ifndef DESOLUTION_H
#define DESOLUTION_H

/*
	Fichier : DESolution.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe représentant une solution s de l'alorithme.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <vector>

#include "DESolutionBounds.h"

class DESolution
{
public:
	size_t size() const;
	void setup(DESolutionBounds const& solutionBounds);
	void resizeBounds(size_t size);
	void randomize(DESolutionBounds& solutionBounds);
	std::vector<double>& data();
	std::vector<double> data() const;
	void setObjective(double obj);
	void setFitness(double fit);
	double Objective();
	double Fitness();

	double const& operator[](int) const;
	double& operator[](int);

private:
	std::vector<double> mData;
	double mObjective;
	double mFitness;
};

#endif // DESOLUTION_H