// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 2: Ejercicio7.cpp
// UN proceso receptor determina si hay mensajes pendientes de recibir de los emisores con idents
// desde id_min, id_max
// Uso de MPI_Iprobe para sondeo no bloqueante
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <cstring>
#include </usr/local/openmpi/include/mpi.h>
#include "scd.h"

using namespace std;
using namespace scd;
using namespace chrono;
using namespace this_thread;


// -----------------------------------------------------------------------------
// Variables Globales

int id_emisor = 0;
const int num_mensajes_por_emisor = 30; // como las iteraciones

// -----------------------------------------------------------------------------
// Funciones

//                    id_propio +1, num_proceso_actual-1
void funcion_receptor(int id_min, int id_max){

    /*
        Necesito para imprimir
            -> num_emisores
            -> total_mensajes
            -> cuenta[num_emisores] determina el numero de mensajes recibido por el emisor para activar el IPrope

    */

    int num_emisores = id_max-id_min +1; 
    int total_mensajes = num_mensajes_por_emisor*num_emisores;
    int contador[num_emisores];

    // 1. IMPORTANTE, INICIALIZAR EL CONTADOR

    for (int i = 0; i < num_emisores; i++)
        contador[i] = 0;
    
    cout << "Inicio del receptor " << endl;

    // 2. Recorro todos los mensajes

    for (int i = 0; i < total_mensajes; i++){

        // 3. inicializo las variables
        //      estado, hay_mensaje, id_emisor, valor
    
        MPI_Status estado; // estado de la recepcion
        int hay_mensaje; // 0 si no hay mensaje, != 0 si hay mensajes
        int id_emisor; // id emisor a recibir/recibido
        int valor; // valor a recibir/recibido

        // 4. Comprobamos si hay mensajes general
        
        // Seguimos un orden crecientede los posibles emisores
        //      -> id emisor toma valores entre id_min e id_max ambos incluidos

        // orden decreciente
        // for (id_emisor = id_max; id_emisor >= id_min; id_emisor--)

        for (id_emisor = id_min; id_emisor <= id_max; id_emisor++){
            
            // compruebo si hay mensaje a recibir "actualizo el tag hay_mensaje"

            // MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)
            MPI_Iprobe(id_emisor, MPI_ANY_TAG, MPI_COMM_WORLD, &hay_mensaje, &estado);

            // veo si hay_mensaje es distinto de 0 -> if true then -> rompemos

            if (hay_mensaje > 0)  break;
        }

        // 5. Compruebo si hay mensajes pendientes

        // si hay mensaje tras usar el Iprobe
        if (hay_mensaje){
            // informo, indico de quien es
            cout << "Hay al menos un mensaje del emisor: " << id_emisor << endl;
        } else{
            // no hay mensajes
            // si no hay mensajes, aceptamos uno cualesquiera
            id_emisor = MPI_ANY_SOURCE; // indico que el id_emisor puede ser cualesquiera
            cout << "No hay mensajes. Recibo cualesquiera. " << endl;
        }


        // 6. Tras comprobar los mensajes, los recibo "Tengo que recibir, si no cualesquiera"

        MPI_Recv(&valor, 1, MPI_INT, id_emisor, 0, MPI_COMM_WORLD, &estado);
        
        id_emisor = estado.MPI_SOURCE;  // MUY IMPORTANTE
        
        cout << "Receptor ha recibido de "<< id_emisor << endl;

        // 7. Actualizamos el contador

        // Veo el tamaño actual
        int tam_actual = id_emisor - id_min;

        // incremento mi contador, he recibido un mensaje
        contador[tam_actual] ++;

        cout << "Num mensaje recibido por cada emisor: " << endl;

        // veo los mensajes recibidos

        for (int i = 0; i < num_emisores; i++){
            cout << contador [i] << ", " ;
        }

        cout << "\n\n";

        // dormir un poco
        sleep_for(milliseconds(aleatorio<8, 12>()));

    }

}


void funcion_emisor(int id_propio){

    for (int i = 0; i < num_mensajes_por_emisor; i++){
       
        // Creo un valor
        int valor;

        // duermo
        sleep_for( milliseconds(aleatorio<8,12>() ) );
        
        // envio valor con Ssend
        MPI_Ssend(&valor, 1, MPI_INT, id_emisor, 0, MPI_COMM_WORLD);

        //cout << "el emisor nº " << id_propio << " ha enviado un mensaje" << endl;
    }

    cout << "acaba el emisor nº " << id_propio << endl;
}



// -----------------------------------------------------------------------------
// main

int main (int argc, char *argv[]){

    // inicializamos

    int id_propio, num_proceso_actual;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proceso_actual);
    
    // caso minimo ( no nos especifican nada)
    // tenemos que tenes como minimo +1 proceso

    if (num_proceso_actual > 1){
        // caso correcto
        
        // tenemos 2 funciones, receptor y varios emisores, tenemos el id del emisor
        if (id_propio == id_emisor){
            
            // hago la funcion del receptor
            // para escribir necesito el numero total de emisores ( total - receptor)

            // recibo n mensajes de cada eemisor
            funcion_receptor(id_propio+1, num_proceso_actual-1);

        } else{

            // emito n mensajes al receptor por emisor
            funcion_emisor(id_propio);
        } // son emisores
        


    } else { // caso error
        cerr << "Necesitamos como minimo 2 procesos " << endl;
    }

    // Finalizamos
    MPI_Finalize();
    return 0;
}