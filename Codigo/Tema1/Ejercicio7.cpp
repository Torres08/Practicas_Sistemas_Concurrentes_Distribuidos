// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 7 (ejemplo7.cpp)
// Creación de hebras idénticas con distintos parámetros.
// (usando async y un vector de futuros)
//
// -----------------------------------------------------------------------------

/*
    Suponemos que queremos que n hebras identicas hagan una misma funcion
    Usamos vector de futuros
    La hebra principal main hace la salida del resultado
*/

#include <future>
#include <iostream>
using namespace std;

// Variable local que muestra el num total de futures
const int num_futures = 8;

// funcion factorial
long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

int main() {

  // future -> hebras sin funcion
  future<long> futuros[num_futures];

  // inicializo los futures
  for (int i = 0; i < num_futures; i++)
    futuros[i] = async(launch::async, factorial, i + 1);

  // hago un get para el cout
  for (int i = 0; i < num_futures; i++)
    cout << "la hebra nº " << i << " hace el factorial: " << i + 1 << " = "
         << futuros[i].get() << endl;
}