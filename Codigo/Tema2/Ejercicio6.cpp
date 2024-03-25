// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: barrera1_sc.cpp
// Aplicar problema Prod Cons con monitores
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <condition_variable>
#include <thread>
#include <random>
#include <cassert>
#include "scd.h"

using namespace std;
using namespace scd;

/*
    Dos opcioes para gestionar la ocupacion del buffer

    LIFO: Se usa una variable entera
    primera_libre = indice en el vector de la primera celda libre (inicialmente 0)
                    incrementamos al escribir y decrementamos al leer

    FIFO: Se usa dos variables enteras no negativas
    primera ocupada: incrementa al leer
    primera libre: incrementa al escribir

    ambas a modulo de tam_vec e inicialmente a 0
*/

// -----------------------------------------------------------------------------
// Variables Globales
const unsigned 
    num_items = 40 ;  // número de items
    const int tam_vec=10;
    
unsigned siguiente_dato       = 0 ;  // siguiente dato a producir en 'producir_dato' (solo se usa ahí)

    unsigned  
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido.
   cont_cons[num_items] = {0}; // contadores de verificación: para cada dato, número de veces que se ha consumido.

mutex
   mtx ;                 // mutex de escritura en pantalla



// -----------------------------------------------------------------------------
// Monitor

class MonitorPC{
private:
    int buffer[tam_vec]; // array con los datos insertados pendientes de extraer
    int primera_libre; // Sol LIFO 

    // la condition variable sustituye a mi funcion de semaforo en un monitor
    condition_variable libres; // cola de espera hasta que nº hebras < tam_vec (prod)
    condition_variable ocupadas; // cola de espera hasta  nº hebras > 0 (cons.)

    // creo mi guarda
    mutex cerrojo;
   
public:

    MonitorPC();
    void insertar(int dato); // escribe un valor (E) CONSUMIDOR
    int extraer(); // lee un valor (L) PRODUCTOR
};

// -----------------------------------------------------------------------------
// Constructor
MonitorPC::MonitorPC(){
    primera_libre = 0;
}
// -----------------------------------------------------------------------------
// Funcion Monitor

void MonitorPC::insertar(int dato){
    
    // escribe un valor (E) CONSUMIDOR
    
    //0. guarda
    unique_lock<mutex> guarda(cerrojo);

    // estamos en la hebra productora, insertamos
    // condicion impuesta en el apartado, si esta llena
    if (primera_libre == num_items) // si vemos que plibres, nuestro contador es igual al tamaño ya no quedan libres, los ponemos a espera
        libres.wait(guarda);

    assert(primera_libre < num_items);// TIENE QUE SER ASSERT PARA LOGRAR LA EM
        
        // si no esta lleno mi buffer, introduzco uno en la cola
        // incremento primera libre y luego notifico a la cola
        // de ocupada que se ha llenado una
        
        buffer[primera_libre] = dato;
        ++primera_libre;

        ocupadas.notify_one();
    
     


}

int MonitorPC::extraer(){
    //0. guarda
    unique_lock<mutex> guarda(cerrojo);
    int valor;

    if (primera_libre == 0)
        ocupadas.wait(guarda);
    assert(0 < primera_libre);
        // si primera_libre es mayor que 0 puedo extraer
        --primera_libre;
        valor = buffer[primera_libre];

        // he liberado una pos libre
        libres.notify_one();
    

    return valor;

}

// -----------------------------------------------------------------------------
// Funciones

unsigned producir_dato()
{
   static int contador = 0 ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "producido: " << contador << endl << flush ;
   mtx.unlock();
   cont_prod[contador] ++ ;
   return contador++ ;
}

void consumir_dato( unsigned dato )
{
  if ( num_items <= dato )
   {
      cout << " dato === " << dato << ", num_items == " << num_items << endl ;
      assert( dato < num_items );
   }
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "                  consumido: " << dato << endl ;
   mtx.unlock();
}

void ini_contadores()
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  cont_prod[i] = 0 ;
      cont_cons[i] = 0 ;
   }
}

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." << flush ;

   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      if ( cont_prod[i] != 1 )
      {
         cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {
         cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

// -----------------------------------------------------------------------------
// Funciones Hebras

void funcion_hebra_productora(MonitorPC *monitor){
    for (int i = 0; i < num_items; i++){
        int dato = producir_dato();
        monitor->insertar(dato);
    }
}

void funcion_hebra_consumidora(MonitorPC *monitor){
    for (int  i= 0; i < num_items; i++){
        int dato = monitor->extraer();
        consumir_dato(dato);
    }
}

// -----------------------------------------------------------------------------
// Main

int main (){

    // inicializo
    MonitorPC monitor;

    thread hebra_consumidor(funcion_hebra_consumidora, &monitor),
            hebra_productor(funcion_hebra_productora, &monitor);
    // espero
    hebra_consumidor.join();
    hebra_productor.join();

    // compruebo
    test_contadores();
}

