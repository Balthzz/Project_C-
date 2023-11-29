#pragma once
#ifndef _THREE_SAMPLE_WITH_ONE_EXCLUSION_H_
#define _THREE_SAMPLE_WITH_ONE_EXCLUSION_H_


#include <vector>
#include <random>


class ThreeSamplesWithOneExclusion
{
public:
	// Les 6 fonctions membres sp�ciales du C++
	ThreeSamplesWithOneExclusion() = default;														// default constructor
	ThreeSamplesWithOneExclusion(ThreeSamplesWithOneExclusion const &) = default;					// copy constructor
	ThreeSamplesWithOneExclusion(ThreeSamplesWithOneExclusion &&) = default;						// move constructor
	ThreeSamplesWithOneExclusion& operator=(ThreeSamplesWithOneExclusion const &) = default;		// copy assignment operator
	ThreeSamplesWithOneExclusion& operator=(ThreeSamplesWithOneExclusion &&) = default;				// move assignment operator
	~ThreeSamplesWithOneExclusion() = default;														// destructor
	// Un autre constructor
	ThreeSamplesWithOneExclusion(size_t sourceSize);												// custom constructor

	// La fonction 'prepare' r�alise l'initialisation des donn�es internes. 
	// Elle doit obligatoirement �tre appel�e au moins une fois avant l'appel 
	// de la fonction 'select'. 
	// 
	// Param�tres : 
	//		- sourceSize	: la taille du vecteur � �chantillonner, >= 4
	//		- retour		: indique si la pr�paration a r�ussie
	bool prepare(size_t sourceSize);

	// R�alise l'�chantionnage. La fonction 'prepare' doit �tre appel�e au 
	// moins une fois avant l'appel de 'select'.
	//
	// Trois �chantillons sont s�lectionn�s. Les �chantillons sont 
	// diff�rents les uns des autres et de celui pass� en param�tre.
	// 
	// Param�tres :
	//		- excludeIndex	: l'index � exlure de l'�chantillonage
	//		- sample0		: l'index du premier �chantillon identifi�
	//		- sample1		: l'index du deuxi�me �chantillon identifi�
	//		- sample2		: l'index du troisi�me �chantillon identifi�
	//		- retour		: indique si la s�lection a r�ussie
	bool select(size_t exludedIndex, size_t & sample0, size_t & sample1, size_t & sample2);

private:
	std::default_random_engine mEngine;
	std::vector<size_t> mSource;
};


#endif // _THREE_SAMPLE_WITH_ONE_EXCLUSION_H_
