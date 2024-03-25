// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 2: Ejercicio3.cpp
// Ejemplo de intercambio Sincrono mensaje con array
//
// -----------------------------------------------------------------------------

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include </usr/local/openmpi/include/mpi.h> // #include <mpi.h>
using namespace std;

#define CHAR_BUFFER_SIZE 1024

// -----------------------------------------------------------------------------
// Variables GLobales

// -----------------------------------------------------------------------------
// main

int main(int argc, char *argv[]){

    // Inicializamos obteniendo el id propio y el num_proceso actual
    int id_propio, num_proceso_actual;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proceso_actual);

    // buscamos los procesos pares en esta ocasion
    // necesitamos que el total de procesos sea para para que cada uno tenga su pareja

    if (num_proceso_actual %2 == 0){
        // variables a necesitar
        
        MPI_Status estado;
        int tam_buff = 128;
        char valor_enviado[tam_buff];
        char valor_recibido[tam_buff];

        

        //int valor_enviado = 5*id_propio; // valor cualesquiera
        //int valor_recibido;
  
        
        int id_vecino= id_propio; // necesitamos saber quien es nuestro vecino

        /*
            Hemos dicho que si es par envia y recibe
            si es impar recibe y envia
            asi no hay interbloqueo
            dividimos los procesos
        */

        
        if (id_propio % 2 == 0){ // si es par
            id_vecino = id_vecino + 1; // mi vecino es una mas que yo "proceso 0 tiene a 1, 2 tiene a 3, 4 tiene a 5"
            

            // Envio Sincrono
            // int MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
            MPI_Ssend(&valor_enviado, 1, MPI_INT, id_vecino, 0, MPI_COMM_WORLD);
          
            MPI_Recv(&valor_recibido, 1, MPI_INT, id_vecino, 0, MPI_COMM_WORLD, &estado);



        } else { // si es impar
            id_vecino = id_vecino - 1;
            MPI_Recv(&valor_recibido, 1, MPI_INT, id_vecino, 0, MPI_COMM_WORLD, &estado);
            MPI_Ssend(&valor_enviado, 1, MPI_INT, id_vecino, 0, MPI_COMM_WORLD);
            
        }

        // informo
        cout << "El proceso " << id_propio << " Ha recibido " << valor_recibido << " del proceso " << id_vecino << endl;



    }else if (id_propio == 0) {
            cerr << "Error: se esperaba un numero par de procesos" << endl;
    }



    // Finalizamos 
    MPI_Finalize();
    return 0;

}