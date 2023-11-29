#pragma once
#ifndef INTERVALWHOLENUMBERS_H
#define INTERVALWHOLENUMBERS_H

/*
	Fichier : IntervalWholeNumbers.h
	
	- - - - - - - - - - - - - - - - - - - - - - -
	Intervalle représentant un interval mathématique de nombres
	entiers.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <random>

class IntervalWholeNumbers
{
public:
	IntervalWholeNumbers();
	IntervalWholeNumbers(int lower, int upper);

	int randomize();

	void setLower(int lower);
	void setUpper(int upper);

	double getLower();
	double getUpper();

	void set(int lower, int upper);

private:
	int mLower;
	int mUpper;

	// Voir cppreference -> std::uniform_int_distribution
	// https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
	// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
	// 
	// On �vite de cr�er un seed et un gén�rateur à chaque appel 
	// pour ne pas détruire la performance (5 Kb sur le stack).

	static std::mt19937 mEngine;
	std::uniform_int_distribution<> distance;
};

#endif // INTERVALWHOLENUMBERS_H