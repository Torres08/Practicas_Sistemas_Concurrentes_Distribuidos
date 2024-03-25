// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 1 (ejemplo1.cpp)
// Puesta en marcha de dos hebras, con finalización errónea.
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread> // libreria para declarar tipo thread
using namespace std;

// Funcion que va a ejecutar mi hebra 1
void funcion_hebra_1() {
  for (unsigned long i = 0; i < 5000; i++)
    cout << "hebra 1, i == " << i << endl << flush;
}

// funcion que va a ejecutar mi hebra 2
void funcion_hebra_2() {
  for (unsigned long i = 0; i < 5000; i++)
    cout << "                hebra 2, i == " << i << endl << flush;
}

int main() {
  thread hebra1(funcion_hebra_1), // creo mi hebra 1 que hace funcion hebra 1
      hebra2(funcion_hebra_2);    // creo mi hebra 2 que hace funcion hebra 2

  /*
      terminate called without an active exception
  zsh: IOT instruction  ./Ejercicio1
  */
}