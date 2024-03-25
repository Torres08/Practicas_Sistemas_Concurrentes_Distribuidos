// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: monitor_em.cpp
// Ejemplo de monitores en C++11 con guardas
// (solo con encapsulamiento y exclusión mutua)
//
//  -- MContador3 : con EM con guardas de cerrojos
//
// -----------------------------------------------------------------------------

/*
    Concepto de Guarda
    Una guarda de cerrojo es una variable local a cada método exportado
    del monitor, variable que contiene una referencia al cerrojo del
    monitor
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include "scd.h"
#include <cassert>

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables Locales
int num_iteraciones = 10000;

// -----------------------------------------------------------------------------
// Monitor

class MContador3{
private:
    int contador;
    mutex cerrojo;

public:
    MContador3(int valor);
    int lee_valor();
    void incrementa();

};

// -----------------------------------------------------------------------------
// Constructor

MContador3::MContador3(int valor){
    contador = valor;
}

// -----------------------------------------------------------------------------
// Funciones 

void MContador3::incrementa(){
    unique_lock<mutex> guarda(cerrojo); // guarda, garantiza EM
    contador++;
}

int MContador3::lee_valor(){
    unique_lock<mutex> guarda(cerrojo); // guarda, garantiza EM
    int resultado = contador;
    return resultado;
}

// -----------------------------------------------------------------------------
// Funciones Hebras
 
void funcion_hebra(MContador3 *monitor){
    for (int i = 0; i< num_iteraciones; i++)
        monitor->incrementa();

}

// -----------------------------------------------------------------------------
// Main

int main (){

    // Inicializo
    MContador3 monitor(0);

    thread Hebra1(funcion_hebra, &monitor),
           Hebra2(funcion_hebra, &monitor);
        
    // Espero
    Hebra1.join();
    Hebra2.join();

    // Compruebo

    cout  << "Monitor contador :" << endl
         << endl
         << "  valor esperado == " << 2*num_iteraciones<< endl
         << "  valor obtenido == " << monitor.lee_valor() << endl
         << endl ; 




}