// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: monitor_em.cpp
// Ejemplo de monitores en C++11 sin variables condición
// (solo con encapsulamiento y exclusión mutua)
//
//  -- MContador1 : sin e.m.
//
// -----------------------------------------------------------------------------

/*
    Hago un ejemplo de uso de un monitor que hace la funcion de contador
   Mcontador1 no tiene E.M
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
// Variables Globales
int num_incrementos = 10000;

// -----------------------------------------------------------------------------
// Monitor

class MContador1 {
private:
  int contador;

public:
  MContador1(int i);
  void incrementar();
  int lee_valor();
};

// -----------------------------------------------------------------------------
// Constructor

   MContador1::MContador1(int i){
       contador = i;
   }

// -----------------------------------------------------------------------------
// Funciones

void MContador1::incrementar(){
    contador++;
}

int MContador1::lee_valor(){
    int i;
    i=contador;
    return i;
}

// -----------------------------------------------------------------------------
// Funciones Hebras

void funcion_hebra(MContador1 *monitor){
    for (int i = 0; i < num_incrementos; i++)
        monitor->incrementar();

}


// -----------------------------------------------------------------------------
// main

int main() {
    
    // Inicializo mi monitor y mis hebras a usar
    MContador1 monitor(0);

    thread hebra1(funcion_hebra, &monitor ),
           hebra2(funcion_hebra, &monitor);
    
    // Espero
    hebra1.join();
    hebra2.join();

    // Hago un cout para comprobar si el contador esta bien

     cout  << "Monitor contador :" << endl
         << endl
         << "  valor esperado == " << 2*num_incrementos << endl
         << "  valor obtenido == " << monitor.lee_valor() << endl
         << endl ; 


}