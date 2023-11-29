#pragma once
#ifndef DESTATISTICS_H
#define DESTATISTICS_H

/*
	Fichier : DEStatistics.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Classe contenant la meilleure solution de chaque générations.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <vector>

#include "DESolution.h"

class DEStatistics
{
public:
	void add(DESolution const &solution);
	void reset();
	size_t size();
	DESolution last();
private:
	std::vector<DESolution> mStatistics;
};

#endif // DESTATISTICS_H