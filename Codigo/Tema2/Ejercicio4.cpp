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
    Monitor sencillo llamado barrera con una unica cola condicion
    MONITOR NATIVO TIPO SEÑALAR Y CONTINUAR

    -   Hay n procesos usando el monitor, los procesos ejecutan un bucle y en 
        cada iteracion hacen una actividad, luego llaman a otro procedimiento exportado del monitor 
        llamado cita
    
    -   Cuando una hebra llama a cita en su iteracion numero k, no termina la llamada
        hasta que todas y cada una de las otras hebras hayan hecho tmb su
        llamada numero k
    
    -   Avanzan de forma sincrona

    -   Cada iteraciom, cada hebra debe esperar a que todas las demas invoquen a cita, excepto
        la ultima en llegar que despierta a las demas
            -> necesitamos variable permanente del monitor que nos indique cuantas hebras han llegado a la cita -> var llegados.

    -   Se inicializa a 0 y cada vez que una hebra habra cita se incrementa
    - Tras incrementar se comprueba respecto al total
        si llegados < total entonces ponemos la hebra en espera
        si no si llegados = todas -> despertamos a todas 

*/

#include <condition_variable> // Hay que incluirlo
#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include "scd.h"
#include <cassert>

using namespace std;
using namespace scd;


// -----------------------------------------------------------------------------
// Variables Globales

int num_iteraciones = 10;

// -----------------------------------------------------------------------------
// Monitor

class MonitorBarrera1{
private:
    int llegados; // numero de hebras que han ejecutado cita
    int total_hebras; // numero total de hebras usando el monitos
    condition_variable cola; // cola de hebras esperando hasta que llegados = total
    mutex cerrojo;


public:

    MonitorBarrera1(const int numero_hebras);
    void cita(int num_hebra);

};


// -----------------------------------------------------------------------------
// Constructor

    MonitorBarrera1::MonitorBarrera1(const int numero_hebras){
        total_hebras = numero_hebras;
        llegados = 0;

    }

// -----------------------------------------------------------------------------
// Funciones Monitor

void MonitorBarrera1::cita(int num_hebra){
    
    // 0. Pongo mi cerrojo para evitar la EM
    unique_lock<mutex> guarda(cerrojo);

    // 1. Incremento -> guardamos llegados en una var local const por si lo usamos
    llegados += 1;

    const int l = llegados;

    // informamos de lo que sucede

    cout << "Ha llegado la hebra " << num_hebra << " ( " << l << " ) " << endl;

    // 2. Compruebo si tengo que despertar a todas o no "Mi condicion"
    if (llegados < total_hebras){
         cola.wait(guarda); // ponemos la cola a esperar
    } else {
        // despierto a todas recorriendo todas las hebras
        //for (int i = 0; i < total_hebras-1; i++)
        //    cola.notify_one();
        cola.notify_all();
        llegados = 0; // tengo que ponerlo a 0
    }

    // Informo de lo sucedido
    cout << "   Ha salido la hebra " << num_hebra << " ( " << l << " ) " << endl;

}

// -----------------------------------------------------------------------------
// Funciones 

void duerme(){
    const int ms = aleatorio<50, 100>();
    this_thread::sleep_for( chrono::milliseconds(ms) );
}

// -----------------------------------------------------------------------------
// Funciones Hebras

void funcion_hebra(MonitorBarrera1 *monitor, int num_hebra){
    for (int i = 0; i < num_iteraciones; i++){
        duerme();
        monitor->cita(num_hebra);
    }

}

// -----------------------------------------------------------------------------
// Main

int main(){

    const int num_hebras = 5;

    // Inicializo
    MonitorBarrera1 monitor(num_hebras); // pongo 5 hebras

    thread hebras[num_hebras];

    for (int i=0; i < num_hebras; i++){
        hebras[i] = thread(funcion_hebra,&monitor,i);
    }

    // Espero
    for (int i = 0; i < num_hebras; i++)
        hebras[i].join();
}