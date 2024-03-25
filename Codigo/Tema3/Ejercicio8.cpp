
// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3. Introducción al paso de mensajes con MPI
//
// Archivo: Ejercicio8.cpp
// Intercambio asíncrono (inseguro) de mensajes entre pares de procesos
//
// -----------------------------------------------------------------------------
#include <iostream>
#include </usr/local/openmpi/include/mpi.h>
using namespace std;


// -----------------------------------------------------------------------------
// Variables Globales

// -----------------------------------------------------------------------------
// funciones

// -----------------------------------------------------------------------------
// main

int main (int argc, char *argv[]){

    // Inicializamos
    int id_propio, num_procesos_actuales;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos_actuales);

    // caso minimo

    if (num_procesos_actuales  %2 == 0){
        // Como en el ejercicio 4 de los vecinos

        // Variables que necesitamos
        MPI_Status estado;
        
        int valor_enviado = 5; // envio el 5
        int valor_recibido;

        int id_vecino = id_propio;

        // EN el caso de usar eventos asincronos necesitamos crear tickets

        MPI_Request ticket_envio;
        MPI_Request ticket_recibido;

        // como usamos operaciones inseguras, no hace falta dividir el proceso

        // dependiendo si es par o impar actualizamos el id_vecino

        if( id_propio % 2 == 0)
            id_vecino +=1;
        else
            id_vecino -=1;


        // las siguientes llamadas para recibir y lees pueden aparecer en cualquier orden

        // envio
        MPI_Isend(&valor_enviado, 1, MPI_INT, id_vecino, 0, MPI_COMM_WORLD, &ticket_envio);

        // recibo
        MPI_Irecv(&valor_recibido,1,MPI_INT,id_vecino,0,MPI_COMM_WORLD,&ticket_recibido);

        // al usar eventos asincronos tengo que comprobar el ticket para saber el estado de la funcion -> paso al estado
        // uso wait -> espera bloqueada

        MPI_Wait(&ticket_envio, &estado);

        MPI_Wait(&ticket_recibido, &estado);

        // informo
        cout << "Soy el proceso numero " << id_propio << " y he recibido " << valor_recibido << " de mi vecino " << id_vecino << endl;

        


    } else if (id_propio == 0){
        // caso error
        cerr << "Necesitamos numero par de procesos, pero hay " << num_procesos_actuales << " procesos" << endl;
    }

    // Finalizamos
    MPI_Finalize();
    return 0;

}