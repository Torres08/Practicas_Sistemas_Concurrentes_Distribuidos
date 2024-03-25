
# Makefile Plantilla


#Compilar: mpicxx -std=c++11 -o malafolla malafolla.cpp
#Ejecutar: mpirun -oversubscribe -np 11 malafolla > malafolla.txt
#Añadir --mca opal_warn_on_missing_libcuda 0 si no tienes tarjeta Nvidia


# ************ Compilación y Ejecuta ************

Holamundo:
	mpicxx -std=c++11 -o Holamundo Holamundo.cpp
	mpirun -oversubscribe -np 8 ./Holamundo

Ejercicio1:
	mpicxx -std=c++11 -o Ejercicio1 Ejercicio1.cpp
	mpirun -oversubscribe -np 8 ./Ejercicio1

Ejercicio2:
	mpicxx -std=c++11 -o Ejercicio2 Ejercicio2.cpp
	mpirun -oversubscribe -np 10 ./Ejercicio2
	

# ************ Practicas ************

Practica1:
	mpicxx -std=c++11 -o Practica1 Practica1.cpp
	mpirun -oversubscribe -np 11 ./Practica1

Practica2:
	mpicxx -std=c++11 -o Practica2 Practica2.cpp
	mpirun -oversubscribe -np 11 ./Practica2

# ************ Compilar ************

CompilaHolamundo:
	mpicxx -std=c++11 -o Holamundo Holamundo.cpp
	@echo "Compila, limpio ..."
	@rm Holamundo

Compila:
	mpicxx -std=c++11 -o Practica1 Practica1.cpp
	@echo "Compila, limpio ..."
	@rm Practica1


# ************ Limpieza ************
clean :
	@echo "Limpiando "
	@rm Ejercicio1 Ejercicio2 Holamundo
	@echo "Archivos borrados con exito"
	

cleanp:

	@echo "Limpiando "
	@rm Practica1 Practica2 
	@echo "Archivos borrados con exito"

autor:
	@echo "Torres Ramos, Juan Luis"
