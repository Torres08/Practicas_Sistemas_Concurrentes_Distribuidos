// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: monitor_em.cpp
// Ejemplo de monitores en C++11 con cerrojos
// (solo con encapsulamiento y exclusión mutua)
//
//  -- MContador2 : con EM con cerrojos
//
// -----------------------------------------------------------------------------

/*
    Arreglo el fallo del ejercicion 1 resolviendo la EM
    con cerrojos tipo Mutex
*/

#include "scd.h"
#include <cassert>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
//  Variables Locales
int num_iteraciones = 10000;

// -----------------------------------------------------------------------------
// Monitor

class MContador2 {
private:
  int contador;
  mutex cerrojo;
public:
  MContador2(int n);
  void incrementar();
  int lee_valor();
};

// -----------------------------------------------------------------------------
// Constructor
MContador2::MContador2(int n) { 
    contador = n; 
}

// -----------------------------------------------------------------------------
// Funciones

void MContador2::incrementar(){
    
    cerrojo.lock();
    contador++;     //SC
    cerrojo.unlock();

}

int MContador2::lee_valor(){
    int resultado;

    cerrojo.lock();
    resultado = contador; //SC
    cerrojo.unlock();

    return resultado;
}

// -----------------------------------------------------------------------------
// Funciones Hebras
void funcion_hebra(MContador2 *monitor) {
    for(int i=0; i < num_iteraciones; i++)
        monitor->incrementar();
}
// -----------------------------------------------------------------------------
// Main

int main() {

    // Inicializo
    MContador2 monitor(0);

    thread hebra1(funcion_hebra, &monitor),
           hebra2 (funcion_hebra, &monitor);

    // espero
    hebra1.join();
    hebra2.join();

    // veo si los resultados son coherentes

    cout  << "Monitor contador :" << endl
         << endl
         << "  valor esperado == " << 2*num_iteraciones<< endl
         << "  valor obtenido == " << monitor.lee_valor() << endl
         << endl ; 

}