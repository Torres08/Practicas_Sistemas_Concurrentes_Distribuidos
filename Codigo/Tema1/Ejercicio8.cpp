// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 8 (ejemplo8.cpp)
// Medicion de la duración de la ejecución de unas instrucciones.
//
// -----------------------------------------------------------------------------

/*
    Vamos a medir cuanto tiempo tarda en hacer una funcion
    En este caso el factorial de 20
*/

#include <iostream>
#include <chrono> // incluye las herramientas para medir el tiempo
                  // como now, timpe\_point, duration
using namespace std;
using namespace std::chrono; // HAY QUE USAR ESTA CABECERA TMB

// funcion factorial a usar
long factorial( int n ) { return n > 0 ? n*factorial(n-1) : 1 ; }

int main (){

    // 1. hago un steady clock now declarando una variable inicio con time_point
    // uso steady clock

    time_point<steady_clock> instante_inicio = steady_clock::now();

    // Funcion a la que le quiero medir el tiempo
    long resultado = factorial ( 20);

    // 2.Leo mi instante final, al igual que hice con el instante inicio
    time_point<steady_clock> instante_final = steady_clock::now();

    // 3. Calculo la duracion restando final-inicio
    // variable tipo duration

    duration<float,micro> Tiempo_Total =instante_final-instante_inicio;

    //4. Imprimo los resultados hago un .count y al ponerlo en micro es microsegundos
    cout << "El factorial de 20 es: " << resultado<< endl;
    cout << "El tiempo total en hacer la funcion es de : " << Tiempo_Total.count() << " microsegundos" << endl ;

}


