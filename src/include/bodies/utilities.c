/**
 * @file utilities.c
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Librería con funciones útiles generales.
 * @version 3.7.4
 * @since 2021-08-18
 */

#include "../headers/utilities.h"

/**
 * @brief Esta función restaura el stream original del proceso.
 *        Debe ser utilizada luego de una llamada a la función
 *        redirect_stdx.
 *
 * @param stdx Qué stream se desea restaurar.
 */
void restore_stdx(int stdx)
{
    if (stdx == STDIN_FILENO)
    {
        if (dup2(original_stdin, STDIN_FILENO) == -1)
            show_err("Could not restore original STDIN stream", _FATAL_ERR_);

        if (close(original_stdin) == -1)
            show_err("Could not close file descriptor when restoring original STDIN stream", _FATAL_ERR_);

        redirect_stdin_flag = false;
    }
    else
    {
        if (dup2(original_stdout, STDOUT_FILENO) == -1)
            show_err("Could not restore original STDOUT stream", _FATAL_ERR_);

        if (close(original_stdout) == -1)
            show_err("Could not close file descriptor when restoring original STDOUT stream", _FATAL_ERR_);

        redirect_stdout_flag = false;
    }
}

/**
 * @brief Esta función muestra el mensje solicitado en pantalla
 *        y termina la ejecución del programa si es necesario.
 *
 * @param msg Mensaje a mostrar.
 * @param err_type Gravedad del error.
 */
void show_err(char *msg, int err_type)
{
    try_write(STDERR_FILENO, mk_err_msg(msg, err_type));

    if (err_type == _FATAL_ERR_)
        exit(EXIT_FAILURE);
}

/**
 * @brief Esta función se encarga de intentar matar un proceso
 *        especificado mediante su ID. Si no se logra, se
 *        imprime un mensaje de error y se aborta el programa.
 *
 * @param pid ID del proceso a matar.
 * @param signal Señal a enviar al proceso.
 */
void try_kill(int pid, int signal)
{
    if (kill(pid, signal) == -1)
        show_err("Could not kill process", _FATAL_ERR_);
}

/**
 * @brief Esta función se encarga de intentar escribir un
 *        mensaje en pantalla de manera thread-safe.
 *        Si no se logra, se imprime un mensaje de error
 *        y se aborta el programa.
 *
 * @param stdx Stream en el que se escribirá el mensaje.
 * @param msg Mensaje a mostrar.
 */
void try_write(int stdx, char *msg)
{
    if (write(stdx, msg, strlen(msg)) == -1)
    {
        fprintf(stderr, "\nFatal error on write inscruction --- ABORTING\n");

        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Esta función se encarga de cortar la cadena hasta la primera
 *        coincidencia con el caracter especificado.
 *
 * @param str Cadena a recortar.
 * @param target Caracter utilizado como indicador para recortar la cadena.
 *
 * @returns La cadena recibida recortada hasta la primera coincidencia con
 *          el caracter especificado.
 */
char *chop_to_char(char *str, char *target)
{
    str[strcspn(str, target)] = '\0';

    return str;
}

/**
 * @brief Esta función se encarga de armar un mensaje de error.
 *
 * @details Se almacena memoria inicializada con ceros para evitar
 *          error con strcat. Se sobrescribe con el formato
 *          correspondiente para un mensaje de error y el mensaje
 *          que indica la falla, recibido por parámetro.
 *
 * @param msg Mensaje de error a mostrar.
 * @param err_type Gravedad del error.
 *
 * @returns El mensaje de error formateado con el texto recibido por parámetro.
 */
char *mk_err_msg(char *msg, int err_type)
{
    char *err_msg;

    if (err_type == _FATAL_ERR_)
    {
        // + 8 por "\nERROR ", + 7 por "[FATAL]", + 3 por " > " y + 4 por "\n\n"
        err_msg = (char *)calloc(strlen(_FORMAT_BOLD_) + strlen(_FG_COLOR_WHITE_) + (strlen(_FORMAT_RESET_ALL_) * 2) +
                                 strlen(msg) + sizeof(NULL) + (sizeof(char) * 22), sizeof(char));

        if (!err_msg)
        {
            fprintf(stderr, "\nFatal calloc error --- ABORTING\n");

            exit(EXIT_FAILURE);
        }

        strcat(err_msg, _FORMAT_BOLD_);
        strcat(err_msg, _FG_COLOR_WHITE_);
        strcat(err_msg, "\nERROR ");
        strcat(err_msg, _BG_COLOR_LGRAY_);
        strcat(err_msg, "[FATAL]");
    }
    else
    {
        // + 7 por "\nERROR", + 3 por " : " y + 4 por "\n\n"
        err_msg = (char *)calloc(strlen(_FORMAT_BOLD_) + (strlen(_FORMAT_RESET_ALL_) * 2) +
                                 strlen(msg) + sizeof(NULL) + 14, sizeof(char));

        if (!err_msg)
        {
            fprintf(stderr, "\nFatal calloc error --- ABORTING\n");

            exit(EXIT_FAILURE);
        }

        strcat(err_msg, _FORMAT_BOLD_);
        strcat(err_msg, _FG_COLOR_WHITE_);
        strcat(err_msg, "\nERROR");
    }

    strcat(err_msg, _FORMAT_RESET_ALL_);
    strcat(err_msg, _FG_COLOR_WHITE_);
    strcat(err_msg, " > ");
    strcat(err_msg, msg);
    strcat(err_msg, _FORMAT_RESET_ALL_);
    strcat(err_msg, "\n\n");

    return err_msg;
}

/**
 * @brief Esta función se encarga de quitar los
 *        espacios en los extremos de un string.
 *
 * @details Recorremos el string desde el comienzo hasta
 *          encontrar una letra. A partir de allí,
 *          recorremos el string desde el final hasta
 *          encontrar una letra. Antes de esta letra
 *          final, se coloca el caracter de fin de string.
 *          Si el string está completamente vacío, se lo
 *          retorna tal y como vino.
 *
 * @param str String a modificar.
 *
 * @returns El string sin espacios ni tabulaciones
 *          ni al comienzo ni al final.
 */
char *strtrim(char *str)
{
    while ((*str == ' ') || (*str == '\t'))
        str++;

    if (*str == 0)
        return str;

    char *end = str + strlen(str) - 1;

    while ((end > str) && ((*end == ' ') || (*end == '\t')))
        end--;

    end[1] = '\0';

    return str;
}

/**
 * @brief Esta función se encarga de abrir el archivo que queremos
 *        en modo lectura.
 *
 * @param file Archivo con el que se trabajará.
 * @param dir Directorio del archivo que queremos abrir.
 *
 * @returns Si se pudo abrir exitosamente el archivo.
 */
bool open_file_r(FILE **file, char *dir)
{
    *file = fopen(dir, "r");

    // Si hubo un error al leer el archivo, mostramos un mensaje de error y salimos del programa
    if (!*file)
    {
        char err_msg[_STR_MAX_LEN_];

        if (sprintf(err_msg, "Failed attempting to open file: %s", dir) < 0)
            show_err("Could not make error message", _FATAL_ERR_);

        show_err(err_msg, _NORM_ERR_);

        return false;
    }

    return true;
}

/**
 * @brief Esta función se encarga de redirigir un stream del
 *        proceso a un file descriptor especificado mediante
 *        su nombre. Esta función es útil para redirigir un
 *        stream hacia algún archivo, no para operaciones
 *        con pipe.
 *
 * @details Se abre un fd para el nuevo stream y se lo duplica
 *          al fd del stream correspondiente del proceso, y
 *          se cierra el fd abierto en primera instancia,
 *          almacenando una copia del mismo para una posterior
 *          restauración.
 *          Las flags utilizadas para abrir el nuevo stream de
 *          stdout indican que se abrirá el archivo en modo de
 *          sólo lectura y se creará si no existe. El nuevo
 *          stream de stdin se abrirá en modo de sólo lectura.
 *
 * @param new_stdx_name Nombre del file descriptor al cual
 *                      se va a redirigir el stream deseado.
 * @param stdx Qué stream se desea redirigir.
 *
 * @returns Si se ha podido redirigir correctamente el stream.
 */
bool redirect_stdx(char *new_stdx_name, int stdx)
{
    switch (stdx)
    {
        case STDIN_FILENO:
        {
            new_stdin = open(new_stdx_name, O_RDONLY, 0666);

            if (new_stdin == -1)
            {
                show_err("Could not redirect STDIN", _NORM_ERR_);

                return false;
            }

            original_stdin = dup(STDIN_FILENO);

            if (original_stdin == -1)
                show_err("Could not save original STDIN file descriptor", _FATAL_ERR_);

            if (dup2(new_stdin, STDIN_FILENO) == -1)
                show_err("Could not duplicate STDIN", _FATAL_ERR_);

            if (close(new_stdin) == -1)
                show_err("Could not close file descriptor when redirecting STDIN", _FATAL_ERR_);

            return true;
        }

        default:
        {
            new_stdout = open(new_stdx_name, (O_WRONLY | O_CREAT), 0666);

            if (new_stdout == -1)
            {
                show_err("Could not redirect STDOUT", _NORM_ERR_);

                return false;
            }

            original_stdout = dup(STDOUT_FILENO);

            if (original_stdout == -1)
                show_err("Could not save original STDOUT file descriptor", _FATAL_ERR_);

            if (dup2(new_stdout, STDOUT_FILENO) == -1)
                show_err("Could not duplicate STDOUT", _FATAL_ERR_);

            if (close(new_stdout) == -1)
                show_err("Could not close file descriptor when redirecting STDOUT", _FATAL_ERR_);

            return true;
        }
    }
}

/**
 * @brief Esta función chequea si un string está vacío.
 *
 * @param str String a analizar.
 *
 * @returns Si el string sólo consta de espacios y/o tabulaciones.
 */
bool stremp(char *str)
{
    int i = 0;

    while (str[i] != '\0')
        if ((str[i] != ' ') && (str[i] != '\t'))
            return false;

    return true;
}