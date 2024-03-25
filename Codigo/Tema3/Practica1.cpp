

// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3. Introducción al paso de mensajes con MPI
//
// Archivo: Practica1.cpp
// Problema Productor y consumidor con buffer acotado. Solucion distribuida v1
//
// -----------------------------------------------------------------------------

/*
    En la sol distribuida hay 3 procesos 
        -prductor
        -buffer
        -Consumidor
    
*/

#include <iostream>
#include </usr/local/openmpi/include/mpi.h>
#include <chrono>
#include "scd.h"

using namespace std;
using namespace scd;
using namespace std::chrono;
using namespace std::this_thread;
// -----------------------------------------------------------------------------
// Variables globales


const int id_productor = 0;
const int id_buffer= 1;
const int id_consumidor = 2;
const int num_procesos_esperado = 3;
const int num_items = 20;

// -----------------------------------------------------------------------------
// funciones auxiliares

int producir()
{
   static int contador = 0 ;
   sleep_for( milliseconds( aleatorio<10,200>()) );
   contador++ ;
   cout << "Productor ha producido valor " << contador << endl << flush;
   return contador ;
}

void consumir( int valor_cons )
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<10,200>()) );
   cout << "Consumidor ha consumido valor " << valor_cons << endl << flush ;
}

// -----------------------------------------------------------------------------
// funciones

/*
    LOs procesos productor y consumidor usan envio sincrono seguro
*/

// 3. realiza peticiones al buffer, como respuesta recibe los enteros y los consume
void funcion_consumidor(){

    int peticion;
    int valor_recibido = 1;
    MPI_Status estado;

    for (int i = 0; i < num_items; i++){
        MPI_Ssend(&peticion,1,MPI_INT,id_buffer,0,MPI_COMM_WORLD);
        MPI_Recv(&valor_recibido,1,MPI_INT,id_buffer,0,MPI_COMM_WORLD,&estado);
        cout << "Consumido ha recibido el valor " << valor_recibido << endl;
        consumir(valor_recibido);
    }

    
}

// 1. produce una serie de datos y los envia al buffer
void funcion_productor(){
    
    for (int i = 0; i < num_items; i++){
        
        int valor_producido = producir();

        // informo
        cout << "Productor va a enviar valor " << valor_producido << endl;
        MPI_Ssend(&valor_producido,1,MPI_INT,id_buffer,0,MPI_COMM_WORLD);
    }


}

// 2. Recibe de forma alterna enteros del proceso productor y peticiones del consumidor. Responde
// al consumidor enviandole los enteros recibidos, en el mismo orden.
void funcion_buffer(){
    int valor;
    int peticion;
    MPI_Status estado;

    for (int i = 0; i < num_items; i++){
        
        // Recibo el valor del productor
        MPI_Recv(&valor, 1, MPI_INT, id_productor, 0, MPI_COMM_WORLD, &estado);

        cout << "Buffer ha recibido el valor del productor: " << valor << endl;

        // Recibo la peticion y envio el dato al consumidor
        MPI_Recv(&peticion, 1, MPI_INT, id_consumidor, 0, MPI_COMM_WORLD, &estado);
        cout << "El buffer va a enviar " << valor << endl;

        MPI_Send(&valor, 1, MPI_INT, id_consumidor, 0, MPI_COMM_WORLD);

    }
}

// -----------------------------------------------------------------------------
// main

int main (int argc, char *argv[]){

    // inicializamos
    int id_propio,num_proceso_actual;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proceso_actual);

    if (num_procesos_esperado == num_proceso_actual){
        
        if (id_propio == id_productor)
            funcion_productor();
        else if (id_propio == id_buffer)
            funcion_buffer();
        else
            funcion_consumidor();

    } else {

        cerr << "ERROR con el numero de procesos " << endl;

    }
    // FInalizamos
    MPI_Finalize();
    return 0;
}

