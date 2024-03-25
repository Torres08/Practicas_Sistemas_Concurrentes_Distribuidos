// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-plantilla.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// -----------------------------------------------------------------------------

#include <iostream>          
#include <sys/types.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include </usr/local/openmpi/include/mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

// -----------------------------------------------------------------------------
// Variables Globales

const int num_filosofos = 5;
const int num_tenedores = 2*num_filosofos;
const int num_procesos = 10;
// -----------------------------------------------------------------------------
// funciones auxiliares


//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

void come(int id_propio){

    cout << "El filosofo " << id_propio << " comienza a comer " << endl;
    sleep_for( milliseconds( aleatorio<10,100>() ) );

}

void piensa(int id_propio){

    cout << "Filosofo " << id_propio << " comienza a pensar" << endl;
    sleep_for( milliseconds( aleatorio<10,100>() ) );
}

// -----------------------------------------------------------------------------
// funciones

void funcion_filosofos(int id_propio){
    
    // Determino cual es mi tenedor de la derecha y el de la izquierda
    int id_tenedor_izquierda = (id_propio+1) % num_tenedores; // % es por que hay varios, no solo uno
    int id_tenedor_derecha = (id_propio+num_tenedores-1) % num_tenedores;
    int valor;

    while (true){

        //1. Toma los tenedores ( primero el izquierdo, luego el derecho)
        
        // solucion al interbloqueo
        if (id_propio == 0) { 
            cout <<"Filósofo " <<id_propio <<" solicita ten. der." <<id_tenedor_derecha <<endl;
    	    MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

	        cout <<"Filósofo " <<id_propio << " solicita ten. izq." <<id_tenedor_izquierda <<endl;
	        MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);
        }
        

        cout <<"Filósofo " <<id_propio <<" solicita ten. izquierda " <<id_tenedor_izquierda <<endl;
    	MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

	    cout <<"Filósofo " <<id_propio << " solicita ten. derecha" <<id_tenedor_derecha  <<endl;
	    MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);


        //2. Come 
        come(id_propio);
        

        //3. Solta los tenedores
        cout <<"Filósofo " <<id_propio <<" suelta ten. izquierda " <<id_tenedor_izquierda <<endl;
         MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

        cout<< "Filósofo " <<id_propio <<" suelta ten. derecha " <<id_tenedor_derecha <<endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

        //4. piensa
        piensa(id_propio);
    }
}

void funcion_tenedor(int id_propio){
    
    int valor, id_filosofo;
    MPI_Status estado;

    while (true){

        // 1. Espera hasta recibir un mensaje de un filosofo (mensaje peticion)
        // ANY_SOURCE -> DE CUALQUIERA
        MPI_Recv(&valor,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&estado);
        id_filosofo = estado.MPI_SOURCE;
        cout << "El filosofo " << id_filosofo << "coge el tenedor " << id_propio << endl; 

        // 2.Espera hasta recibir un mensaje del mismo filosofo emisor del anterior (liberacion)
        MPI_Recv(&valor,1,MPI_INT,id_filosofo,0,MPI_COMM_WORLD,&estado);
        cout << "El filosofo " << id_filosofo << "coge el tenedor " << id_propio << endl; 
    }
}

// -----------------------------------------------------------------------------
// main

int main( int argc, char ** argv){

   int id_propio, num_procesos_actual;

   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if (num_procesos == num_procesos_actual){ // tiene que ser 10
        // pares son filosofos
        if (id_propio % 2 == 0)
            funcion_filosofos(id_propio);
        else
            funcion_tenedor(id_propio);

   } else if (id_propio == 0)
        cerr << "Error: Se esperan 10 procesos " << endl;

    MPI_Finalize();
    return 0;
}