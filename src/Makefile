# Nombre del archivo ejecutable
TARGET = nombre_del_ejecutable

# Archivos fuente
SRCS = implementacion.c cliente.c servidor.c claves.c

# Flags de compilación
CFLAGS = -Wall -Wextra -pedantic -pthread

# Comando de compilación
CC = gcc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
