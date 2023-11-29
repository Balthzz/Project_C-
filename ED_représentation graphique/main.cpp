#include "GPA434Lab2DEApp.h"
#include <QtWidgets/QApplication>


// Si on désire exécuter la fonction démo de delib, il suffit de retirer 
// les commentaires des lignes identifiée avec un commentaire   // démo delib
//#include "delibBasicDemo.cpp"             // démo delib

int main(int argc, char *argv[])
{
    //delibBasicDemo();                     // démo delib

    QApplication a(argc, argv);
    GPA434Lab2DEApp w;
    w.show();
    return a.exec();
}


