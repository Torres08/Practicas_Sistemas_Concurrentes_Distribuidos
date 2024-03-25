
// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 5 (ejemplo5.cpp)
// Obtención de resultados mediante 'return' y la llamada a 'async'
//
// -----------------------------------------------------------------------------

/*
    Vemos como obtener los resultados directamente de la funcion factorial
    sin variables globales ni parametros por salida (OPCION RECOMENDADA)
    uso metodo get de future
*/

#include <future>
#include <iostream>
using namespace std;

long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

int main() {

  // iniciar los future <> y asignarlos a un async relacionado con la funcion
  // implementar  y obtener los objetos con get

  future<long> futuro1 = async(launch::async, factorial, 10),
               futuro2 = async(launch::async, factorial, 5);

  // como son futures, variaciones de las hebras, no hace falta join, hacemos un
  // get para obetener su resultado

  cout << "Factorial de 10: " << futuro1.get() << endl;
  cout << "Factorial de 5: " << futuro2.get() << endl;
}