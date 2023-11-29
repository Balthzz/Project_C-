#ifdef _WIN32
#include <conio.h>		// _getch()
#else
#include <termios.h>	// Implémentation custom de _getch()
#include <stdio.h>
#endif

#include <iostream>
#include <string>

#include "GPA434Lab1.h"
#include "OpenBoxSolverInterface.h"
#include "PeaksSolverInterface.h"
#include "FactoryProductionSolverInterface.h"

void GPA434Lab1::run()
{
	std::string str("Bienvenue dans le programme de laboratoire #1 pour le cours de GPA434.\n");
	str.append("NOTE: Puisque le terminal Windows ne supporte pas les accents, ils ont ete retires du texte.\n");
	str.append("Ce programme est un algorithme d'evolution differentielle afin de resoudre trois probleme d'optimisation.\n");
	str.append("Programme realise par Timothee Leclaire-Fournier, Balthazar Supiot et Pacome Gasnier.\n\n");
	std::cout << str;

	pause();

	OpenBoxSolverInterface oBSI;
	PeaksSolverInterface pSI;
	FactoryProductionSolverInterface fPSI;

	while (true) {
		clearScreen();

		// Présenter les problèmes
		std::cout << oBSI.problemPresentation() << "\n";
		std::cout << pSI.problemPresentation() << "\n";
		std::cout << fPSI.problemPresentation() << "\n";

		std::cout << "===================================================================================================\n";
		std::cout << "Quel programme voulez-vous? (1 = boite, 2 = fonction Peaks, 3 = Optimisation d'usine, q = quitter): ";

		std::getline(std::cin, response);
		if (response == "q")
			break;

		if (!response.empty()) {
			int responseToInt{};

			// On ne veut pas lancer une exception quand on se fait passer
			// une valeur pas bonne. On le mentionne simplement à l'utilisateur.
			try {
				responseToInt = std::stoi(response);
			}
			catch (...) {
				invalid();
				continue;
			}

			switch (responseToInt) {
			case 1:
				oBSI.solve();
				pause();
				break;
			case 2:
				pSI.solve();
				pause();
				break;
			case 3:
				fPSI.solve();
				pause();
				break;
			default:
				invalid();
				break;
			}
		}
	}
}

void GPA434Lab1::pause()
{
	std::cout << "Appuyez sur une touche pour continuer...";

#ifdef _WIN32
	(void)_getch(); // On vient chercher 1 caractère et on ignore la valeur de retour.
#else 
	// Si ce n'est pas Windows, on assume POSIX.
	// https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html
	// https://pubs.opengroup.org/onlinepubs/7908799/xsh/tcgetattr.html
	// https://pubs.opengroup.org/onlinepubs/7908799/xsh/tcsetattr.html

	struct termios infoOriginal;						// Structs qui vont contenir les infos de terminal courant.
	struct termios infoModified;

	tcgetattr(STDIN_FILENO, &infoOriginal);				// On garde une copie originale pour le restaurer suite à getchar().
	tcgetattr(STDIN_FILENO, &infoModified);				// On veut modifier stdin. D'habitude c'est 0 comme fd, mais on
														// veut être certain, alors on prend la constante définie par POSIX.

	infoModified.c_lflag &= ~ICANON;					// On désactive le mode canonique (buffer ligne par ligne).
	infoModified.c_lflag &= ~ECHO;						// On désactive le mode echo (on ne veut pas voir le char à l'écran).
	infoModified.c_cc[VMIN] = 1;						// Limite de 1 caractère.
	infoModified.c_cc[VTIME] = 0;						// On désactive le timeout.
	tcsetattr(STDIN_FILENO, TCSANOW, &infoModified);	// On modifie le terminal pour mettre les changements.
														// TCSANOW -> Mettre changements maintenant sans flusher.

	(void)getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &infoOriginal);	// On remet le mode original.
#endif
}

void GPA434Lab1::invalid() 
{
	std::cout << "Vous n'avez pas rentre une commande valide. Veuillez reessayer.\n";
	pause();
}

void GPA434Lab1::clearScreen()
{
	// NOTE: Windows 10 TH2 et plus récent seulement (mais fonctionne sur *nix);
	//
	// On utilise pas system("cls") pour des raisons de sécurité et de performance.
	// Un programme pourrait se renommer "cls" et être executé avec tous les privilèges
	// de l'utilisateur.
	std::cout << "\033c";
}