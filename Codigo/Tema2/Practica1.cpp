// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// archivo: barrera1_sc.cpp
// Monitores SU
//
// -----------------------------------------------------------------------------

#include "scd.h"
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unistd.h>

using namespace std;
using namespace scd;

// -----------------------------------------------------------------------------
// Variables Globales

const int num_fumadores = 3;

// -----------------------------------------------------------------------------
// MOnitor SU

class Estanco : public HoareMonitor {

private:
  CondVar mostrador, clientes[num_fumadores]; // mis hebras
  int ingrediente_mostrador;

public:
  Estanco();
  void ponerIngrediente(int x);
  void ObtenerIngrediente(int i);
  void esperaRecogerIngrediente();
};

// -----------------------------------------------------------------------------
// constructor

Estanco::Estanco() {
  for (int i = 0; i < num_fumadores; i++) {
    clientes[i] = newCondVar();
  }
  mostrador = newCondVar();
  ingrediente_mostrador = -1;
}

// -----------------------------------------------------------------------------
// Funciones Monitor
void Estanco::ponerIngrediente(int x) {
  ingrediente_mostrador = x;
  cout << "\nIngrediente " << x << " en el mostrador" << endl;
  //cout << "\nMostrador lleno";
  clientes[x].signal();
}

void Estanco::ObtenerIngrediente(int i) {
  if (ingrediente_mostrador != i) {
    clientes[i].wait();
  }

  cout << "\nSe retira el ingrediente " << i << " del mostrador." << endl;
  //cout << "\nMostrador Vacio" << endl;

  ingrediente_mostrador = -1;

  mostrador.signal();
}

void Estanco::esperaRecogerIngrediente() {
  if (ingrediente_mostrador != -1)
    mostrador.wait();
}

// -----------------------------------------------------------------------------
// funciones

// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

// -----------------------------------------------------------------------------
// FUnciones Hebras

void funcion_hebra_estanquero(MRef<Estanco> estanco) {
  //for (int i = 0; i < 10; i++){
  while (true) {
    int ingrediente = producir_ingrediente();
    estanco->ponerIngrediente(ingrediente);
    // cout <<"\nEn el mostrador esta el ingrediente: "<<ingrediente;
    // ingrediente_mostrador=ingrediente;
    // Se le hace un signal al fumador que necesite ese ingrediente
    // sem_signal(ingr_disp[ingrediente_mostrador]);
    estanco->esperaRecogerIngrediente();
  }
}

void funcion_hebra_fumador(MRef<Estanco> estanco, int num_fum) {
   //for (int i = 0; i < 10; i++){
  while (true) {
    estanco->ObtenerIngrediente(num_fum);
    fumar(num_fum);
  }
}
// -----------------------------------------------------------------------------
// main

int main() {

  MRef<Estanco> estanco = Create<Estanco>();
  thread hebra_estanco(funcion_hebra_estanquero, estanco);
  thread clientes[3];

  for (int i = 0; i < 3; ++i)
    clientes[i] = thread(funcion_hebra_fumador, estanco, i);
  hebra_estanco.join();

  for (int i = 0; i < 3; ++i)
    clientes[i].join();
  return 0;
}