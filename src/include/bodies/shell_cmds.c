/**
 * @file shell_cmds.c
 * @author Bonino, Francisco Ignacio (franbonino82@gmail.com).
 * @brief Librería con funciones útiles para ejecutar los
 *        comandos ingresados a la shell de los laboratorios
 *        #4 y #5.
 * @version 4.2
 * @since 2021-10-17
 */

#include "../headers/shell_cmds.h"

/**
 * @brief Esta función cambia el directorio actual de trabajo.
 *
 * @details Se utiliza la función 'chdir' para cambiar el directorio y
 *          se modifican las variables de entorno $PWD y $OLDPWD con 'setenv'.
 *
 * @param dir Directorio al que se quiere cambiar.
 */
void cmd_cd(char *dir)
{
    if (chdir(dir) != 0)
    {
        show_err(strerror(errno), _NORM_ERR_);

        return;
    }

    if (setenv("OLDPWD", getenv("PWD"), 1) == -1)
        show_err("Could not modify OLDPWD environment variable", _FATAL_ERR_);

    if (setenv("PWD", dir, 1) == -1)
        show_err("Could not modify PWD environment variable", _FATAL_ERR_);
}

/**
 * @brief Esta función limpia la consola.
 */
void cmd_clr(void)
{
    try_write(STDOUT_FILENO, _CLR_SCR_);
}

/**
 * @brief Esta función se encarga de crear un child process y
 *        asignarle la tarea de ejecutar un programa recibido
 *        como parámetro. Se aceptan comandos con pipes.
 *
 * @details Antes de llamar a esta función, debe llamarse a
 *          la función get_prog_data de la librería
 *          shell_utilities para poder setear el nombre,
 *          path y los argumentos del programa a ejecutar.
 */
void cmd_exec(void)
{
    /*
        File descriptors para pipes.

        fd[0] > lectura
        fd[1] > escritura
    */
    int fd[2];

    /*
        Si el comando contiene pipes, se arma de a uno por vez,
        se recorta el string desde un caracter '|' hasta el
        salto de línea y se ejecuta de manera recursiva cada
        comando, creando tantos pipes como sea necesario.
    */
    if (pipe_flag)
    {
        if (pipe(fd) == -1)
            show_err("Failed to create pipe", _FATAL_ERR_);

        strcpy(aux_input, input);
        strtok(aux_input, "|");

        pipe_cmd = strtrim(strtok(NULL, "\n"));

        strcpy(input, pipe_cmd);
    }

    pid_t parent_pid = getpid();
    pid_t child_pid = fork();

    switch (child_pid)
    {
        // Rutina del parent process en caso de fallo en fork
        case -1:
            show_err("Fork failed", _FATAL_ERR_);

        // Rutina del child process
        case 0:
        {
            if (signal(SIGINT, SIG_DFL) == SIG_ERR ||
                signal(SIGTSTP, SIG_DFL) == SIG_ERR ||
                signal(SIGQUIT, SIG_DFL) == SIG_ERR)
            {
                show_err("Could not assign signal handlers to child process", _NORM_ERR_);

                try_kill(getpid(), SIGTERM);
            }

            /*
                Configuramos que el child process reciba la signal
                SIGTERM cuando el parent process haya terminado.
                Testeamos en caso de que prctl falle o que el parent
                process haya terminado justo antes de la llamada a prctl.
            */
            if ((prctl(PR_SET_PDEATHSIG, SIGTERM) == -1) || (getppid() != parent_pid))
            {
                show_err("Failure in prctl function", _NORM_ERR_);

                try_kill(getpid(), SIGTERM);
            }

            if (redirect_stdin_flag && !redirect_stdx(new_stdin_name, STDIN_FILENO))
                try_kill(getpid(), SIGTERM);

            /*
                Si hay pipes, redirigimos el stdout al file descriptor
                especificado como de escritura para este proceso.
                Cerramos los extremos que no se utilicen para evitar
                file descriptors duplicados o inutilizados.
            */
            if (pipe_flag)
            {
                dup2(fd[1], STDOUT_FILENO);

                close(fd[0]);
                close(fd[1]);
            }

            /*
                Si el input fue detectado como un programa estándar, se lo
                buscará en todas las direcciones de $PATH.
                Ante la primera coincidencia con el programa, el proceso hijo
                no retornará, entonces no se seguirá buscando y así nos garantizamos
                que el programa se ejecutará sólo una vez y en la primera coincidencia.
            */
            if (prog_type == _STD_)
            {
                char *path = getenv("PATH");

                if (!path)
                {
                    show_err("Could not get PATH environment variable", _NORM_ERR_);

                    try_kill(getpid(), SIGTERM);
                }

                char *aux_path = (char *)malloc(strlen(path) + sizeof(NULL));

                if (!aux_path)
                {
                    show_err("Could not allocate memory for PATH environment variable", _NORM_ERR_);

                    try_kill(getpid(), SIGTERM);
                }

                // Tomo el primer directorio en $PATH y le concateno el nombre del programa
                strcpy(aux_path, path);

                char *next_dir = strtok(aux_path, ":");

                // Búsqueda del programa en los directorios de $PATH
                do
                {
                    strcpy(prog_path, next_dir);

                    strcat(prog_path, "/");
                    strcat(prog_path, prog_args[0]);

                    execv(prog_path, prog_args);
                } while ((next_dir = strtok(NULL, ":")) != NULL);

                show_err("No such program found", _NORM_ERR_);

                try_kill(getpid(), SIGTERM);
            }
            else if (execv(prog_path, prog_args) == -1)
            {
                show_err("No such program found", _NORM_ERR_);

                try_kill(getpid(), SIGTERM);
            }
        }

        // Rutina del parent process en caso de fork exitoso
        default:
        {
            if (!bg_flag)
            {
                if ((wait(NULL) == -1))
                    show_err("Parent process failed waiting for child process", _FATAL_ERR_);

                /*
                    Si hay pipes, hacemos fork nuevamente creando un nuevo child process
                    (en este punto podemos estar hablando de procesos hijos de padres
                    anteriores que están creando procesos hijos para sí mismos).
                */
                if (pipe_flag)
                {
                    int new_child_pid = fork();

                    switch (new_child_pid)
                    {
                        case -1:
                            show_err("Pipe fork failed", _FATAL_ERR_);

                        case 0:
                        {
                            /*
                                Redirigimos el stdin de este proceso hijo al
                                file descriptor especificado como de lectura
                                y cerramos los demás file descriptors.
                            */
                            dup2(fd[0], STDIN_FILENO);

                            close(fd[0]);
                            close(fd[1]);

                            /*
                                En este punto ponemos la flag de pipes a 'false'
                                y analizamos el string recortado entre un '|' y
                                el salto de línea. Si hay más pipes, la flag se
                                levantará de nuevo.
                                Luego, con el string analizado, se ejecutará lo
                                correspondiente, permitiendo recursividad.
                                Al final la ejecución se termina este proceso.
                            */
                            pipe_flag = false;

                            read_line(input);

                            try_kill(getpid(), SIGTERM);
                        }

                        default:
                        {
                            /*
                                El proceso padre (que aquí puede ser un proceso hijo)
                                cierra los file descriptors que no utiliza.
                                Si no hay procesos hijos que esperar, matamos este
                                subproceso para finalizar su ejecución.
                            */
                            close(fd[0]);
                            close(fd[1]);

                            if ((wait(NULL) == -1))
                                try_kill(getpid(), SIGTERM);

                            break;
                        }
                    }
                }
            }
            else
            {
                // + 7 de "PID: []" y + 2 de "\n"
                char *buffer = (char *)malloc(((sizeof(child_pid)) / (sizeof(int))) + sizeof(NULL) + (sizeof(char) * 9));

                if (!buffer)
                    show_err("Could not allocate memory for child process information message", _FATAL_ERR_);

                sprintf(buffer, "PID: [%d]\n", child_pid);

                try_write(STDOUT_FILENO, buffer);
            }

            return;
        }
    }
}

/**
 * @brief Esta función muestra en pantalla el comentario o
 *        variable de entorno que acompaña al comando 'echo'.
 *
 * @details Se toma el input y se lo muestra como comentario o
 *          como variable de entorno en base al parámetro en_var.
 *
 * @param input String a mostrar.
 * @param is_env_var Si lo que se va a mostrar es una variable de entorno.
 *
 * @returns Si la impresión fue exitosa.
 */
bool cmd_echo(char *input, bool is_env_var)
{
    if (is_env_var)
    {
        char *env_var = getenv(input);

        if (!env_var)
        {
            show_err("Invalid environment variable - no match", _NORM_ERR_);

            return false;
        }

        try_write(STDOUT_FILENO, env_var);
    }
    else
        try_write(STDOUT_FILENO, input);

    return true;
}

/**
 * @brief Esta función analiza una línea a ejecutar, recopilando información
 *        sobre las flags de comandos internos y el nombre, path y los
 *        argumentos de los programas externos a ejecutar. Luego, ejecuta
 *        lo correspondiente.
 *
 * @details Antes de ejecutar un programa, debe llamarse a esta función para
 *          setear sus datos correctamente.
 *
 * @param input String a analizar.
 *
 * @returns Si el usuario quiere terminar el programa.
 */
bool read_line(char *input)
{
    if ((set_flags(input) == -1) ||
        (pipe_flag && (cd_flag || clr_flag || quit_flag)))
        return false;

    if (quit_flag && !(cd_flag || clr_flag || echo_flag || redirect_stdin_flag || redirect_stdout_flag))
        return true;
    else if (cd_flag && !(quit_flag || clr_flag || echo_flag || redirect_stdin_flag || redirect_stdout_flag))
    {
        /*
            Si nos queremos mover de directorio, hacemos strtok al input
            con " \t" y "\n" para obtener el directorio al que queremos ir.
            Si obtenemos NULL, significa que el usuario no ingresó parámetros
            para 'cd' y eso es un input inválido. En ese caso, imprimimos un
            error y salimos.
        */
        strtok(input, " \t");

        char *aux = strtok(NULL, " \t\n");
        char *new_dir;

        if (aux == NULL)
        {
            show_err("Invalid cd command input", _NORM_ERR_);

            return false;
        }

        if (strncmp(aux, "/", 1) == 0)
        {
            /*
                Si el input comienza con el caracter '/' significa que queremos
                movernos desde el sistema de archivos principal y para esto
                reemplazamos la variable new_dir con el valor de este input.
            */
            new_dir = (char *)malloc(strlen(aux) + sizeof(NULL));

            if (!new_dir)
                show_err("Could not allocate memory for new path in cd command", _FATAL_ERR_);

            strcpy(new_dir, aux);
        }
        else if (strcmp(aux, "-") == 0)
        {
            /*
                Si el input es '-' significa que el usuario quiere acceder a la carpeta
                anterior, por lo que se hace cd con la variable de entorno $OLDPWD.
            */
            char *oldpwd = getenv("OLDPWD");

            if (!oldpwd)
            {
                show_err("There is no value for OLDPWD environment variable yet", _NORM_ERR_);

                return false;
            }

            new_dir = (char *)malloc(strlen(oldpwd) + sizeof(NULL));

            if (!new_dir)
                show_err("Could not allocate memory for new path in cd command", _FATAL_ERR_);

            strcpy(new_dir, oldpwd);
        }
        else
        {
            /*
                Si el input no es '-' y no contiene el caracter '/', significa que
                queremos acceder a una carpeta dentro del directorio actual o queremos
                ir a un directorio superior pasando por el directorio padre de la carpeta
                en la que nos encontramos, por lo que concatenamos '/aux' al directorio
                donde estamos (obtenido mediante $PWD).
                Primero chequeamos si estamos en el sistema de archivos principal (/) para no
                concatenar otro caracter '/' y terminar con un directorio erróneo con '//'.
            */
            new_dir = (char *)malloc(strlen(getcwd(NULL, 0)) + strlen(aux) + sizeof(NULL) + (sizeof(char) * 2)); // + 2 por "/"

            if (!new_dir)
                show_err("Could not allocate memory for new path in cd command", _FATAL_ERR_);

            strcpy(new_dir, getcwd(NULL, 0));

            strcat(new_dir, "/");
            strcat(new_dir, aux);
        }

        cmd_cd(new_dir);
    }
    else if (clr_flag && !(quit_flag || cd_flag || echo_flag || redirect_stdin_flag || redirect_stdout_flag))
        cmd_clr();
    else if (echo_flag && !(quit_flag || clr_flag || cd_flag))
    {
        if (redirect_stdout_flag)
        {
            if (!redirect_stdx(new_stdout_name, STDOUT_FILENO))
            {
                redirect_stdout_flag = false;

                return false;
            }

            chop_to_char(input, ">");
        }

        /*
            El input del usuario se divide en substrings.
            Si algún substring contiene el símbolo '$' indicando que
            se trata de una variable de entorno, se setea la flag
            env_var para utilizar en cmd_echo.
            Al finalizar, si hubo redirección de algún stream, se
            restaura(n) a su estado inicial.
        */
        char *token;

        if (!redirect_stdin_flag)
        {
            bool echo_err = false;
            bool space = false;

            strtok(input, " ");

            while ((token = strtok(NULL, " \t")) != NULL)
            {
                if (space)
                    try_write(STDOUT_FILENO, " ");

                bool env_var = false;

                char *aux_token = strstr(token, "$");

                // Chequeamos si el usuario sólo quiere imprimir el símbolo '$'
                if (aux_token && *(aux_token + 1) != '\0')
                {
                    memmove(&token[0], &token[1], strlen(token));

                    env_var = true;
                }

                if (!stremp(token) && !cmd_echo(token, env_var))
                    echo_err = true;

                if (!space)
                    space = true;
            }

            if (!echo_err)
                try_write(STDOUT_FILENO, "\n");
        }
        else
        {
            if (!redirect_stdx(new_stdin_name, STDIN_FILENO))
            {
                redirect_stdin_flag = false;

                return false;
            }

            size_t len = 0;

            char *line = NULL;

            // Imprimimos línea por línea todo el archivo
            while (getline(&line, &len, stdin) != -1)
                try_write(STDOUT_FILENO, line);

            try_write(STDOUT_FILENO, "\n");

            free(line); // Ver `man getline`

            /*
                Si no le hacemos rewind al nuevo stdin, dos echoes consecutivos
                al mismo archivo no funcionarán: el segundo echo quedará en el EOF
                y no imprimirá nada.
            */
            rewind(stdin);

            restore_stdx(STDIN_FILENO);
        }
    }
    else if (!(quit_flag || cd_flag || clr_flag || echo_flag))
    {
        if (redirect_stdout_flag && !redirect_stdx(new_stdout_name, STDOUT_FILENO))
            return false;

        get_prog_data(input);

        cmd_exec();
    }
    else
        show_err("Invalid commands combination", _NORM_ERR_);

    if (redirect_stdout_flag)
        restore_stdx(STDOUT_FILENO);

    return false;
}