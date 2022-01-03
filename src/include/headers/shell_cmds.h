/**
 * @file shell_cmds.h
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Header de librería con funciones útiles para ejecutar
 *        los comandos ingresados a la shell de los laboratorios
 *        #4 y #5.
 * @version 2.3
 * @since 2021-10-17
 */

#ifndef __SHELL_CMDS__
#define __SHELL_CMDS__

/* ---------- Librerías a utilizar -------------- */

#include "shell_utilities.h"
#include <sys/prctl.h>
#include <sys/wait.h>

/* ---------- Prototipado de funciones ---------- */

void cmd_cd(char *);
void cmd_clr(void);
void cmd_exec(void);

bool cmd_echo(char *, bool);
bool read_line(char *);

#endif