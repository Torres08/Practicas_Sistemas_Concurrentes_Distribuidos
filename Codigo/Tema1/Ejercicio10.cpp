// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 10 (ejemplo10.cpp)
// Uso variables atomicas.
//
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic> // me permite declarar variables atomicas

using namespace std;
using namespace std::chrono;

//Variables locales que vamos a usar
const long iteraciones = 1000000l;
int const_no_atom;
atomic<int> const_atom;

// contador hasta 1000 no atom
void funcion_hebra_no_atom(){
    for (int i = 0; i < iteraciones; i++)
        const_no_atom++;
}

// contador hasta 1000 atom
void funcion_hebra_atom(){
    for (int i=0; i < iteraciones; i++)
        const_atom++;

}

int main (){
// inicializo 3 hebras con datos no atomicos
    thread hebra1(funcion_hebra_no_atom),
            hebra2(funcion_hebra_no_atom),
            hebra3(funcion_hebra_no_atom);
// las esperamos
        hebra1.join();
        hebra2.join();
        hebra3.join();

// inicializo 3 hebras con datos atomicos
    thread hebra4(funcion_hebra_atom),
            hebra5(funcion_hebra_atom),
            hebra6(funcion_hebra_atom);
// las esperamos
        hebra4.join();
        hebra5.join();
        hebra6.join();
    
    cout << "Valor esperado es de 3000000" << endl;
    cout << "Valos const no atom: " << const_no_atom << endl;
    cout << "Valor const atom: " << const_atom;

}