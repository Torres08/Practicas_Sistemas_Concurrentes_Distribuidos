// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 3 (ejemplo3.cpp)
// Obtención de resultados mediante variables globales
//
// -----------------------------------------------------------------------------

#include <future> // declara thread, async y future
#include <iostream>
using namespace std;

/*
    Busco que dos hebras claculen de forma concurrente el factorial de
    un numero, tengo mi funcion factorial
    Implementamos el resultado con una variable global (No recomendado)
*/

// DECLARO MIS VARIABLES LOCALES
// variables locales donde se escriben mis resultados
// resultado de mis hebras
long long resultado1, resultado2;

// Funcion factorial
long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

// Funcion de mis hebras

// funcion mi hebra 1
// calcula el factorial de un numero n y se guarda en variable local
void funcion_hebra_1(int n) { resultado1 = factorial(n); }

void funcion_hebra_2(int n) { resultado2 = factorial(n); }

int main() {
  // declaro mis hebras
  thread hebra1(funcion_hebra_1, 10), hebra2(funcion_hebra_2, 5);

  // espero a que mis hebras termines
  hebra1.join();
  hebra2.join();

  // expulso el resultado buscado
  cout << "Factorial de 10: " << resultado1 << endl;
  cout << "Factorial de 5: " << resultado2 << endl;
}