// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Seminario 1. Programación Multihebra y Semáforos.
//
// Ejemplo 9 (ejemplo9.cpp)
// Calculo concurrente de una integral. Plantilla para completar.
//
// -----------------------------------------------------------------------------

#include <chrono>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

// -----------------------------------------------------------------------------
// Variables Locales
const long m = 1024l * 1024l *
               1024l, // número de muestras (del orden de mil millones)
    n = 4;            // número de hebras concurrentes (divisor de 'm')

// -----------------------------------------------------------------------------
// Funciones

// evaklua la integral de forma secuencial, devuelve el resultado
double f(double x) { return 4.0 / (1.0 + x * x); }

double calcular_integral_secuencial(){
  double suma = 0.0;           // inicializar suma
  for (long j = 0; j < m; j++) // para cada $j$ entre $0$ y $m-1$:
  {
    const double xj = double(j + 0.5) / m; //      calcular $x_j$
    suma += f(xj); //      añadir $f(x_j)$ a la suma actual
  }
  return suma / m; // devolver valor promedio de $f$
}

// -----------------------------------------------------------------------------
// Funciones Hebras

// funcion que ejecuta cada hebra
double funcion_hebra( long i){
    double SumaParcial = 0.0;

    for ( int j = i; j < m ; j++){
        const double z = double(i+0.5)/m;
        SumaParcial += f(z);
    }

    return SumaParcial;
}


// aqui uso vector de futures para calcular como en la integral
// n = 4
double calcular_integral_concurrente(){

    // declaramos
    future<double> futuros[n];

    // inicializamos
    for (int i = 0; i < n; i++)
        futuros[i] = async(launch::async, funcion_hebra, i);
    
    double resultado;
    // hacemos get aacumulandolo en resultado

    for (int i = 0; i
    < n; i++)
        resultado += futuros[i].get();
    
    return resultado;


}

// -----------------------------------------------------------------------------
// main

int main() {




  time_point<steady_clock> inicio_sec = steady_clock::now();
  const double result_sec = calcular_integral_secuencial();
  time_point<steady_clock> fin_sec = steady_clock::now();
  double x = sin(0.4567);
  time_point<steady_clock> inicio_conc = steady_clock::now();
  const double result_conc = calcular_integral_concurrente();
  time_point<steady_clock> fin_conc = steady_clock::now();
  duration<float, milli> tiempo_sec = fin_sec - inicio_sec,
                         tiempo_conc = fin_conc - inicio_conc;
  const float porc = 100.0 * tiempo_conc.count() / tiempo_sec.count();

  constexpr double pi = 3.14159265358979323846l;

  cout << "Número de muestras (m)   : " << m << endl
       << "Número de hebras (n)     : " << n << endl
       << setprecision(18) << "Valor de PI              : " << pi << endl
       << "Resultado secuencial     : " << result_sec << endl
       << "Resultado concurrente    : " << result_conc << endl
       << setprecision(5) << "Tiempo secuencial        : " << tiempo_sec.count()
       << " milisegundos. " << endl
       << "Tiempo concurrente       : " << tiempo_conc.count()
       << " milisegundos. " << endl
       << setprecision(4) << "Porcentaje t.conc/t.sec. : " << porc << "%"
       << endl;
}