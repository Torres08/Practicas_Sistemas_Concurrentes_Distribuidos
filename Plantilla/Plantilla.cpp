// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Ejercicio 1
//
// -----------------------------------------------------------------------------


/*

    // Cabeceras más importantes

    #include <iostream>          
    #include <sys/types.h>
    #include <thread> // this_thread::sleep_for
    #include <random> // dispositivos, generadores y distribuciones aleatorias
    #include <chrono> // duraciones (duration), unidades de tiempo
    #include <mpi.h>
*/

/*
    Tema 3 Funciones a usar
    
    MPI_Init: inicializa el entorno de ejecucion de MPI
    MPI_Finalize: finaliza entorno
    MPI_Comm_size: determina el numero de procesos de un comunicador
    MPI_Comm_rank: determina el identificador del proceso en un comunmicador

    MPI_COM_WORLD -> comunicador univesal

    // enviar mensaje
    int MPI_Send((void *buf_emi, int num,MPI_Datatype datatype, int dest, int tag, MPI_Commm)
    
    // Recibe el mensaje
    int MPI_Recv (void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)

    MPI_ANY_SOURCE
    MPY_ANY_TAG

     estado.MPI_SOURCE ,estado.MPI_TAG

    // escribe el entero apuntado por num en una llamada MPI_Recv previa
    int MPI_get_count(MPI_Status *status, MPI_Datatype dtype, int *num)
                             
    Paso de mensajes sincronos

    //Inicia un envio, lee los datos y esoera ek inicio de recepcion
    int MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
    int MPI_Recv (void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
    usas Ssend y Recv en general

    Sondeo Mensajes

    permiten a un proceso receptor averiguar si hay algun mensaje pendiente por recibir, y en ese caso obtener sus metadatos "estado"
    metadartos (quien es el emisot, la etiqueta o el numero de items)

    // Ejercicio6
    // Hace una espera bloqueada hasta que haya un mensaje
    MPI_Probe ( int source, int tag, MPI_Comm, MPI_Status *status);

    // Consulta si hay mensajes pendientes en el momento
    MPI_Iprobe( int source, int tag, MPI_Comm, int *flag, MPI_Status *status)
    El entero apuntado por flags será mayor que 0 solo si hay algun mensaje enviado al proceso que llama, si no hay mensajes -> tag es 0

    Comunicacion Insegura
    //MPI_Isend: inicia envio pero retorna ante de leer el buffer
    MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
    //MPI_Irecv: Inicia recepcion pero retorna antes de escribir
    int MPI_Irecv( void* buf, int count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Request *request );

    //MPI_Wait : espera bloqueada hasta que acabe el envio/recepcion
    MPI_Wait( MPI_Request *request, MPI_Status *status )
    //MPI_Test Comprueba si el envio o recepcion ha finalizado (no hay espera bloqueante)
    MPI_Test( MPI_Request *request, int *flag, MPI_Status *status )

    MPI_Request *request -> es un ticket -> con el identificamos la operacion cuyo resultado se pretende usar
    no hay *status en Isend o I recv -> obtenemos status con las operaciones de Consulta

*/


/*

    // enviar mensaje
    int MPI_Send((void *buf_emi, int num,MPI_Datatype datatype, int dest, int tag, MPI_Commm)

     // Recibe el mensaje
    int MPI_Recv (void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)

    MPI_ANY_SOURCE
    MPY_ANY_TAG

     estado.MPI_SOURCE ,estado.MPI_TAG

    MPI_Request ticket_envio;
    MPI_Request ticket_recibido;

    const int etiqueta_productor = 0;
    const int etiqueta_consumidor = 1;

    //Inicia un envio, lee los datos y espera el inicio de recepcion
    int MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

    // Hace una espera bloqueada hasta que haya un mensaje
    MPI_Probe ( int source, int tag, MPI_Comm, MPI_Status *status);

    // Consulta si hay mensajes pendientes en el momento
    MPI_Iprobe( int source, int tag, MPI_Comm, int *flag, MPI_Status *status)

    //MPI_Wait : espera bloqueada hasta que acabe el envio/recepcion
    MPI_Wait( MPI_Request *request, MPI_Status *status )

    //MPI_Test Comprueba si el envio o recepcion ha finalizado (no hay espera bloqueante)
    MPI_Test( MPI_Request *request, int *flag, MPI_Status *status )

    MPI_Request *request -> es un ticket -> con el identificamos la operacion cuyo resultado se pretende usar
    no hay *status en Isend o I recv -> obtenemos status con las operaciones de Consulta


*/

// -----------------------------------------------------------------------------
// Variables Globales

// -----------------------------------------------------------------------------
// funciones auxiliares

// -----------------------------------------------------------------------------
// funciones

// -----------------------------------------------------------------------------
// main

/*

int main( int argc, char ** argv){

   //Inicializamos

   int id_propio, num_procesos_actual;

   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( ){

   } else if (id_propio == 0){
       cerr << "Error, se esperaba x" << endl;
   }

    // MPI_Finalize();
    return 0
}

*/