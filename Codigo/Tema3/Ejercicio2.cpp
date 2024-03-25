// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 2: Ejercicio2.cpp
// Ejemplo MPI emisor y receptor
//
// -----------------------------------------------------------------------------

#include <iostream>
#include </usr/local/openmpi/include/mpi.h> // #include <mpi.h>
using namespace std;

// -----------------------------------------------------------------------------
// Variables globales/ comunicadores

/*
    IMPORTANTE
    mpirun -np 2 ./Ejercicio2
    2 -> SON 2 PROCESOS UNO QUE EMITE Y OTRO QUE RECIBE, NO HAY MAS
*/

const int id_emisor = 0; // identificador del emisor
const int id_receptor = 1; // identificador de receptor
const int num_procesos_esperado = 2; // numero de procesos esperados SE ESPERA EL EMISOR Y RECEPTOR, NO HAY MAS

// -----------------------------------------------------------------------------
// main

int main (int argc, char *argv[]){

    // variables
    int id_propio; // identificador propio, 
    int num_procesos_actual; // num procesos

    // Inicio MPI
    MPI_Init(&argc, &argv );

    // leo el identificador
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);

    // leo el numero de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos_actual);

    // envio el mensaje segun el num_proceso_esperado

    if (num_procesos_esperado == num_procesos_actual){

        // hace el envio de mensaje o recibe uno, segun el id_propio
        // 0-> emisor, 1-> receptor

        // En el caso correcto cada proceso depende de su rol

        if (id_propio == id_emisor){
            // actua como el emisor
            // ejemplo de lo que puede hacer un emisor

            int valor_enviado = 100; // envio el valor 100

            // Envio con la funcion Send
            // int MPI_Send((void *buf_emi, int num,MPI_Datatype datatype, int dest, int tag, MPI_Commm)
            
            /* Envio ->     el buff valor enviado, 
                            envio 1 elemento, 
                            datatype = MPI_INT, 
                            Se lo envio al receptor, 
                            no tiene tag (entero no negaativo), 
                            por el comunicador universal
            
            
            */

            MPI_Send(&valor_enviado, 1,MPI_INT, id_receptor, 0, MPI_COMM_WORLD);

            // Informo de que he enviado el mensaje
            cout << "Emisor ha enviado: " << valor_enviado << endl;


        } else {
            // si no es un emisor actua como un receptor 

            // declaro la variable a recibir
            int valor_recibido;
            
            // Defino el estado de la recepcion
            MPI_Status estado;

            // Recibo con la funcion Recv
            // int MPI_Recv (void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)

            /* 
                Recibo ->   el buf valor enviado,
                            envio 1 elemento,
                            datatype = MPI_INT
                            lo recibo del emisor (id)
                            estoy en el comunicador general/universal
                            defino un estado
            */

            MPI_Recv(&valor_recibido, 1, MPI_INT, id_emisor, 0, MPI_COMM_WORLD, &estado);

            // Comunico lo recivido 
            cout << "El receptopr ha recibido: " << valor_recibido << endl;

        }

    } else if (id_propio ==0 ){     // si es error, el primer proceso informa
                                    // aun no ha llegado a los 2 procesos para actuar

    // Entonces termino MPI en este caso

        cerr << "Esperados 2 procs, hay "<< num_procesos_actual << endl;
        
    }


    // finalizo fuera de las condiciones

    MPI_Finalize();
    return 0;

}