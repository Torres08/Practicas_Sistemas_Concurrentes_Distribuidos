// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: barrera1_sc.cpp
// Monitores SU
//
// -----------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <thread>
#include "scd.h"
#include <condition_variable>
#include <mutex>
#include <random>
#include <cassert>
#include <iomanip>
#include <unistd.h>

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Var Globales

// -----------------------------------------------------------------------------
// Monitor

class MBarreraParSU: public HoareMonitor{
private:
    int contador, subconjunto;
    CondVar cola; // nuevo condition variable para aplicar SU, hay que inicializarlo

public:
    MBarreraParSU(int p_num_hebras_cita);
    void cita(int num_hebra);

};

// -----------------------------------------------------------------------------
// Constructor

MBarreraParSU::MBarreraParSU(int p_num_hebras_cita){
    subconjunto=p_num_hebras_cita;
    
    contador = 0;
    cola = newCondVar(); // hay que iniciar la cola en monitores SU

}

// -----------------------------------------------------------------------------
// Funciones Monitor

void MBarreraParSU::cita(int num_hebra){

    contador++;
    const int valor = contador;

    cout << "Llega la hebra " << num_hebra << " ( " << valor << " ) " << endl;

    if(contador < subconjunto)
        cola.wait();
    else{
        for (int i = 0; i < subconjunto-1; i++)
            cola.signal();
        
        contador = 0;
    }

    cout << "       Sale la hebra " << num_hebra << " ( " << valor << " ) " << endl;
}


// -----------------------------------------------------------------------------
// funciones

void duerme(){
    const int ms = aleatorio<50, 100>();
    this_thread::sleep_for( chrono::milliseconds(ms) );
}

// -----------------------------------------------------------------------------
// funciones Hebras

void funcion_hebra(MRef<MBarreraParSU> monitor, int num_hebra){
    for(int i =0; i < 10; i++){
        duerme();
        monitor->cita(num_hebra);
    }
}



// -----------------------------------------------------------------------------
// main

int main (){

    const int num_hebras = 100,
              subconjunto=10;

    // Creo el monitor e inicializo
    MRef<MBarreraParSU> monitor = Create<MBarreraParSU>(subconjunto);

    thread hebras[num_hebras];
    for (int i = 0; i< num_hebras; i++)
        hebras[i] = thread(funcion_hebra,monitor,i);

    // espero    
    for (int i =0;i < num_hebras; i++)
        hebras[i].join();

}