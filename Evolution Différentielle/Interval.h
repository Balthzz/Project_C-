#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H

/*
	Fichier : Interval.h
	
	- - - - - - - - - - - - - - - - - - - - - - -
	Intervalle représentant un interval mathématique.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <random>

class Interval
{
public:
	Interval();
	Interval(double lower, double upper);

	double randomize();

	void setLower(double lower);
	void setUpper(double upper);

	double getLower();
	double getUpper();

	void set(double lower, double upper);

private:
	double mLower;
	double mUpper;


	// Voir cppreference -> std::uniform_real_distribution
	// https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
	// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	// 
	// On �vite de cr�er un seed et un générateur à chaque appel 
	// pour ne pas détruire la performance (5 Kb sur le stack).

	static std::mt19937 mEngine;
	std::uniform_real_distribution<> distance;
};

#endif // INTERVAL_H