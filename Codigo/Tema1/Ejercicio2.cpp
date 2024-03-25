// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 2 (ejemplo2.cpp)
// Puesta en marcha de dos hebras, con finalización correcta.
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread>
using namespace std;

// Funcion que va a ejecutar mi hebra 1
void funcion_hebra_1() {
  for (unsigned long i = 0; i < 5; i++)
    cout << "hebra 1, i == " << i << endl << flush;
}

// funcion que va a ejecutar mi hebra 2
void funcion_hebra_2() {
  for (unsigned long i = 0; i < 5; i++)
    cout << "                hebra 2, i == " << i << endl << flush;
}

int main() {

  thread hebra1(funcion_hebra_1), // creo mi hebra 1 que hace funcion hebra 1
      hebra2(funcion_hebra_2);    // creo mi hebra 2 que hace funcion hebra 2

  // ambas hace un contador de 50000

  hebra1.join(); // la hebra principal espera a que hebra 1 termine
  hebra2.join(); // la hebra principal main espera a que hebra 2 termine
}