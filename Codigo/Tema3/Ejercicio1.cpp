// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 3:Introduccion a pasos de mensaje con MPI
//
// Archivo 1: Ejercicio1.cpp
// Ejemplo de uso de MPI con holamundo
//
// -----------------------------------------------------------------------------



#include </usr/local/openmpi/include/mpi.h> // no lo tengo en la carpeta include, lo tengo en otro lado
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{


    int id_propio,num_procesos;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procesos);
    MPI_Comm_rank(MPI_COMM_WORLD,&id_propio);

    cout << "Hola desde el proceso " << id_propio << " de " << num_procesos << endl;
    MPI_Finalize();
    return 0;
}


