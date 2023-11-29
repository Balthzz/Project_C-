#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*
	Fichier : Définitions.h
	- - - - - - - - - - - - - - - - - - - - - - -
	Définitions de différents raccourcis.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include "DESolution.h"

using de_objective_function_t = double(*)(DESolution const&);
using de_fitness_function_t = double(*)(double);

#endif // DEFINITIONS_H
