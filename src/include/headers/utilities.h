/**
 * @file utilities.h
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Header de librería con funciones útiles generales.
 * @version 3.7.4
 * @since 2021-08-18
 */

#ifndef __UTILITIES__
#define __UTILITIES__

/* ---------- Librerías a utilizar -------------- */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ---------- Definición de constantes ---------- */

// COLORES DE PRIMER PLANO (LETRAS)
#define _FG_COLOR_GREEN_ "\x1b[32m"
#define _FG_COLOR_YELLOW_ "\x1b[33m"
#define _FG_COLOR_BLUE_ "\x1b[34m"
#define _FG_COLOR_WHITE_ "\x1b[37m"
#define _FG_COLOR_RESET_ "\x1b[39m"

// COLORES DE SEGUNDO PLANO (FONDO)
#define _BG_COLOR_WHITE_ "\x1b[47m"
#define _BG_COLOR_LGRAY_ "\x1b[100m"
#define _BG_COLOR_RESET_ "\x1b[49m"

// FORMATOS DE TEXTO
#define _FORMAT_BOLD_ "\033[1m"
#define _FORMAT_RESET_ALL_ "\033[0m"

// MISCELÁNEOS
#define _CLR_SCR_ "\033[H\033[J"

#define _STR_MAX_LEN_ 509
#define _HOST_MAX_LEN_ 253

#define _NORM_ERR_ 0
#define _FATAL_ERR_ 1

/* ---------- Declaración de variables ---------- */

int original_stdin, original_stdout, new_stdin, new_stdout;

char new_stdin_name[_STR_MAX_LEN_];
char new_stdout_name[_STR_MAX_LEN_];

bool redirect_stdin_flag, redirect_stdout_flag;

/* ---------- Prototipado de funciones ---------- */

void restore_stdx(int);
void show_err(char *, int);
void try_kill(int, int);
void try_write(int, char *);

char *chop_to_char(char *, char *);
char *mk_err_msg(char *, int);
char *strtrim(char *);

bool open_file_r(FILE **, char *);
bool redirect_stdx(char *, int);
bool stremp(char *);

#endif