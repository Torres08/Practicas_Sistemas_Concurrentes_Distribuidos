// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Practica 2
// Fumadores
//
// -----------------------------------------------------------------------------

/*
    Tengo un estanco donde hay 3 fumadores y un estanquero

    - Cada fumador es una hebra que realiza la funcion fumar en un bucle infinito
    - Cada fumador debe de esperar antes de fumar, debe tener los ingredientes
    - El estanquero produce ingredientes para que los fumadores fumen en un bucle infinito
    - Se debe permitir que varios fumadores fumen simultaneamente
    - Cada fumador empieza con un ingrediente de 3, tabaco papel y cerillas
    - estanquero genera aleatoriamente un ingrediente
    - El estanquero, cuando se desbloquea vuelve a poner un ingrediente

    - fumadores : vector de hebras 0, 1, 2, tmb se enumeran igual los ingredientes
    - Cuando se pone el ingrediente, se informa con un mensaje (P)
    - el fumador retira un ingrediente (R)
    - Condicion de interfoliacion -> 0 <= P-R
    -Solo cabe un ingrediente en el mostrador, 0,1 -> semaforo mostr_vacio 1 si esta libre 0 no
*/
#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include "scd.h"

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables globales y semaforos

const int numero_fumadores = 3;
Semaphore fumadores[numero_fumadores] ={0,0,0}; // 1 si el ingrediente esta disponible, 0 no
Semaphore mostrador_vacio = 1; // 1 si mostrador vacio, 0 no


// -----------------------------------------------------------------------------
// Funciones

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,numero_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

// -----------------------------------------------------------------------------
// Funciones para las hebras

void funcion_hebra_estanquero(){
    
    while (true){
        int i = producir_ingrediente();
        // Como hemos producido un ingrediente, el mostrador no esta vacio -> pasa a 0, hacemos un wait
        
        mostrador_vacio.sem_wait();
        // Como hemos producido un ingrediente hacemos un cout de lo que hemos puesto
        cout << "Hemos puesto el ingrediente: " << i << endl;

        // ingrediente i corresponde al fumador i para que fume
        // en consecuencia, hay un ingrediente disponible para fumador[i] -> lo ponemos a 1

        fumadores[i].sem_signal();

        //SC
    }
}

void funcion_hebra_fumadores(int num_fumador){
    
    while (true){

        // Aqui se retira un ingrediente, por lo que deja de haber el ingrediente i para 
        // el fumador i, en consecuencia lo pasamos a 0
        int i = num_fumador;
        fumadores[i].sem_wait();

        // hago un cout de que se ha gastado el ingrediente 
        cout << "Se ha retirado el ingrediente: " << i << " por el fumador " << i << endl;

        // como se ha retirado el ingrediente del mostrador, el mostrador queda vacio lo ponemos a 1
        mostrador_vacio.sem_signal();
        

        // Ahora el fumador procede a fumar
        fumar(num_fumador);
    }
}

// -----------------------------------------------------------------------------
// main

int main(){

    thread hebra_fumadores[numero_fumadores],
            hebra_estanquero;

    // Inicializo
    hebra_estanquero = thread(funcion_hebra_estanquero);

    for (int i = 0; i < numero_fumadores; i++)
        hebra_fumadores[i] = thread(funcion_hebra_fumadores,i);

    // las espero
    hebra_estanquero.join();

    for (int i=0; i < numero_fumadores; i++)
        hebra_fumadores[i].join();
    

    

}