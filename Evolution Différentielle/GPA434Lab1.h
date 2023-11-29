#pragma once
#ifndef GPA434LAB1_H
#define GPA434LAB1_H

/*
	Fichier : GPA434Lab1.h

	- - - - - - - - - - - - - - - - - - - - - - -
	Classe simplifiant l'initialisation du laboratoire et l'interaction
	avec l'utilisateur.
	- - - - - - - - - - - - - - - - - - - - - - -
*/

#include <string>

class GPA434Lab1
{
public:
	void run();
	void pause();
	void invalid();
	void clearScreen();
private:
	std::string response;
};

#endif // GPA434LAB1_H
