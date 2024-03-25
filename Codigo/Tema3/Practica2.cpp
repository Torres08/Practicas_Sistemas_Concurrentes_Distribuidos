// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3. Introducción al paso de mensajes con MPI
//
// Archivo: Practica2.cpp
// Problema Productor y consumidor con buffer acotado. Solucion con seleccion no determinista
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
const int tam_vector = 10;

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
   
    // ESPERA SELECTIVA, BUFFER
    int buffer[tam_vector]; // buffer con celdas ocupadas y vacias
    int valor;
    int primera_libre = 0;
    int primera_ocupada = 0;
    int num_celdas_ocupadas = 0;
    int id_emisor_aceptable;

    MPI_Status estado;  // los metadatos del mensaje

    // se itera el doble de num items
    for (int i = 0; i < num_items*2; i++){

        // 1. Determina si se puede enviar solo productos, solo consumidor o todos
        if (num_celdas_ocupadas == 0)
            id_emisor_aceptable = id_productor; // SI EL BUFER ESTA VACIO HAY QUE PRODUCIR
        
        else if (num_celdas_ocupadas == tam_vector)
            id_emisor_aceptable = id_consumidor;    // CUANDO ESTE LLENO SE CONSUME
        
        else 
            id_emisor_aceptable =MPI_ANY_SOURCE;    // SI NO, NO NOS IMPORTA LO QUE SE HACE

        // 2. Recibir un mensaje del emisor o emisores aceptables
        MPI_Recv(&valor, 1, MPI_INT, id_emisor_aceptable, 0, MPI_COMM_WORLD, &estado);

        // 3. Se procesa el mensaje recibido
        // miramos a ver lo que valor el iestado.MPI_SOURCE
        // ESTAMOS USANDO FIFO

        switch (estado.MPI_SOURCE) {
            
            case id_productor: // si ha salido el productor, insertamos valor en el buffer
            
                buffer[primera_libre] = valor;
                primera_libre=(primera_libre+1) % tam_vector;
                num_celdas_ocupadas++;
                // informamos
                cout << "Buffer ha recibido el valor " << valor << endl;
                break;

            break;
            case id_consumidor: // si ha salido consumidor extraemos y enviamos

                valor = buffer[primera_ocupada];
                primera_ocupada = (primera_ocupada+1) % tam_vector;
                num_celdas_ocupadas --;

                //informamos
                cout << "Buffer va a enviar el valor " << valor << endl;
                MPI_Ssend(&valor, 1, MPI_INT, id_consumidor, 0, MPI_COMM_WORLD);
                 
            break;
        }
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

