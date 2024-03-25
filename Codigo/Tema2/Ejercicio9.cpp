// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: barrera1_sc.cpp
// Monitores SU
//
// -----------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <random>
#include <thread>
#include "scd.h"

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables Globales
const int tam_vec = 10;
const int tam_productoras = 4;
const int tam_consumidoras = 5;
const int num_iteraciones = 5;

int num_items = 40;


// -----------------------------------------------------------------------------
// Monitor

class monitorSU: public scd::HoareMonitor{
private:
    CondVar libre, ocupada;
    int buffer[tam_vec];
    int primera_libre;

public:
    monitorSU();
    void escribe(int num_productor, int dato);
    int lee(int posicion);
};

// -----------------------------------------------------------------------------
// Constructor
monitorSU::monitorSU(){
    libre = newCondVar();
    ocupada = newCondVar();
    primera_libre= 0;

}
// -----------------------------------------------------------------------------
// Funcion Monitor

void monitorSU::escribe(int num_productor, int dato){

    if (primera_libre == num_items)
        libre.wait(); // esta lleno no quedan hueco libres
    
    const int valor = dato;

    cout << "Introducido el dato = " << valor << " por el productor " << num_productor << endl;

    // LIFO
    buffer[primera_libre] = dato;
    primera_libre ++;

    ocupada.signal(); // he rellenado una poosicion

}

int monitorSU::lee(int posicion){
    int resultado;

    if (primera_libre == 0)
        ocupada.wait(); // si esta vacia ocupadas no puede continuar
    
    // FIFO
    --primera_libre;
    int valor = buffer[primera_libre];

    cout << "       Consumido: " << valor << " por el consumidor " << posicion << endl;
    libre.signal(); // he quitado un hueco de mi buffer, he de comunicarlo 

    return resultado;
}

// -----------------------------------------------------------------------------
// Funciones
int producir_dato (){
    int resultado = rand() % 10;// 1- 10
    return resultado;
}
// -----------------------------------------------------------------------------
// funciones hebras
void funcion_hebra_productora(MRef<monitorSU> monitor, int num_hebra){
    //for (int i = 0; i < num_items; i++){
    while(true){
        int dato = producir_dato(); // produzco un dato , random del uno al 10
        monitor->escribe(num_hebra, dato);
    }
}

void funcion_hebra_consumidora(MRef<monitorSU> monitor, int num_hebra){
    //for (int i = 0; i < num_items; i++){
     while(true){
        int dato = monitor->lee(num_hebra); // la lectura se hace en la propia funcion
        //cout << "Iteracion: " << i << endl;
        cout << "---------------------------------------" << endl;
    }
}
// -----------------------------------------------------------------------------
// main

int main (){

    MRef<monitorSU> Monitor = Create<monitorSU>();

    thread hebras_productoras[tam_productoras],
            hebras_comsumidoras[tam_consumidoras];
    
     for (int i = 0; i < tam_productoras; i++){
        hebras_productoras[i] = thread(funcion_hebra_productora, Monitor, i);
    }

    for (int i=0; i< tam_consumidoras; i++){
        hebras_comsumidoras[i] = thread(funcion_hebra_consumidora,Monitor,i);
    }


    // espero


    for (int i = 0; i < tam_productoras; i++){
        hebras_productoras[i].join();
    }

    for (int i=0; i< tam_consumidoras; i++){
        hebras_comsumidoras[i].join();
    }

}