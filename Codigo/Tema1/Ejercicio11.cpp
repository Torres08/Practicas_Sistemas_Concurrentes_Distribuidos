
// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 11 (ejemplo11.cpp)
// Uso cerrojos, objetos mutex
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <future>
#include <mutex>

using namespace std;

//Variable local
// Declao mi cerrojo 
mutex cerrojo;
int num_hebras = 5;

// funcion del factorial de un numero
long factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

void funcion_hebras(int i){
    
    int fac = factorial(i+1);

    cerrojo.lock();
    //SECCION CRITICA: quiero que se haga 1 por 1 no haya intergoliacion
    // zona donde se ha de garantizar la exclusion mutua

     cout << "Hebra numero= " << i << " factorial (" << i+1 << ") = " << fac << endl;
    cerrojo.unlock();

}

int main (){
    thread hebras[num_hebras];

    for (int i = 0 ; i < num_hebras; i++)
        hebras[i] = thread(funcion_hebras, i);
    
    for (int i = 0; i< num_hebras; i++)
        hebras[i].join();
}