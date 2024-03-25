// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: barrera1_sc.cpp
// Ejemplo de un monitor 'Barrera' simple, con semántica SC
//
// -----------------------------------------------------------------------------

/*
*/

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <random>
#include <cassert>
#include "scd.h"

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables Globales

int num_iteraciones = 10;

// -----------------------------------------------------------------------------
// Monitor
class BarreraParcial{
private:

    int contador;
    int subconjunto;
    int total_hebras;
    condition_variable cola;
    mutex cerrojo;

public:
    BarreraParcial(int num_hebras);
    void cita(int num_hebra);
};

// -----------------------------------------------------------------------------
// Constructor

BarreraParcial::BarreraParcial(int num_hebras){
    contador = 0;
    total_hebras=num_hebras;
    subconjunto = 5;
}


// -----------------------------------------------------------------------------
// Funciones Monitor

void BarreraParcial::cita(int num_hebra){
    
    // pon la guarda
    unique_lock<mutex> guarda(cerrojo);
    
    contador++;

    const int valor = contador;
    cout << "Ha llegado la hebra " << num_hebra << " ( " << valor << " ) " << endl;

    if (contador < subconjunto){
            cola.wait(guarda);
    } else {
        cola.notify_all();
        contador = 0;
    }

     // Informo de lo sucedido
    cout << "   Ha salido la hebra " << num_hebra << " ( " << valor << " ) " << endl;

}

// -----------------------------------------------------------------------------
// Funciones 
void duerme(){
    const int ms = aleatorio<50, 100>();
    this_thread::sleep_for( chrono::milliseconds(ms) );
}

// -----------------------------------------------------------------------------
// Funciones Hebras
void funcion_hebra(BarreraParcial *monitor, int num_hebra){
    for (int i = 0; i < num_iteraciones; i++){
        duerme();
        monitor->cita(num_hebra);
    }
    
}
// -----------------------------------------------------------------------------
// Main

int main(){

    // Inicializo
    const int num_hebras = 10;

    BarreraParcial monitor(num_hebras);

    thread hebras[num_hebras];

    for (int i = 0; i < num_hebras; i++)
        hebras[i] = thread(funcion_hebra,&monitor, i);

    //espero

    for (int i = 0; i < num_hebras; i++)
        hebras[i].join();

}