# author: Bonino, Francisco Ignacio.
# version: 2.7
# since: 2021-10-04

# Flags a utilizar.
CC = gcc
CFLAGS = -Wall -Werror -pedantic
SLIBF = ar rcs

all: myshell # En caso de ejecutar 'make' sin argumento, se aplica el target indicado.

# Directorios donde se guardarán los archivos.
build_folders:
	mkdir -p ./obj ./slib ./bin ./bfs ./redir/stdin ./redir/stdout

# Librería estática propia: utilities.
lib_utilities.a: utilities.o
	$(SLIBF) slib/lib_utilities.a obj/utilities.o

utilities.o: src/include/bodies/utilities.c src/include/headers/utilities.h
	$(CC) $(CFLAGS) -c src/include/bodies/utilities.c -o obj/utilities.o

# Librería estática propia: shell_utilities.
lib_shell_utilities.a: shell_utilities.o
	$(SLIBF) slib/lib_shell_utilities.a obj/shell_utilities.o

shell_utilities.o: src/include/bodies/shell_utilities.c src/include/headers/shell_utilities.h
	$(CC) $(CFLAGS) -c src/include/bodies/shell_utilities.c -o obj/shell_utilities.o

# Librería estática propia: shell_cmds.
lib_shell_cmds.a: shell_cmds.o
	$(SLIBF) slib/lib_shell_cmds.a obj/shell_cmds.o

shell_cmds.o: src/include/bodies/shell_cmds.c src/include/headers/shell_cmds.h
	$(CC) $(CFLAGS) -c src/include/bodies/shell_cmds.c -o obj/shell_cmds.o

# Ejecutable final.
myshell: build_folders myshell.o lib_utilities.a lib_shell_utilities.a lib_shell_cmds.a
	$(CC) $(CFLAGS) -o bin/myshell obj/myshell.o slib/lib_utilities.a slib/lib_shell_utilities.a slib/lib_shell_cmds.a

myshell.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/myshell.o

# Limpieza de archivos y carpetas creados
clean:
	rm -r ./bfs
	rm -r ./bin
	rm -r ./obj
	rm -r ./redir
	rm -r ./slib