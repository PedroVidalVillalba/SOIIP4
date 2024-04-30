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

# Fuentes
SRC = productor_FIFO.c consumidor_FIFO.c

# Ejecutables o archivos de salida
OUT = $(SRC:.c=)

############
#- REGLAS -#
############

# Regla por defecto: compila todos los ejecutables
all: $(OUT)

# Genera los ejecutables
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDES)

# Borra todos los resultados de la compilación (prerrequisito: cleanobj)
clean:
	rm -f $(OUT)
