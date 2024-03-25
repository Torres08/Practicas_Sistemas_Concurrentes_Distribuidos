// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 4 (ejemplo4.cpp)
// Obtención de resultados mediante parámetros de salida (referencias)
//
// -----------------------------------------------------------------------------

// AÑADIMOS UN PARAMETRO DE SALIDA (POR REFERENCIA) A LA FUN HEBRA

#include <future>
#include <iostream>
using namespace std;

long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

// ahora declaro solo una funcion que comparte dos hebras
void funcion_hebra(int n, long &resultado) { resultado = factorial(n); }

int main() {
  // Para usar paso por referencia tengo que declararlas en el main
  long resultado1, resultado2;

  thread hebra1(funcion_hebra, 10, ref(resultado1)), // calcula fact 10
      hebra2(funcion_hebra, 5, ref(resultado2));     // calcula fact 5

  // espero a que mis hebras terminen
  hebra1.join();
  hebra2.join();

  // expulso el resultado buscado
  cout << "Factorial de 10: " << resultado1 << endl;
  cout << "Factorial de 5: " << resultado2 << endl;
}