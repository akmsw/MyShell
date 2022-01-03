/**
 * @file shell_utilities.c
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Librería con funciones útiles particulares
 *        para los laboratorios #4 y #5.
 * @version 8.0
 * @since 2021-10-05
 */

#include "../headers/shell_utilities.h"

/**
 * @brief Esta función devuelve el prompt a mostrar por consola en formato
 *        "<user_name>\@<group_name>:<CWD>$".
 *
 * @param user_name Nombre de usuario.
 * @param group_name Nombre de grupo (hostname).
 * @param prompt Prompt a modificar.
 */
void mk_prompt(char *user_name, char *group_name, char **prompt)
{
    char *aux_prompt = (char *)realloc(*prompt, (malloc_usable_size(*prompt) + strlen(getcwd(NULL, 0)) + sizeof(NULL)));

    if (!aux_prompt)
    {
        free(*prompt);

        show_err("Could not get enough memory for prompt reallocation", _FATAL_ERR_);
    }

    *prompt = aux_prompt;

    strcpy(*prompt, _FG_COLOR_GREEN_);
    strcat(*prompt, chop_to_char(user_name, "\n"));
    strcat(*prompt, _FG_COLOR_YELLOW_);
    strcat(*prompt, "@");
    strcat(*prompt, _FG_COLOR_GREEN_);
    strcat(*prompt, chop_to_char(group_name, "\n"));
    strcat(*prompt, _FG_COLOR_YELLOW_);
    strcat(*prompt, ":");
    strcat(*prompt, _FG_COLOR_BLUE_);
    strcat(*prompt, chop_to_char(getcwd(NULL, 0), "\n"));
    strcat(*prompt, _FG_COLOR_YELLOW_);
    strcat(*prompt, "$ ");
    strcat(*prompt, _FORMAT_RESET_ALL_);
}

/**
 * @brief Esta función imprime un mensaje predefinido en la consola.
 *
 * @param msg_type Si el mensaje debe ser de bienvenida, despedida
 *                 o de pedido de ayuda.
 */
void print_hmsg(int msg_type)
{
    char *text;

    switch (msg_type)
    {
        case _HELP_:
        {
            text = "[HELP]\n\n";

            break;
        }

        case _WELC_:
        {
            text = "[WELCOME]\n\n";

            break;
        }

        case _EXPS_:
        {
            text = "[EXAMPLES]\n\n";

            break;
        }

        default:
        {
            text = "[GOODBYE]\n\n";

            break;
        }
    }

    // + 18 por "\n> MyShell\\akmsw_"
    char *h_msg = (char *)calloc(strlen(_FORMAT_BOLD_) + strlen(_FG_COLOR_WHITE_) +
                                 strlen(_FORMAT_RESET_ALL_) + strlen(text) +
                                 sizeof(NULL) + (sizeof(char) * 18), sizeof(char));

    if (!h_msg)
        show_err("Could not allocate memory for message", _FATAL_ERR_);

    strcat(h_msg, _FORMAT_BOLD_);
    strcat(h_msg, _FG_COLOR_WHITE_);
    strcat(h_msg, "\n> MyShell\\akmsw_");
    strcat(h_msg, _FORMAT_RESET_ALL_);
    strcat(h_msg, text);

    try_write(STDOUT_FILENO, h_msg);

    free(h_msg);
}

/**
 * @brief Esta función imprime el menú de ayuda correspondiente a lo solicitado.
 *
 * @param h_type El tipo de ayuda solicitada.
 */
void show_help(int h_type)
{
    print_hmsg(h_type);
    
    if (h_type == _HELP_)
    {
        // + 3187 por el largo del mensaje
        char *h_general = malloc((strlen(_FORMAT_BOLD_) * 28) + (strlen(_FORMAT_RESET_ALL_) * 29) +
                                 (strlen(_FG_COLOR_WHITE_) * 32) + (strlen(_BG_COLOR_LGRAY_) * 2) +
                                 sizeof(NULL) + (sizeof(char) * 3187));

        if (!h_general)
            show_err("Could not allocate memory for message", _FATAL_ERR_);

        strcpy(h_general, _FORMAT_BOLD_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "INTERNAL COMMANDS\n\n");
        strcat(h_general, "cd <dir> ______________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Changes the current directory to ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "<dir>");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " if it exists.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "cd .. _________________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Accesses the parent folder of the current directory.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "cd - __________________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Returns to the last working directory.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "clr ___________________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Cleans the console output.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "echo <comment> ________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Shows ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "<comment>");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " followed by a space.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "echo <env_var> ________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Shows the ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "<env_var>");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " value.\n                                ");
        strcat(h_general, "If the user input consists of mixed comments and\n                                ");
        strcat(h_general, "environment variables, they will be shown properly.\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "quit __________________________ ");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Closes the shell.\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "EXTERNAL PROGRAMS EXECUTION\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "Every input that isn't an internal command, ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "-h");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " (or any help-request argument) or ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "-e");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, ", is interpreted as an executable\nfile name with its arguments.\n");
        strcat(h_general, "The user can run an external program with its absolute path, relative path and even\n");
        strcat(h_general, "a standard program that will be searched through the ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "$PATH");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " environment variable.\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "BATCHFILES\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "The user can run this shell using a custom batchfile with any of the commands listed\nabove. Such batchfile must be a shell script (i.e.: it must be a ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, ".sh");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " file) and must\nbe located in the ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "bfs/");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " folder created automatically by the makefile.\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "BACKGROUND EXECUTION\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "If the command entered is an external program execution and it ends with '");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "&");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "', a child\nprocess will be created to execute such program and it will remain in background\n");
        strcat(h_general, "execution until the parent process dies.\n");
        strcat(h_general, "When a background execution starts, a message is printed showing the child process ID.\n");
        strcat(h_general, "Due to interleaving and unpredictible concurrency behaviour, the parent and the child\nprocess outputs can be mixed.\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "I/O REDIRECTION\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "The user can redirect the input and/or output stream(s).\n");
        strcat(h_general, "For both cases, the new stream file must be placed in their corresponding folder.\n");
        strcat(h_general, "New stdin streams will be searched only in ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "/redir/stdin");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, " and new stdout streams will be\nsearched only in ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "/redir/stdout");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, ".\nIn order to redirect the input stream, the user must use the reserved character: ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "<");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "\nfollowed by the file name. The same goes to the output stream redirection, but with\n");
        strcat(h_general, "its corresponding reserved character: ");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, ">");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, ".\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, "PIPING\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "The user can connect two or more commands via the pipe operator '");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "|");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, "'.\n");
        strcat(h_general, "The pipe operator redirects the left-side command stdout to the right-side command stdin.\n");
        strcat(h_general, "This functionality allows the user to interact with the input and output of different programs.\n\n");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, _BG_COLOR_LGRAY_);
        strcat(h_general, "For execution examples, run this program with the parameter '");
        strcat(h_general, _FORMAT_BOLD_);
        strcat(h_general, "-e");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, _FG_COLOR_WHITE_);
        strcat(h_general, _BG_COLOR_LGRAY_);
        strcat(h_general, "'.");
        strcat(h_general, _FORMAT_RESET_ALL_);
        strcat(h_general, "\n\n");

        try_write(STDOUT_FILENO, h_general);
    }
    else
    {
        // + 1556 por el largo del mensaje
        char *h_examples = malloc((strlen(_FORMAT_BOLD_) * 30) + (strlen(_FORMAT_RESET_ALL_) * 30) +
                                  (strlen(_FG_COLOR_WHITE_) * 30) + (strlen(_BG_COLOR_LGRAY_) * 2) +
                                  sizeof(NULL) + (sizeof(char) * 1556));

        if (!h_examples)
            show_err("Could not allocate memory for message", _FATAL_ERR_);

        strcpy(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "INTERNAL COMMANDS EXAMPLES\n\n");
        strcat(h_examples, "cd");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " /home\n");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "cd ..\ncd ../..\ncd -\nclr\necho");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " example comment\n");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "echo");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " comment1 $PATH comment2 $USER $PWD\n");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "quit\n\n");
        strcat(h_examples, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "EXTERNAL PROGRAMS EXECUTION EXAMPLES\n\n");
        strcat(h_examples, "man strtok\nls -l\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "exe_name\nexe_name arg1\n./relative/path/to/executable arg1 arg2\n/absolute/path/to/executable\n\n");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "BATCHFILES EXAMPLES\n\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "./bin/myshell batchfile_name\n\n");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "BACKGROUND EXECUTION EXAMPLES\n\nman strtok ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "ls -l ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "spotify ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "firefox ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "slack ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "./relative/path/to/executable arg1 arg2 ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "/absolute/path/to/executable ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "&\n\n//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "I/O REDIRECTION EXAMPLES\n\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "echo ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, ">");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdin\necho comment ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, ">");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdout\necho ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "<");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdin ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, ">");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdout\nps aux ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, ">");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdin\ngrep [something] ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "<");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdin ");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, ">");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, " new_stdout");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "\n\n//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, "PIPING EXAMPLES\n\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "last <username>");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "wc -l\nps aux");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "grep firefox\ngrep bash /etc/passwd");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "cut -d : -f 1");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "sort\ncat [filename]");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "sort -r");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "uniq");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, " | ");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, "head -4 > [new_stdout]");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "\n\n//////////////////////////////////////////////////////////////////////////////////////\n\n");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, _BG_COLOR_LGRAY_);
        strcat(h_examples, "For more detailed help, run this program with the parameter '");
        strcat(h_examples, _FORMAT_BOLD_);
        strcat(h_examples, "-h");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, _FG_COLOR_WHITE_);
        strcat(h_examples, _BG_COLOR_LGRAY_);
        strcat(h_examples, "'.");
        strcat(h_examples, _FORMAT_RESET_ALL_);
        strcat(h_examples, "\n\n");

        try_write(STDOUT_FILENO, h_examples);
    }
}

/**
 * @brief Esta función se encarga de setear las flags
 *        correspondientes a las funcionalidades de
 *        la shell.
 *
 * @param input Input de usuario a analizar.
 *
 * @returns Si no hubo problema al setear las flags, se
 *          retorna 0. En caso de falla, se retorna -1.
 */
int set_flags(char *input)
{
    bg_flag = false;
    cd_flag = false;
    clr_flag = false;
    echo_flag = false;
    pipe_flag = false;
    quit_flag = false;
    redirect_stdin_flag = false;
    redirect_stdout_flag = false;

    if (regexec(&cd_regex, input, 0, NULL, 0) == 0)
        cd_flag = true;

    if (regexec(&clr_regex, input, 0, NULL, 0) == 0)
        clr_flag = true;

    if (regexec(&echo_regex, input, 0, NULL, 0) == 0)
        echo_flag = true;

    if (regexec(&pipe_regex, input, 0, NULL, 0) == 0)
        pipe_flag = true;

    if (regexec(&quit_regex, input, 0, NULL, 0) == 0)
        quit_flag = true;

    if (regexec(&bg_regex, input, 0, NULL, 0) == 0)
    {
        if (!(cd_flag || clr_flag || quit_flag || echo_flag || pipe_flag))
            bg_flag = true;
        else
        {
            show_err("Invalid background execution command", _NORM_ERR_);

            return -1;
        }
    }

    if (strstr(input, "<"))
        redirect_stdin_flag = true;

    if (strstr(input, ">"))
        redirect_stdout_flag = true;

    if (redirect_stdin_flag)
    {
        strtok(input, "<"); /* No hace falta chequear si strtok devuelve NULL porque
                               la flag está levantada y strstr no modifica el input. */
        strcpy(new_stdin_name, base_cwd);
        strcat(new_stdin_name, "/redir/stdin/");

        char *temp_s = strtok(NULL, " \t>");

        if (!temp_s)
        {
            show_err("Invalid STDIN redirection input", _NORM_ERR_);

            return -1;
        }

        strcat(new_stdin_name, chop_to_char(temp_s, "\n"));
    }

    if (redirect_stdout_flag)
    {
        strtok(input, ">");

        strcpy(new_stdout_name, base_cwd);
        strcat(new_stdout_name, "/redir/stdout/");

        char *temp_s = strtok(NULL, " \t");

        if (!temp_s)
        {
            show_err("Invalid STDOUT redirection input", _NORM_ERR_);

            return -1;
        }

        strcat(new_stdout_name, chop_to_char(temp_s, "\n"));
    }

    return 0;
}

/**
 * @brief Esta función recopila el nombre, el path y los argumentos de
 *        un programa a ejecutar.
 *
 * @param input Input del usuario, donde está toda la información del
 *              programa a ejecutar.
 *
 * @returns Si se pudo recopilar la información correctamente.
 */
bool get_prog_data(char *input)
{
    args_amount = 1;

    prog_path = malloc(strlen(getenv("PATH"))); // Directorio actual de búsqueda

    if (!prog_path)
        show_err("Could not allocate memory for program path", _FATAL_ERR_);

    prog_args = (char **)malloc(sizeof(char *) + sizeof(NULL)); /* Espacio mínimo necesario por el nombre
                                                                   y el argumento NULL. */

    if (!prog_args)
        show_err("Could not allocate memory for program arguments", _FATAL_ERR_);

    strcpy(aux_input, input);

    /*
        Si el input comienza con "./", hace referencia a un path relativo.
        Si el input comienza con "/", hace referencia a un path absoluto.
        De lo contrario, hace referencia a un programa estándar a buscar en $PATH.
    */
    if (aux_input[0] == '.' && aux_input[1] == '/')
        prog_type = _REL_;
    else if (aux_input[0] == '/')
        prog_type = _ABS_;
    else
        prog_type = _STD_;

    /*
        Si hay un espacio o tabulación en el input, significa que además
        de proporcionar el nombre del programa, el usuario ingresó
        argumentos. De lo contrario, el argumento es NULL.
        Se reserva memoria de manera dinámica para almacenar los
        argumentos recibidos para el programa.
    */
    if (strstr(aux_input, " ") || strstr(aux_input, "\t"))
    {
        prog_name = strtok(aux_input, " \t");

        prog_args[0] = (char *)malloc(strlen(prog_name) + sizeof(NULL));

        if (!prog_args[0])
        {
            show_err("Could not allocate memory for program arguments", _NORM_ERR_);

            free(prog_args);

            return false;
        }

        strcpy(prog_args[0], prog_name); // Siempre el primer argumento para el programa será el nombre del programa

        char *aux_arg = strtok(NULL, " \t"); // Porción del input a evaluar si es argumento

        while ((aux_arg != NULL) &&
               (strcmp(aux_arg, "<") != 0) && (strcmp(aux_arg, ">") != 0) &&
               (strcmp(aux_arg, "&") != 0) && (strcmp(aux_arg, "|") != 0))
        {
            aux_arg = strtrim(aux_arg);

            char **aux_prog_args = (char **)realloc(prog_args, (((1 + args_amount) * sizeof(char *)) + sizeof(NULL)));

            if (aux_prog_args == NULL)
            {
                show_err("Could not reallocate memory for program arguments", _NORM_ERR_);

                free(prog_args);

                return false;
            }

            prog_args = aux_prog_args;

            prog_args[args_amount] = (char *)malloc(strlen(aux_arg) + sizeof(NULL));

            if (!prog_args[args_amount])
            {
                show_err("Could not allocate memory for program arguments", _NORM_ERR_);

                free(prog_args);

                return false;
            }

            strcpy(prog_args[args_amount], chop_to_char(aux_arg, "\n"));

            args_amount++;

            aux_arg = strtok(NULL, " \t");
        }
    }
    else
    {
        prog_name = strtok(aux_input, "\n");

        prog_args[0] = (char *)malloc(strlen(prog_name) + sizeof(NULL));

        if (!prog_args[0])
        {
            show_err("Could not reallocate memory for program arguments", _NORM_ERR_);

            free(prog_args);

            return false;
        }

        strcpy(prog_args[0], prog_name);
    }

    prog_args[args_amount] = NULL; // Siempre el último argumento para el programa será NULL

    if (prog_type == _REL_)
    {
        prog_name = strtok(prog_name, ".");

        chop_to_char(prog_name, "\n");

        strcpy(prog_path, getenv("PWD"));

        strcat(prog_path, "/");
        strcat(prog_path, prog_name);
    }
    else if (prog_type == _ABS_)
        strcpy(prog_path, chop_to_char(input, "\n"));

    return true;
}