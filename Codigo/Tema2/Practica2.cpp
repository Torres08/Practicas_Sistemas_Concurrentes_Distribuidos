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

/*
    Escritores: Procesos que modifican la estructura de datos. No se puede
   ejecutar concurrentemente con optros procesos.

    Lectores : Procesos que leen la estructura de datos, pero no modifican su
   estado en absoluto., se puede ejecutar concurrentemente con otro proceso
*/

// -----------------------------------------------------------------------------
// Variables Globales
const int num_lectores = 10;
const int num_escritores = 10;

// -----------------------------------------------------------------------------
// Monitor
class MonitorLE : public HoareMonitor {
private:
  bool escrib; // true si un escritor esta escribiendo, false si no hay
               // escritores escribiendo
  int n_lec;   // numero de lectores que están escribiendo en un momento dado

  CondVar lectura; // se usa por los lectores (ini_lectura) para esperar cuando
                   // hay un escritor escribiendo (escrib == true)
  CondVar escritura; // se usa por los escritores (ini_escritura) para esperar
                     // cuando hay otro escritor escribiendo (escrib == true) o
                     // bien hay lectores leyendo n_lec > 0

  // estas condiciones cumple que not escrib or n_lec == 0
  // si se termina de ller  -> signal

public:
  MonitorLE();
  void ini_lectura();
  void fin_lectura();
  void ini_escritura();
  void fin_escritura();
};

// -----------------------------------------------------------------------------
// Constructor
MonitorLE::MonitorLE() {
  escrib = false;
  n_lec = 0;
  lectura = newCondVar();
  escritura = newCondVar();
};

// -----------------------------------------------------------------------------
// Funciones Monitor
void MonitorLE::ini_lectura() {
  if (escrib)
    lectura.wait(); // si se esta escribiendo, no se pede hacer otra cosa

  n_lec += 1; // incremento el nº lectores

  // como he incrementado, tmmb llamo
  lectura.signal();
}

void MonitorLE::fin_lectura() {
  // terminamos una lectura, decrementamos
  n_lec -= 1;

  // si vemos que es el ultimo lector, desbloqueamos un escritor y que escriba
  if (n_lec == 0)
    escritura.signal();
}

void MonitorLE::ini_escritura() {
  // si vemos que hay otros procesos esperamos la escritura se hace uno a uno
  if (escrib || n_lec > 0)
    escritura.wait();

  escrib = true; // podemos escribir
}

void MonitorLE::fin_escritura() {
  escrib = false; // hemos terminado de leer

  // si hay lectores, despertamos a uno, si no despertamos a un escritor
  if (!lectura.empty()) {
    lectura.signal();
  } else {
    escritura.signal();
  }
}

// -----------------------------------------------------------------------------
// Funciones
void Espera() {
  chrono::milliseconds duration(aleatorio<100, 200>());
  this_thread::sleep_for(duration);
}

void escribir(int escritor) {
  chrono::milliseconds duration_escrib(aleatorio<10, 100>());

  cout << "El escritor " << escritor << " comienza a escribir" << endl;

  // espera bloqueada por un tiempo duracion_escrib
  this_thread::sleep_for(duration_escrib);

  cout << "Escritor " << escritor << " ha terminado." << endl;
}

void leer(int lector) {
  chrono::milliseconds duration_lect(aleatorio<10, 100>());

  cout << "El lector " << lector << " comienza a leer" << endl;

  this_thread::sleep_for(duration_lect);

  cout << "Lector " << lector << " ha terminado." << endl;
}
// -----------------------------------------------------------------------------
// Funciones hebras

void funcion_hebra_lectores(MRef<MonitorLE> monitor, int num_lect) {
  while (true) {
    Espera();
    monitor->ini_lectura();
    leer(num_lect);
    monitor->fin_lectura();
  };
}

void funcion_hebra_escritores(MRef<MonitorLE> monitor, int num_escrit) {
  while (true) {
    Espera();
    monitor->ini_escritura();
    escribir(num_escrit);
    monitor->fin_escritura();
  }
}

// -----------------------------------------------------------------------------
// main

int main() {

  // Inicio
  MRef<MonitorLE> monitor = Create<MonitorLE>();
  thread lectores[num_lectores], escritores[num_escritores];
  for (int i = 0; i < num_escritores; i++)
    escritores[i] = thread(funcion_hebra_escritores, monitor, i);

  for (int i = 0; i < num_lectores; i++)
    lectores[i] = thread(funcion_hebra_lectores, monitor, i);

  for (int i = 0; i < num_lectores; i++)
    lectores[i].join();

  for (int i = 0; i < num_escritores; i++)
    escritores[i].join();
}