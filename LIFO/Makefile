###############
#- VARIABLES -#
###############

# Compilador y opciones de compilación
CC = gcc
CFLAGS = -Wall -Wpedantic -g -lrt

# Carpeta con las cabeceras
HEADERS_DIR = .

# Opción de compilación que indica dónde están los archivos .h
INCLUDES = -I$(HEADERS_DIR)

# Archivos de cabecera para generar dependencias
HEADERS = $(HEADERS_DIR)/params.h

# Fuentes con las funcionalidades básicas del stack, del productor/consumidor y el array (implementaciones de los .h)
COMMON = $(HEADERS:.h=.c)

# Fuentes
SRC = productor_LIFO.c consumidor_LIFO.c

# Objetos
OBJ = $(SRC:.c=.o)

# Ejecutables o archivos de salida
OUT = $(SRC:.c=)

############
#- REGLAS -#
############

# Regla por defecto: compila todos los ejecutables
all: $(OUT)

# Genera los ficheros objeto .o necesarios, dependencia de sus respectivos .c y todas las cabeceras.
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)

# Genera el ejecutable del ejercicio 1, dependencia de su objeto y los objetos comunes.
%: %.o
	$(CC) $(CFLAGS) -o $@ $^

# Borra todos los resultados de la compilación (prerrequisito: cleanobj)
clean: cleanobj
	rm -f $(OUT)

# Borra todos los ficheros objeto del directorio actual y todos sus subdirectorios
cleanobj:
	find . -name "*.o" -delete
