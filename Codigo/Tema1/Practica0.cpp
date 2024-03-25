
// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Practica 0
// Generar numero aleatorios. Tiro un dado
//
// -----------------------------------------------------------------------------

#include "scd.h"
#include <iostream>
using namespace std;
using namespace scd;

/*
    Con la plantilla aleatorio de scd. implemento un numero entero aleatorio,
    cuyo valor estara entre un minimo y un maximo, deben de ser dos constantes
    conocidas a compilar
*/

/*
    Calcula una duracion aleatoria entre 20 -200 milisegundos
    chrono::milliseconds dusacion_bloqueo_ms ( aleatorio<20,200>());

    coge la duracion anterior y espera durante un tiempo la hebra en ejecucion
    this_thread::sleep_for(duracion_bloque_ms)

*/

int Tiro_dado() {

  int num;

  num = aleatorio<0, 6>();

  return num;
}

int main() {
  const int desde = 0;
  const int hasta = 6;

  int num = aleatorio<0, 100>();

  cout << "Aleatorio del 0 - 100 : " << num << endl;

  cout << "\n";

  // Ejemplo de tirar un dado 10 veces
  cout << "Voy a hacer 10 tiradas \n" << endl;

  for (int i = 0; i <= 6; i++) {

    int tirada = Tiro_dado();
    cout << "En la tirada " << i << " ha salido un " << tirada << endl;
  }
}