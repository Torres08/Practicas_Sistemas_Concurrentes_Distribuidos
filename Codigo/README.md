# Índice

## Torres Ramos, Juan Luis

## Tema 1

### Seminario 1

- Ejercicio1: Ejemplo mal hebras
- Ejercicio2: Uso de hebras con join
- Ejercicio3: Uso variables locales y hebras
- Ejercicio4: Parametros por salida, usar 2 hebras y una sola funcion; ref() 
- Ejercicio5: Uso de futures con async
- Ejercicio6: Ejemplo de uso de vectores de hebras
- Ejercicio7: Uso de array de futures con async
- Ejercicio8: Medir Tiempo con chrono
- Ejercicio9: Calculo de Integral
- Ejercicio10: Variables Atomicas
- Ejercicio11: Uso de cerrojos mutex
- Ejercicio12: Ejemplo de problema de prod, cons (varias lecturas y escrituras)

### Practicas 1:

- Practica0: Generar numero aleatorios Tirar dados y esperar con sleep_for
- Practica1: Problema del productor-consumidor
- Practica2: Problema de los fumadores

## Tema 2

### Seminario 2

- Ejercicio1: Ejemplo de Monitor sin EM 
- Ejercicio2: Ejemplo de Monitor con EM con cerrojos
- Ejercicio3: Ejemplo de Monitor con EM con guardas
- Ejercicio4: Monitor Barrera simple, con cola condicion monitorSC condition_variable
- Ejercicio5: Monitor Barrera parcial, no espera a toda las hebras, si no a un subconjunto
- Ejercicio6: Monitor Barrera SC Prod cons
- Ejercicio7: Monitor Barrera SC Prod cons multiple
- Ejercicio8: Monitor Barrera Parcial SU
- Ejercicio9: Produ/Cons SU


### Practicas 2:

- Practica1: Fumadores con monitor SU
- Practica2: Lectores y Escritores con monitor SU


## Tema 3

#### Seminario 3

- Ejercicio1: holamundo con MPI basico
- Ejercicio2: Ejemplo de MPI emisor y receptor sincrono
- Ejercicio3: Escribes un numero, varios procesos los leen, uso while emisor, receptor
- Ejercicio4: Intercambio Sincrono Ejemplo Vecinos Ssend y Recv
- Ejercicio5: Vecinos se saludan, mandar cadenas char
- Ejercicio6: Proceso donde emisores mandan cadena de texto y el receptor las lee, voy al baño
- Ejercicio7: Uso MPI_Iprobe para sondeo no bloqueante , se reciben mensajes hasta rellenar un contador, el emisor las lee y actualiza
- Ejercicio8: Vecinos V2, intercambio no_bloqueante, con eventos asincronos, tickets


## Practicas 3:

- Practica 1: Productor Consumidor con buffer acotado. Solucion distribuida
- Practica 2: Prod Cons con bufferr acotado. Sol seleccion no determinista
- Practica 3: Varios Productores y consumidores. buffer FIFO orden arbitrario
- Practica 4: Filosofos sin camarero sin interbloqueo
- Practica 5: Filosofos con camarero ESpera selectiva Etiquetas
