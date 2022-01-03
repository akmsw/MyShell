/**
 * @file shell_utilities.h
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Header de librería con funciones útiles particulares
 *        para los laboratorios #4 y #5.
 * @version 2.9
 * @since 2021-10-05
 */

#ifndef __SHELL_UTILITIES__
#define __SHELL_UTILITIES__

/* ---------- Librerías a utilizar -------------- */

#include "utilities.h"
#include <malloc.h>
#include <regex.h>

/* ---------- Definición de constantes ---------- */

#define _HELP_ -3
#define _WELC_ -2
#define _FARE_ -1
#define _EXPS_ 0
#define _STD_ 1
#define _ABS_ 2
#define _REL_ 3

/* ---------- Declaración de variables ---------- */

int args_amount, prog_type;

char input[_STR_MAX_LEN_];
char aux_input[_STR_MAX_LEN_];

char *base_cwd; // CWD base de la ubicación del proyecto - para redirecciones
char *pipe_cmd;
char *prog_name;
char *prog_path; // Directorio actual de búsqueda

char **prog_args;

bool pipe_flag, bg_flag, cd_flag,
     clr_flag, echo_flag, quit_flag;

regex_t bg_regex, cd_regex, clr_regex,
        echo_regex, pipe_regex, quit_regex;

/* ---------- Prototipado de funciones ---------- */

void mk_prompt(char *, char *, char **);
void print_hmsg(int);
void show_help(int);

int set_flags(char *);

bool get_prog_data(char *);

#endif