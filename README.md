# Laboratorios Nº4 y Nº5 - MyShell
## Sistemas Operativos I - FCEFyN - UNC

Para comenzar, se debe clonar el repositorio. Una vez hecho, podemos compilar todo el proyecto aprovechando el archivo makefile, con el comando:

`make myshell`

o, simplemente:

`make`

Esto compilará todo con las flags indicadas en la consigna:

`gcc -Wall -Werror -pedantic`

Se organizó el proyecto en distintas carpetas para un mejor manejo de archivos. El código fuente en lenguaje C que engloba la resolución del trabajo se encuentra en la carpeta `src`. Los headers y cuerpos de las librerías utilizadas se encuentran en `src/include/headers` y `src/include/bodies` respectivamente.

Se crean carpetas adicionales para los archivos `.o`, `.a`, para los batchfiles, para los archivos de redirección de `stdin` y `stdout`, y para el ejecutable final.

A continuación se proporciona una vista general de la organización de los archivos del proyecto una vez compilado y listo para su ejecución, suponiendo un archivo batchfile (`bf1.sh`), un archivo de redirección de `stdin` (`new_stdin`) y un archivo de redirección de `stdout` (`new_stdout`):

![tree](https://user-images.githubusercontent.com/66426042/145492464-a1487aec-cb1b-4001-9e5a-fd2d7c77ee93.png)

Para obtener información sobre cómo correr el programa, puede seguir leyendo este documento o ejecutar el programa con los parámetros `-h`, `?` o `--help` para desplegar el menú de ayuda.

## Internal commands
Esta consola soporta los siguientes comandos internos:

- **`cd <directorio>`**: Cambia el directorio actual a `directorio`. Si `directorio` no existe, se imprime un error. Este comando modifica las variables de entorno `$PWD` y  `$OLDPWD`.
    - **`cd ..`**: Accede al directorio padre de la carpeta actual.
    - **`cd -`**: Vuelve al último directorio de trabajo.
- **`clr`**: Limpia la consola.
- **`echo <comment>`**: Muestra `comment` en la pantalla seguido por un salto de línea. Si `comment` contiene alguna variable de entorno (`$PATH`, `$PWD`, `$OLDPWD`, `$USER`, `$HOME`...), se muestra el valor de la misma.
- **`quit`**: Cierra la consola.

> *Consulte ejemplos de ejecución de comandos internos iniciando la shell con el parámetro `-e`*

## External commands / programs invocation
Las entradas del usuario que no sean comandos internos, son interpretadas como la invocación de un programa con sus argumentos. El input puede ser algún programa estándar como por ejemplo `man`, `ls` o `ps` (cuyos paths absolutos se obtienen mediante la variable de entorno `$PATH`), un path relativo a algún ejecutable (por ejemplo, la shell puede ejecutarse a sí misma ingresando el path relativo `./bin/lab4`), o puede ser un path absoluto que indique dónde se debe hallar el ejecutable deseado. Todas estas opciones soportan tantos argumentos como sean necesarios para estos programas o comandos.

> *Consulte ejemplos de ejecución de programas y comandos externos iniciando la shell con el parámetro `-e`*

## Batchfiles
Esta shell es capaz de tomar sus comandos a ejecutar desde un archivo **batchfile**. Para esto, el usuario debe crear un archivo con extensión `.sh` y ubicarlo en la carpeta `/bfs` creada automáticamente por el makefile. Una vez hecho, debe ejecutarse la shell con el nombre del batchfile como parámetro. La extensión `.sh` y el path con la carpeta `/bfs` lo agrega de forma automática la propia shell, por lo que sólo es necesario el **nombre** del archivo.

Cuando se alcanza el fin de archivo (`EOF`) o el comando `quit`, la consola se cierra.

Si el programa se ejecuta sin argumento (`.bin/lab4`), se muestra el command prompt y se esperan los inputs del usuario.

> *Consulte ejemplos de ejecución de batchfiles iniciando la shell con el parámetro `-e`*

## Background execution
Un ampersand (`&`) al final de la línea de comandos indica que la consola debe lanzar un proceso hijo para ejecutar el programa en background.\
Cuando se comienza un trabajo en background, se imprime un mensaje indicando el ID del proceso hijo en ejecución, de la forma:

`PID: [<pid>]`

> *Consulte ejemplos de ejecución de programas en background iniciando la shell con el parámetro `-e`*

## Signal handling
Las señales `SIGINT`, `SIGQUIT` y `SIGTSTP` serán ignoradas por la shell y serán redirigidas al child process en foreground execution en caso de existir alguno.

## I/O redirection
El usuario podrá redirigir tanto el input como el output del programa. Para poder redirigir el `stdin` debe hacerlo mediante el caracter reservado `<` seguido del nombre del archivo que reemplazará al `stdin`. Lo mismo se debe hacer para redirigir el `stdout` pero con el caracter reservado `>`.\
Se puede redirigir sólo el `stdin`, sólo el `stdout`, o ambos a la vez.\
Si se redirige el `stdin`, se tomará el contenido del archivo especificado como input para el programa a ejecutar.\
Si se redirige el `stdout`, el output de la shell será enviado al archivo especificado, sobrescribiendo lo que haya si el archivo ya existe, o, en caso contrario, creando el archivo y escribiéndolo.\
Sin importar la carpeta en la que se esté, los archivos de redirección de I/O que quiera utilizar el usuario, deberán estar guardados en las carpetas apropiadas creadas automáticamente por el makefile, nombradas en la introducción del proyecto.

> *Consulte ejemplos de redirección de I/O iniciando la shell con el parámetro `-e`*

## Piping
El usuario podrá ejecutar más de un comando a la vez mediante *pipes* a través del operador `|`. Este operador conecta la salida estándar (`stdout`) de un proceso lanzado por el comando de la izquierda del pipe con la entrada estándar (`stdin`) del proceso que se genera con el comando a la derecha del pipe.\
Se pueden concatenar mediante pipes tantos comandos como se desee, y el output final puede ser redirigido a otro archivo.

> *Consulte ejemplos de comandos conectados mediante pipes iniciando la shell con el parámetro `-e`*

# Remarks
## General
- Para evitar posibles malinterpretaciones de los inputs del usuario, evite usar tabulaciones y/o más de un caracter de espacio entre los comandos/programas y sus argumentos. Se han realizado muchos tests que aseguran un buen reconocimiento en escenarios de este estilo, pero puede que algún caso específico no haya sido detectado aún.\
Si detecta un mal funcionamiento de la shell en algún escenario no contemplado en la sección ***Known issues***, considere abrir un issue en el repositorio para trabajar en su resolución tan rápido como se pueda.
- Esta shell no mantiene un historial de comandos ingresados, por lo que no se puede hacer uso de las teclas de cursor para acceder a algún comando ingresado previamente.
## Stream-redirection related:
- A la hora de redirigir el `stdin`, recuerde que el archivo que reemplazará el stdin **debe estar situado en la carpeta correspondiente nombrada anteriormente**. Lo mismo se debe tener en cuenta para los archivos que reemplazarán al `stdout`: se encontrarán en la carpeta de redirección de `stdout`.\
Si desea ejecutar un programa tomando como input el output de otro programa redirigido anteriormente, tenga en cuenta que deberá mover el archivo de `stdout` anterior a la carpeta de redirección de `stdin` para que la shell lo encuentre.\
Por ejemplo, si se desea hacer:\
`ps aux > new_stdout`\
`grep [something] < new_stdout > new_stdout2`\
se deberá mover el archivo `new_stdout` resultante de la primera línea hacia la carpeta `/redir/stdin` para que la segunda línea pueda hallarlo como input para el programa.\
***Se resalta este detalle repetidas veces para evitar inconvenientes.***

# Known issues
## Signal-handling related:
- Al ejecutar el programa `man` y querer interrumpirlo con `SIGTSTP`, el proceso se detiene pero no se puede volver a tomar el control de la shell.
- Cuando no hay child processes en foreground execution y se ingresa alguna de las señales mencionadas en los puntos anteriores, queda escrito como input del usuario `^C`, `^Z` o `^\` y no puede borrarse, por lo que se lo debe enviar como input a la shell (presionar *enter*) y luego se podrá ingresar un comando válido.
## I/O-redirection related:
- El orden de los elementos utilizados para redirección (los caracteres reservados `<`, `>` y los nombres de los archivos a donde redirigir) deben escribirse en orden: primero la redirección del `stdin` y luego la redirección del `stdout`. De lo contrario, el comportamiento de la shell es indefinido.
