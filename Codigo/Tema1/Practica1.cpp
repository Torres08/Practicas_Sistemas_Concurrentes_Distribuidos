// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Practica 0
// Problema Consumidor Productor
//
// -----------------------------------------------------------------------------

/*
    Quiero diseñar un programa donde un proceso o hebra produce items de datos
    en memoria que otro proceso o hebra consume.

    En general, el productor calcula oproduce una secuencia de items de datos( uno a uno) y 
    el consumidor lee o consume dicho items (tmb uno a uno)
*/

/*
    Planteamiento:

    Implementar el productor y el consumidor como dos hebras independientes 
     -> Esto permite tener ocupadas las CPUs disponibles el max tiempo

    Usamos una var compartida que contiene un item de dato, como en el 
    ejemplo Ejercicio12.cpp pero es poco eficiente

    Para mejorar la eficiencia, usamos un vector de items que contenga datos 
    producidos y pendientes de leer 
     -> vector tamaño fijo K

*/

// -----------------------------------------------------------------------------

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "scd.h"

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables globales y semaforos

const unsigned 
    num_items = 40 ,   // número de items
	tam_vec   = 10 ;   // tamaño del buffer
unsigned siguiente_dato       = 0 ;  // siguiente dato a producir en 'producir_dato' (solo se usa ahí)


/*
    Dos opcioes para gestionar la ocupacion del vector compartido

    LIFO: Se usa una variable entera
    primera_libre = indice en el vector de la primera celda libre (inicialmente 0)
                    incrementamos al escribir y decrementamos al leer

    FIFO: Se usa dos variables enteras no negativas
    primera ocupada: incrementa al leer
    primera libre: incrementa al escribir

    ambas a modulo de tam_vec e inicialmente a 0
*/

unsigned buffer[tam_vec] = {0}; 

//FIFO
int plibre = 0;

//LIFO
int primera_libre = tam_vec;
int primera_ocupada = 0;

unsigned  
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido.
   cont_cons[num_items] = {0}; // contadores de verificación: para cada dato, número de veces que se ha consumido.

Semaphore libres = tam_vec;
Semaphore ocupadas = 0;

// -----------------------------------------------------------------------------
// Funciones

unsigned producir_dato()
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   const unsigned dato_producido = siguiente_dato ;
   siguiente_dato++ ;
   cont_prod[dato_producido] ++ ;
   cout << "producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;

}

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}


// -----------------------------------------------------------------------------
// Funciones para las hebras

void funcion_hebra_productora(){
    for (unsigned i = 0; i < num_items; i++){
        unsigned dato = producir_dato();

        libres.sem_wait();
        //SC

        //OPCION FIFO
        buffer[plibre]=dato;
        plibre++;


        //OPCION LIFO
        //buffer[primera_libre] = dato;
        //primera_libre++;
        //primera_libre = primera_libre % tam_vec;

        ocupadas.sem_signal();

    }
}

void funcion_hebra_consumidora(){
    for (unsigned i = 0; i < num_items; i++){
        unsigned dato;

        ocupadas.sem_wait();
         //SC
        

         //OPCION FIFO
         dato = buffer[plibre -1 ];
         plibre--;

         //OPCION LIFO
         //dato = buffer[primera_ocupada];
         //primera_ocupada++;
         //primera_ocupada = primera_ocupada % tam_vec;


         cout << "Extraigo: " << dato << endl;

        libres.sem_signal();

        consumir_dato(dato);
    }
}

// -----------------------------------------------------------------------------
// main

int main (){

    cout << "-----------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución LIFO / FIFO )." << endl
        << "------------------------------------------------------------------" << endl
        << flush ;

   thread hebra_productora ( funcion_hebra_productora ),
          hebra_consumidora( funcion_hebra_consumidora );

   hebra_productora.join() ;
   hebra_consumidora.join() ;

   test_contadores();

}

