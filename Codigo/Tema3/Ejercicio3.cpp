// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 2: Ejercicio3.cpp
// Difusion de mensaje en una cadena de proceso
//
// -----------------------------------------------------------------------------


/*
    Difusion de mensaje en una cadena de procesos
    Funciona con un numero de procesos como minimo igual a 2
    Todos los procesos ejecutan un bucle
        -el primer proceso (identificador = 0)
            -> pide un valor entero por teclado
        
        - el restp ( identificador > 0)
            -> reciben cada uno un valor del proceso anterior
    
    el primero recibe, el resto envia y recibe hasta el ultimo, que solo recibe

*/

#include <iostream>
#include </usr/local/openmpi/include/mpi.h> // #include <mpi.h>
using namespace std;


// -----------------------------------------------------------------------------
// Variables globales/ comunicadores

const int num_procesos_minimo= 2; // minimo numero de procesos para que se ejecute, cuidado al compilar

// -----------------------------------------------------------------------------
// main

int main( int argc, char *argv[]){
    
    // Variables
    int id_propio; // identificador propio
    int num_proceso_actual; // num_proceso

    // Inicio MPI
    MPI_Init(&argc, &argv);

    // leo el identificador
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio);
    
    // leo el numero de proceso
    MPI_Comm_size(MPI_COMM_WORLD, &num_proceso_actual);

    // minimo num procesos para que se pueda ejecutar
    if (num_procesos_minimo <= num_proceso_actual){
        
        
        // bucle de envio y de recepcion

        // variables locales

        // Juego con el id propio para saber que proceso va delante y cual a atras
         int id_anterior = id_propio-1;
         int id_siguiente = id_propio+1;
        
        int valor ; // valor a pasa;
        MPI_Status estado; // Necesito un estado

        do { // acaba cuando hay un valor negativo

            if (id_anterior < 0){ // si el id primero (0) es negativo -> id_propio - 1
               
                cin >> valor;   // Introduzco el valor
            } else {    // si no soy el primero, recibo el valor del anterior
                
                // Envio
                MPI_Recv(&valor, 1, MPI_INT, id_anterior, 0, MPI_COMM_WORLD,&estado);
            }
                // informo de que lo he enviado
                if (valor > 0)
                    cout << "Proceso nº " << id_propio << " ha recibido/leido el valor: " << valor << endl;
                
                    

                // comprueba que no soy el ultimo, no puede ser igualal num_proceso actual, ese es el ultimo
                if (id_siguiente < num_proceso_actual){ 
                    // Recibo
                    MPI_Send(&valor, 1, MPI_INT, id_siguiente, 0, MPI_COMM_WORLD);
                }

            

        }while(valor >= 0);

        

    } else if (id_propio == 0){
        // caso error
        // el primer proceso informa

        cerr << "Numero de procesos menor que el minimo ( " << num_procesos_minimo << " )" << endl;

    }

    // Finalizo el MPI

    
    MPI_Finalize();

    return 0;

}