// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 11 (ejemplo11.cpp)
// Ejemplo de uso de semaforos
// Problema de Productor/Consumidor (Lectura y escritura repetidas)
//
// -----------------------------------------------------------------------------

/*
    Buscamos una interfoliacion donde primero se escriba, luego lectura,
   escritura, l, e, l .. en un bucle El numero de valores escritos en la
   variable compartida(semaforo) tiene que ser de 0 o 1 si hay uno esperando se
   tiene que expulsar

    Implementamos 2 semaforos para solucionarlo
        semaforo puede_escribir -> 1 si se puede leer la variable compartida, 0
   si no semafor puede escribir, -> 1 puede leer 0 no

*/

#include "scd.h" // scd.h contiene las funciones de semaforos
#include <iostream>
#include <thread>
using namespace std;
using namespace scd; // tmb lo necesito para escribir los semaforos

// -----------------------------------------------------------------------------
// Variables Locales

/*
    Como variables locales pongo
    -   mi numero de iteraciones
    -   variable compartida a leer y su contador que te dice cuantas veces lee
    -   Semaforos compartidos

*/

const int iteraciones = 100;
int valor_compartido;
int contador = 0;

Semaphore puede_leer = 0;     // 1 si no hay valores para leer pendientes
Semaphore puede_escribir = 1; // 1 si hay valores pendientes de leer

// -----------------------------------------------------------------------------
// Funciones

/*
    Depende de lo que te pida el problema, solo incrementa contador y lo expulsa
*/

int producir_valor() {
  contador++;
  cout << "Producido: " << contador << endl;
  return contador;
}

/*
    Tras producir el valor, expulsa que se ha consimido dicho valoor
*/

void consumir_valor(int valor) { 
    cout << "       consumido: " << valor << endl; 
}

// -----------------------------------------------------------------------------
// Funciones Hebras

/*
    funcion que ejecuta la hebra productora, con los dos semaforos
    controlo que las interfoliaciones se haga bien

    Uso producir_valor

    Hebra consumidora -> escribe
    Todo en un bucle ocurre esto con los semaforos saltamos en bucle,, los ponemos a dormir para lograr la EM

    1.  Empieza produciendo (el semaforo puede_escribir inicializa a 1) asigna el valor_compartido con la funcion, genero el valor
    2.  Luego lo pone a esperar sem.wait
    3.  Escribe el valor e informa
    4.  Despierta al semaforo puede_leer

*/

void funcion_hebra_productora() {

  for (unsigned long i = 0; i < iteraciones; i++) { 

    int valor_producido = producir_valor(); // guardo la var a producir en una
                                            // var local de la funcion

    puede_escribir.sem_wait(); // pongo a esperar

    // SC
    valor_compartido =valor_producido; // guardo el valor producido en mi variable global
    cout << "Escrito: " << valor_producido << endl; // informo

    puede_leer.sem_signal(); // dicto a mi programa que lea, despierto dicho semaforo

  }
}

/*
    hebra consumidora -> lee
    Usa los dos semaforos para escribir la variable compartida
    uso la funcion consumir valor

    1. pongo lee a esperar, hago solo una lectura
    2. leo e informo
    3. llamo a que puede escribir
    4. LLamo a la funcion consumir valor

    El problema esta en que las lecturas y escrituras con la variable compartida se pueden joder
    esa es la seccion critica
    cuando trabajemos con el valor_compartido es la SC

    luego de terminar de asignar la var compartida, indico el otro semaforo y uso la funcion correspondiente

*/

void funcion_hebra_consumidora() {

    for (unsigned long i = 0; i < iteraciones; i++){

        puede_leer.sem_wait();
        
        //SC
        int valor_leido = valor_compartido;
        cout << "       Leido: " << valor_leido << endl;

        sem_signal(puede_escribir);

        consumir_valor(valor_leido);

    }

}

// -----------------------------------------------------------------------------
// main

int main() {

    // inicializo
    thread hebra_productora(funcion_hebra_productora),
            hebra_consumidora(funcion_hebra_consumidora);


    // las espero
    hebra_consumidora.join();
    hebra_productora.join();
}