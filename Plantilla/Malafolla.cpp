// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Ejercicio 1
//
// -----------------------------------------------------------------------------

#include <iostream>          
//#include <mpi.h>
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
const int num_procesos = 11; // 5 filosofos, 5 tenedores, 1 camarero


const int etiqueta_sentarse = 0;
const int etiqueta_levantarse = 1;
const int etiqueta_malafolla = 2;

const int id_camarero = 10;
const int id_malafolla = 8;

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

void funcion_filosofos_amable(int id_propio, bool hambriento){
    
    // Determino cual es mi tenedor de la derecha y el de la izquierda
    int id_tenedor_izquierda = (id_propio+1) % num_tenedores; // % es por que hay varios, no solo uno
    int id_tenedor_derecha = (id_propio+num_tenedores-1) % num_tenedores;
    
    int valor;
    int esta_malafolla;

    MPI_Status estado;

    while (true){

        // 1. Se sienta
        cout << "El filosofo amable" << id_propio << " duda si sentarse, llama al camarero " << endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiqueta_sentarse, MPI_COMM_WORLD);

        // 2. Se pregunta si esta malafolla al camarero
        cout << "El filosofo " << id_propio << " pregunta por el filosof malafolla " << endl;
        MPI_Recv(&esta_malafolla,1,MPI_INT,id_camarero,etiqueta_malafolla,MPI_COMM_WORLD,&estado);
        
        if (!esta_malafolla || hambriento) {
            // se sienta y come

            cout << "El filosofo amable" << id_propio << " se sienta " << endl;

            // 2. Toma tenedores
            cout <<"Filósofo " <<id_propio <<" solicita ten. izquierda " <<id_tenedor_izquierda <<endl;
    	    MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

	        cout <<"Filósofo " <<id_propio << " solicita ten. derecha" <<id_tenedor_derecha  <<endl;
	        MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

            come(id_propio); // hambriento pasa a false
            hambriento = false;

            // 4. Suelta tenedores
            cout <<"Filósofo " <<id_propio <<" suelta ten. izquierda " <<id_tenedor_izquierda <<endl;
            MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

            cout<< "Filósofo " <<id_propio <<" suelta ten. derecha " <<id_tenedor_derecha <<endl;
            MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

        } else {
            // si no no come
            cout << "El filosofo "<< id_propio << " decide no comer " << " por lo que esta hambriento ";
            hambriento = true;
        }
        

        // 5. Se levanta
        cout << "El filosofo " << id_propio << " llama al camarero para levantarse " << endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiqueta_levantarse, MPI_COMM_WORLD);

        // 6. Piensa
        piensa(id_propio);
    }
}

void funcion_filosofo_malafolla(int id_propio){

    // siempre come
    while (true){

        // Determino cual es mi tenedor de la derecha y el de la izquierda
        int id_tenedor_izquierda = (id_propio+1) % num_tenedores; // % es por que hay varios, no solo uno
        int id_tenedor_derecha = (id_propio+num_tenedores-1) % num_tenedores;
        int valor;

        // 1. Se sienta
        cout << "El filosofo malafolla" << id_propio << " llama al camarero para sentarse " << endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiqueta_sentarse, MPI_COMM_WORLD);

        // 2. Toma tenedores
        cout <<"Filósofo malafolla" <<id_propio <<" solicita ten. izquierda " <<id_tenedor_izquierda <<endl;
    	MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

	    cout <<"Filósofo malafolla" <<id_propio << " solicita ten. derecha" <<id_tenedor_derecha  <<endl;
	    MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

        // 3. Come
        come(id_propio);

        // 4. Suelta tenedores
        cout <<"Filósofo malafolla" <<id_propio <<" suelta ten. izquierda " <<id_tenedor_izquierda <<endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_izquierda, 0, MPI_COMM_WORLD);

        cout<< "Filósofo malafolla" <<id_propio <<" suelta ten. derecha " <<id_tenedor_derecha <<endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_tenedor_derecha, 0, MPI_COMM_WORLD);

        // 5. Se levanta
        cout << "El filosofo malafolla" << id_propio << " llama al camarero para levantarse " << endl;
        MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiqueta_levantarse, MPI_COMM_WORLD);

        // 6. Piensa
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
        cout << "El filosofo " << id_filosofo << " coge el tenedor " << id_propio << endl; 
    }
}

void funcion_camarero(){

    const int maximo_filosofos_sentador= num_filosofos -1 ;// no puede haber 5 sentados
    int num_sentados = 0; // contador
    int esta_malafolla = 0;

    int etiqueta_aceptable;

    int valor;
    int id_filosofo;
    MPI_Status estado;

    while(true){
       
            if (num_sentados < 4)
                etiqueta_aceptable = MPI_ANY_SOURCE;
            else
                etiqueta_aceptable=etiqueta_levantarse;

            // Solo recibe mensajes aquellos que tengan el tag sentarse "esten sentados"
            // y cuando num_sentados no sea 5

            MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiqueta_aceptable, MPI_COMM_WORLD, &estado);
            id_filosofo = estado.MPI_SOURCE;

            switch(estado.MPI_TAG){

                case etiqueta_sentarse:
                    num_sentados++;
                    cout << "Se ha sentado el filosofo " << id_filosofo << endl;
                    
                    if (id_filosofo != 8){
                        cout << "Camarero responde al filosofo amable " << endl;
                        MPI_Send(&esta_malafolla,1,MPI_INT,id_filosofo,etiqueta_malafolla,MPI_COMM_WORLD);

                    } else {
                        esta_malafolla = 1;
                    }

                break;

                case etiqueta_levantarse:
                
                    num_sentados--;
                    cout << "Se ha levantado el filosofo " << id_filosofo << endl;
                    if (id_filosofo == 8)
                        esta_malafolla = 0;

                break;

            }
        

       
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
      
      if (id_propio % 2 == 0){
        // si es par
        if (id_propio == id_camarero)
            funcion_camarero();
        else if (id_propio == id_malafolla)
            funcion_filosofo_malafolla(id_propio);
        else
            funcion_filosofos_amable(id_propio,true);


      } else {
          // si es impar
          funcion_tenedor(id_propio);
      }
      
       

   } else if (id_propio == 0)
        cerr << "Error: Se esperan 11 procesos " << endl;

    MPI_Finalize();
    return 0;
}