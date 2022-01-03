/**
 * @file main.c
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Programa principal de los laboratorios #4 y #5 de Sistemas Operativos I.
 * @version 3.6.5
 * @since 2021-10-04
 */

#include "include/headers/shell_cmds.h"

/**
 * @brief Función principal de la shell.
 *
 * @details Se toman los inputs del usuario y sólo en caso
 *          de haber recibido únicamente el comando 'quit',
 *          la shell se cierra por completo. En caso de no
 *          haber recibido dicho comando, se ejecuta lo que
 *          corresponda y se vuelve a pedir un input al
 *          usuario para evaluar y ejecutar.
 *          Si el programa se ejecuta con un argumento ARG,
 *          se buscará en el directorio 'bfs/' un archivo
 *          batchfile llamado ARG.sh desde el cual se tomarán
 *          los comandos a ejecutar. Cuando se detecta el EOF,
 *          o la instrucción 'quit', la shell se cierra.
 *
 * @param argc Cantidad de argumentos recibidos.
 * @param argv Vector con los argumentos recibidos.
 *
 * @returns Si la ejecución del programa fue exitosa.
 */
int main(int argc, char **argv)
{
    base_cwd = strtrim(chop_to_char(getcwd(NULL, 0), "\n"));

    if (!base_cwd)
        show_err("Could not get the base cwd", _FATAL_ERR_);

    if ((regcomp(&bg_regex, "[ \t]*&[ \t]*$", 0) != 0) ||
        (regcomp(&cd_regex, "^[ \t]*cd[ \t]*", 0) != 0) ||
        (regcomp(&clr_regex, "^[ \t]*clr[ \t]*$", 0) != 0) ||
        (regcomp(&echo_regex, "^[ \t]*echo[ \t]*", 0) != 0) ||
        (regcomp(&pipe_regex, "[ \t]*[|][ \t]*", 0) != 0) ||
        (regcomp(&quit_regex, "^[ \t]*quit[ \t]*$", 0) != 0))
        show_err("Could not compile regex", _FATAL_ERR_);

    char group_name[_HOST_MAX_LEN_];

    char *prompt;
    char *user_name;

    bool bf = false;
    bool quit = false;

    FILE *input_file = NULL;

    /*
        Ignoramos en el proceso padre las señales especificadas.
        Las señales ignoradas son aquellas que deberán ser atendidas
        sólo por los child processes en foreground execution.
    */
    if (signal(SIGINT, SIG_IGN) == SIG_ERR ||
        signal(SIGTSTP, SIG_IGN) == SIG_ERR ||
        signal(SIGQUIT, SIG_IGN) == SIG_ERR)
        show_err("Could not ignore signals in parent process", _FATAL_ERR_);

    if (argc > 2)
        show_err("Invalid program arguments amount", _FATAL_ERR_);
    else if (argc == 2)
    {
        if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "?") == 0) || (strcmp(argv[1], "--help") == 0))
        {
            show_help(_HELP_);

            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[1], "-e") == 0)
        {
            show_help(_EXPS_);

            return EXIT_SUCCESS;
        }
        else
        {
            // + 4 por "bfs/" y + 3 por ".sh"
            char *bf_path = (char *)malloc(strlen(argv[1]) + sizeof(NULL) + (sizeof(char) * 7));

            if (!bf_path)
                show_err("Could not allocate memory for batchfile directory", _FATAL_ERR_);

            strcpy(bf_path, "bfs/");

            strcat(bf_path, argv[1]);
            strcat(bf_path, ".sh");

            if (open_file_r(&input_file, bf_path))
                bf = true;

            free(bf_path);

            if (!bf)
                show_err(strerror(errno), _FATAL_ERR_);
        }
    }

    if (gethostname(group_name, _HOST_MAX_LEN_) != 0)
        show_err("Could not get hostname", _FATAL_ERR_);

    user_name = getenv("USER");

    if (!user_name)
        show_err("Could not get username", _FATAL_ERR_);

    prompt = (char *)malloc(strlen(user_name) + strlen(group_name) +
                           (strlen(_FG_COLOR_YELLOW_) * 3) + (strlen(_FG_COLOR_GREEN_) * 2) +
                            strlen(_FG_COLOR_BLUE_) + strlen(_FORMAT_RESET_ALL_) + sizeof(NULL));

    if (!prompt)
        show_err("Could not allocate memory for prompt", _FATAL_ERR_);

    print_hmsg(_WELC_);

    memset(input, 0, sizeof(input)); // Para evitar basura en el input del usuario

    if (!bf) // Análisis del input del usuario
        while (!quit)
        {
            mk_prompt(user_name, group_name, &prompt);

            if (write(STDOUT_FILENO, prompt, strlen(prompt)) == -1)
            {
                fprintf(stderr, "\nFatal error on write instruction --- ABORTING\n");

                free(prompt);

                exit(EXIT_FAILURE);
            }

            if (read(STDIN_FILENO, input, _STR_MAX_LEN_) == -1)
                show_err("Could not get user input", _FATAL_ERR_);

            strcpy(input, chop_to_char(strtrim(input), "\n"));

            // Sólo ejecutamos líneas de código no vacías
            if (!stremp(input))
                quit = read_line(input);

            memset(input, 0, sizeof(input)); // Reseteamos el input
        }
    else // Análisis de batchfile
        while (!quit && (fgets(input, _STR_MAX_LEN_, input_file) != NULL))
        {
            mk_prompt(user_name, group_name, &prompt);

            if ((write(STDOUT_FILENO, prompt, strlen(prompt)) == -1) ||
                (write(STDOUT_FILENO, input, strlen(input)) == -1))
            {
                fprintf(stderr, "\nFatal error on write instruction --- ABORTING\n");

                free(prompt);

                exit(EXIT_FAILURE);
            }

            sleep(1); // Pausa para ver qué input se va a analizar

            if (!strstr(input, "\n"))
                if (write(STDOUT_FILENO, "\n", 2) == -1)
                {
                    fprintf(stderr, "\nFatal error on write instruction --- ABORTING\n");

                    free(prompt);

                    exit(EXIT_FAILURE);
                }

            strcpy(input, chop_to_char(strtrim(input), "\n"));

            if (!stremp(input))
                quit = read_line(input);

            memset(input, 0, sizeof(input));
        }

    print_hmsg(_FARE_);

    free(prompt);

    return EXIT_SUCCESS;
}