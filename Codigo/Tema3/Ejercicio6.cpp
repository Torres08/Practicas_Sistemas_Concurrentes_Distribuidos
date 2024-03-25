// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 2: Ejercicio6.cpp
// Un programa el cual los procesos mandan cadenas de texto a un proceso receptor que
// las imprime al recibirlas
//
// -----------------------------------------------------------------------------


/*
    Un programa el cual los procesos mandan cadenas de texto a un proceso receptor que
    las imprime al recibirlas
*/

#include <iostream>
#include </usr/local/openmpi/include/mpi.h> // #include <mpi.h>
#include <iostream>
#include <random>  // para 'aleatorio'
#include <thread>  // this_thread::sleep_for
#include <chrono>  // chrono::duration, chrono::milliseconds
#include <cstring>  // strlen
#include <cstdio> // snprintf
#include "scd.h" // para template de aleatorio

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables

const int id_impresion = 0;
const int num_iteraciones_por_emisor = 30; //

// -----------------------------------------------------------------------------
// Funciones

void envia_cadena(const char *cadena){
    
    int num_char=strlen(cadena); // determinoo el tamaño de la cadena

    // la envio a la impresion
    MPI_Ssend(cadena,num_char,MPI_CHAR, id_impresion, 0, MPI_COMM_WORLD);

}


// imprime el resultado
void funcion_receptor( int num_emisores){

    MPI_Status estado ;
   int num_chars_rec ;

    // total mensajes: 2 por emisor, + 2 por cada una de sus iteraciones
    const int num_total_msgs = num_emisores*(2*num_iteraciones_por_emisor+2) ; // num_iteraciones por emisor esta arriba en var globales

    cout << "Comenzamos a imprimir ... " << endl;

    for( int i = 0 ; i < num_total_msgs; i++ ){

        // 1.   Espero a un mensaje de un emisor "recuerda que con MPI_Probe no lo recibe"
        // MPI_Probe ( int source, int tag, MPI_Comm, MPI_Status *status)
        MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado );

        // 2. leo el numero de char del mensaje y lo paso -> num_char
        //MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)
        MPI_Get_count( &estado, MPI_CHAR, &num_chars_rec );

        // 3. Creo mi char para el mensaje
        // creo un array char y reservo memoria dinamica
       char * buffer = new char[num_chars_rec+1] ;

        // 4. Recibe el mensaje y se introduce en el array y añadimos un 0 al fina IMPORTANTE
        // IMPORTANTE: especificar el mismo emisor del probe
        // MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
        MPI_Recv( buffer, num_chars_rec, MPI_CHAR, estado.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado );
        buffer[num_chars_rec] = 0 ;

        // 5.Imprimo
        cout << buffer << endl ;

        // 6. Libero la memoria de mi array dinamico
        delete [] buffer ;

    }

    cout << "Fin de la impresion " << endl;
    
}

// envia cadenas al receptor
void funcion_emisor(int id_propio){

    const int max_char = 1024; // tamaño max cadena
    char cadena[max_char];

    // Write formatted output to sized buffer escribo en el char algo
    snprintf(cadena, max_char, "inicio del proceso numero: %d", id_propio);
    // envio la cadena con mi funcion
    envia_cadena(cadena);

    for (int i = 0; i < num_iteraciones_por_emisor; i++){ // cada emisor hace 30 iteraciones

        // tiempo a dormir
        int milliseconds = aleatorio<20, 200>();

        snprintf(cadena,max_char,"proceso %d: voy al baño ",id_propio);

        envia_cadena(cadena);

        this_thread::sleep_for(chrono::milliseconds(milliseconds));

        envia_cadena("Ya he terminado");

        //
    }

    snprintf(cadena,max_char,"fin del proceso numero: %d", id_propio);
    envia_cadena(cadena);
    

}

// -----------------------------------------------------------------------------
// Main

int main(int argc, char *argv[]){

    // inicializamos
    int id_propio, num_procesos_actual;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id_propio);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procesos_actual);

    // necesitamos que el num_procesos totales no sea 0, casi minimo
    if (num_procesos_actual > 1){

        // se ejecuta la funcion correspondiente al id propio
        if (id_propio == id_impresion){ // el proceso 0 se encarga de la impresion
            // funcion impresion, receptor
            funcion_receptor(num_procesos_actual-1); // incluyo todos los procesos menos al impresor

        } else {
            // resto espera y hace de emisor
            funcion_emisor(id_propio);
        }

    } else {
        // caso error
        cerr << "error: este programa se debe lanzar con al menos dos procesos" << endl ;
    }

    // finalizamos
    MPI_Finalize();
    return 0;

}