Documentacion




# Tabla de Contenidos


1. [Introducción](#introducción)

2. [Descripción del problema](#descripción-del-problema)

3. [Definición de estructuras de Datos](#definición-de-estructuras-de-Datos)

4. [Descripción detallada y explicación de los componentes principales del programa](#descripción-detallada-y-explicación-de-los-componentes-principales-del-programa)

    1. [Mecanismo de acceso a archivos y directorios](#mecanismo-de-acceso-a-archivos-y-directorios)

    2. [Estructura de la tabla de asignación de espacio](#estructura-de-la-tabla-de-asignación-de-espacio)

    3. [Estrategia de administración de bloques libres](#estrategia-de-administración-de-bloques-libres)

    4. [Procedimiento de desfragmentación del archivo (opcional)](#procedimiento-de-desfragmentación-del-archivo-opcional)

5. [Análisis de resultados de pruebas](#análisis-de-resultados-de-pruebas)

6. [Conclusiones sobre rendimiento](#conclusiones-sobre-rendimiento)


# Introducción

Diariamente, cuando interactuamos con algún sistema operativos,directa o indirectamente estamos trabajando con archivos. La manipulación de archivos está presente en los sistemas operativos desde su creación y aunque todos tienen diversos formatos para administrarlos, internamente son muy similares. 

En este documento, se plantea una solución a un problema que se vuelve recurrente cuando se trabaja con múltiples archivos y es necesario ya sea ahorrar espacio en disco o trasladar dichos archivos de un lado a otro, comúnmente al enviar archivos por medio de correo electrónico, la creación de un archivo tar.

Un archivo tar, es una especie de contenedor que permite el almacenaje de uno o más archivos, distribuidos libremente o en carpetas, si función principal no es el comprimir el tamaño de los archivos, como en el caso de los archivos zip, aunque en ocasiones puede realizar dicho labor.

El software star, que se ha desarrollado para resolver el problema del traslado de archivos, cuenta con funciones limitadas a comparación del programa TAR integrado en los sistemas UNIX, dichas funciones son descritas más adelante en la descripción del problema

Además de esto, también se encuentra un análisis del comportamiento en memoria que tiene el programa, llamadas pruebas de correctitud. Unido a esto se brinda una breve explicación de las estructuras implementadas para la creación del archivo y para finalizar, la sección de conclusiones.



# Descripción del problema

Como descripción  general del problema, se incluye el enunciado que fue entregado al estudiante por parte del profesor, con el que se menciona el problema a describir y la forma en que debe de ser solucionado.

El objetivo de este proyecto consiste en programar un empacador de archivos. Este es el tipo de funcionalidad que provee el comando tar en ambientes UNIX. El programa tar, es usado para almacenar archivos y directorios en un solo archivo. Dentro de los entornos Unix tar aparece como un comando que puede ser ejecutada desde la línea de comandos de una consola de texto o desde un simple terminal. El formato del comando tar es, comúnmente 

~~~~
tar <opciones> <archivoSalida> <archivo1> <archivo2> ... <archivoN> 
~~~~

donde archivoSalida es el archivo resultado y archivo1, archivo2, etc., son los diferentes archivos que serán “empaquetados” en archivoSalida.

Las opciones más comunes son las siguientes:

- -c, –create : crea un nuevo archivo.
- -x, –extract : extraer de un archivo.
- -t, –list: listar los contenidos de un archivo.
- –delete: borrar desde un archivo.
- -u, –update: actualiza el contenido del archivo
- -v, –verbose: ver un reporte de las acciones a medida que se van realizando.
- -f, –file: empaquetar contenidos de archivo, si no está presente asume la entrada estándar.
- -r, –append: agrega contenido a un archivo.


Algunos de los ejemplos utilizados para ejecutar el programa son las siguientes:

1. Si queremos empacar un directorio llamado “html” y guardar los datos en “html-paq.tar”, lo haríamos con la instrucción 

~~~~
tar -cvf html-paq.tar html
~~~~

2. Si queremos desempaquetar todo el contenido de un archivo llamado xxx.tar podemos utilizar un comando como este 

~~~~
tar -xvf xxx.tar
~~~~

3. Para archivar el contenido de tres archivos doc1.txt, doc2.txt y data.dat

~~~~
tar -cvf foo.tar doc1.txt doc2.txt data.dat
~~~~

4. Si ahora se desea eliminar el contenido del archivo data.dat se ejecutaría

~~~~
tar --delete -vf foo.tar data.dat
~~~~

5. Para agregar ahora un nuevo archivo test.doc a foo.tar se ejecutaría

~~~~
tar -rvf foo.tar test.doc
~~~~

Los contenidos se desempaquetarán en el mismo directorio donde estamos situados.



# Definición de estructuras de Datos

# Descripción detallada y explicación de los componentes principales del programa:

## Mecanismo de acceso a archivos y directorios
## Estructura de la tabla de asignación de espacio
## Estrategia de administración de bloques libres
## Procedimiento de desfragmentación del archivo (opcional)

# Análisis de resultados de pruebas

# Conclusiones sobre rendimiento