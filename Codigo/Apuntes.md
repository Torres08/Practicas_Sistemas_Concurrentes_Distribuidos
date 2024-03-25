# Seminario 3

## 1. Message Passing Interface MPI

MPI es un estándar que define una API para la programación paralela mediante paso de mensajes. Implica un número fijo de procesos que se comunican mediante llamadas a funciones de envío y recepción de mensajes, y puede operar en dos modelos: single y multiple Program Data (SPMD y MPMD).

`#include <mpi.h>`, las funciones devuelven un código de error, tiene sus propias funciones y datos

## 2. Compilación y ejecución de programas MPI

OpenMPI es una implementación portable del estándar MPI-2, utilizada para compilar y ejecutar programas MPI. Se compila con `mpicxx` y se ejecuta con `mpirun`, donde se pueden especificar el número de procesos y otras opciones como el archivo de máquina y la sobreasignación de procesadores.

```bash
    mpicxx -std=c++11 -o ejemplo_mpi_exe Ejemplo
```

```bash
    mpirun -oversubscribe -np 4 ./ejemplo_mpi_exe
```

- np: define cuantos procesos ejecutara el programa, en este caso -> 4 procesos del ejemplo_mpi_exe
- machinefile : Si NO se indica, lanza los 4 procesos sobre el mismo ordenador
- oversubscribe: es necesaria si el nº de procesadores disponible es inferior al numero de proceso que se lanzan

## 3. Funciones MPI básicas

- `MPI_Init`: inicializa el entorno de ejecucion de MPI
- `MPI_Finalize`: finaliza entorno
- `MPI_Comm_size`: determina el numero de procesos de un comunicador
- `MPI_Comm_rank`: determina el identificador del proceso en un comunmicador
- `MPI_Send`: Operacion basica para enviar mensaje
- `MPI_Recv`: Operacion basica para recepcion de un mensaje

    ```bash
    int MPI_Init(int *argc, char ***argv)
    int MPI_Finalize()
    int MPI_Comm_size(MPI_Comm comm, int *size) -> si relleno (MPI_COM_WORLD, &num_procesos) -> devuelve el total de proceso de la aplicacion en num_procesos
    int MPI_Comm_rank(MPI_Comm comm, int *rank) -> si relleno (MPI_COM_WORLD, int &id_propio) -> devuelve el id propio 
    ```

    `comunicador MPI`: variable MPI_Comm. Formado por:
      - Grupo de proceso: subconjunto de proceso(pueden ser todo)
      - Contexto de comunicacion: ambito de paso de mensaje en el que se comunican los procesos -> Un mensaje enviado en un contexto sólo puede ser recibido en dicho contexto
                                   Todas las funciones de comunicacion de MPI necesitan como argumento un comunicador
    
    `MPI_COM_WORLD` -> comunicador univesal, inflye en todos los procesos lanzados


    ### Enviar y recibir mensajes:
    Un proceso puede enviar umn mensaje con MPI_Send 

    - `int MPI_Send((void *buf_emi, int num,MPI_Datatype datatype, int dest int tag, MPI_Commm)`
      - Envia los datos ( num elementos de tipo datatype almacenados a partir de buf_emi) al proceso dest dentro del comunicador comunicador
            - `tag`: se transfiere con el mensaje, califica el mensaje en funcion de sus etiquetas , es un entero no negativo
      - Se inplementa un envio asincrono seguro, podemos escribir otra vez en el buf_emi
    
    Un proceso puede recibir un mensaje con MPI_Recv

    - `int MPI_Recv (void *buf_rec, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)`

      - espera hasta recibir un mensaje del proceso source, dentro del comuncador comm con la etiqueta tag, escribe los datos -> buf_rec
        - Se puede utilizar valores especiales o comodin
            - si source es `MPI_ANY_SOURCE` : puede recibir cualquier mensaje desde cualquier proceso del comunicador
            - si tag es `MPY_ANY_TAG`: puede recibir un mensaje con cualquier etiqueta

    Los datos se copian desde buf_emi hacia buf_rec

    num y datatype determina la longitud del buff

    objeto status lo forma
    - campo MPI_SOURCE 
   - etiqueta MPI_TAG
    
    Para obtener la cuenta de valores recibidos usamos status

    `int MPI_get_count(MPI_Status *status, MPI_Datatype dtype, int *num)`
  
    - escribe el entero apuntado por num en una llamada MPI_Recv previa, hay que dar el datatype


    ### Emparejar operaciones de envio y recibo 
    
    Una operacion de envio con etiqueta e realizado por un emisor encaja con una operacion de recepcion B de un receptor si:
    - A nombra a B como recpetor y e como etiqueta
    - B especifina MPI_ANY_SOURCE, o nombra a A como emisor
    - B especifica MPI_ANY_TAG, o nombra la etiqueta emisor
    
    Si se asocian varias operaciones de envio con una de recepcion  
    - la operacion de recepcion coge la primera inmicializada
    - no se tiene en cuenta el tipo de dato ni la cuenta de items
    
    EL programador ha de asegurarse que en el receptor  
    - los bytes transferidos son equivalentes al de los datos
    - se sabe cuantos items de datos se han recibido
    - Se ha reservado memoria suficiente


## 4. Paso de mensajes síncronos en MPI

Forma de enviar mensajes sincronos (siempre seguro)

`int MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);`

- Usa los mismos valores que MPI_Send
- Inicia un envio, lee los datos y esoera ek inicio de recepcion
- Sincrono y seguro
- MPI_Recv para recibir

Ejemplo intercambio sincrono:

Tenemos parejas de procesos, cada proceso envia un dato a su compañero usando MPI_Ssend
- es decir ambos se hacen un envio, 
- "No se produce interbloqueo" -> los procesos pares hacen envios seguidos de recepcion y los procesos impares hacen recepcion seguida del envio


## 5. Sondeo de mensajes

MPI incorpora 2 funciones que permiten a un proceso receptor averiguar si hay algun mensaje pendiente por recibir, y en ese caso obtener sus metadatos

con estas funciones 
- no recibimos el mensaje
- podemos restringir a mensajes de un emisor
- podemos restringir a mensajes con una etiqueta
- Cuando hay mensaje -> permite obtener los metadatos( emisor, etiqueta, numero de items)
    
`MPI_Iprobe` (Consulta si hay mensaje pendiente en el momento)
`MPI_Probe` (espera bloqueada hasta que haya un mensaje)

`MPI_Probe ( int source, int tag, MPI_Comm, MPI_Status *status);` 


`MPI_Iprobe( int source, int tag, MPI_Comm, int *flag, MPI_Status *status)`
        -> El entero apuntado por flags será mayor que 0 solo si hay algun mensaje enviado al proceso que llama, si no hay mensajes -> tag es 0

## 6. Comunicación insegura

MPI ofrece 
- operaciones inseguras (Operaciones asincronas)
- permten el inicio de una operacion de envio o recepcion, y luego el emisor puede continuar concurrentemente con la transmision
    
`MPI_Isend`: inicia envio pero retorna ante de leer el buffer
`MPI_Irecv`: Inicia recepcion pero retorna antes de escribir

Tenemos que comprobar si la operacion ha terminado o no

`MPI_Wait` : espera bloqueada hasta que acabe el envio/recepcion
`MPI_Test` Comprueba si el envio o recepcion ha finalizado (no hay espera bloqueante)

`MPI_Request *request` -> es un ticket -> con el identificamos la operacion cuyo resultado se pretende usar
no hay *status en Isend o I recv -> obtenemos status con las operaciones de Consulta

CUANDO NO SE VAYA A USAR UNA VARIABLE MPO_Request -> LIBERAMOS memoria `MPI_Request_free`


`MPI_test:` COmprubea la operacion identificada por un ticket y escribe en el flag un nº > 0si ha acabado, si no 0.

`MPI_Wait:` Espera bloqueada hasta que termine una operacion

EN ambas funciones, al terminar la operacion
- podemos usar objeto status para consultar los metadatos
- la memoria por request es liberada automaticamente

Operaciones inseguras
- Permiten simultanear trabajo util del emisor y/o receptor con la lectura, transmision y recepcion del mensaje
- Aumenta paralelismo, pero es mas complicado de usar
    
Con MPI_Wait y MPI_Test permiten cuando es seguro, el momento, de vovler a usar el buffer de envio o recepcion ya que nos dicen cuando 
- se ha leido y copiado los datos del buffer del emisor
- se han recibido los datos en el buffer del receptor
    
Una operacion insegura puede ir con una segura

Ejemplo Intercambio_nobloq.cpp
