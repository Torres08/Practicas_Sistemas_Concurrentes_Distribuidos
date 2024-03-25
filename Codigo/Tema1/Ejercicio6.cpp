// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 6 (ejemplo6.cpp)
// Creación de hebras idénticas con distintos valores de los parámetros.
// (usando un vector de variables 'thread')
//
// -----------------------------------------------------------------------------

#include <future>
#include <iostream>
using namespace std;

/*
    Suponemos que queremos usar n hebras identicas para calcular
   concurrentemente el factorial de cada uno de los numeros entre 1 y n, ambos
   incluidos

    Usamos vector de thread Ejemplo de uso
*/

// VARIABLES LOCALES
// el num  de hebras lo declaro global
int num_hebras = 8;

// funcion factorial
long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

// funcion de la hebra
// n: es mi indice
void funcion_hebras(int n) {
  int resultado = factorial(n + 1); // recuerda que los vectores empiezan en 0

  // hago la exspulsion aqui
  cout << "la hebra nº: " << n << " tiene un factorial " << n + 1
       << "  de: " << resultado << endl;
}

int main() {

  // declaro mi vector de hebras de forma separada
  thread hebras[num_hebras];

  // ahora inicializo con un for todas las hebras del vector
  for (int i = 0; i < num_hebras; i++) {
    hebras[i] = thread(funcion_hebras, i);
  }

  // Tengo que cerrar todas las hebras de mi vectos

  for (int i = 0; i < num_hebras; i++) {
    hebras[i].join();
  }
}