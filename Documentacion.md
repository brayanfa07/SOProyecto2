
# PROYECTO 2


## GABRIEL RAMÍREZ RAMÍREZ (201020244)
## BRAYAN FAJARDO ALVARADO (201157035)





# Empacador de archivos TAR



Prof. Ing. Armando Arce Orozco


![Alt](/https://fotos.subefotos.com/513ff90b04afc1f7e14f93270de61a19o.png "TEC")


TEC
TECNOLÓGICO DE COSTA RICA
ESCUELA DE INGENIERÍA EN COMPUTACIÓN,
IC6600 PRINCIPIOS DE SISTEMAS OPERATIVOS,
GRUPO 02,
CARTAGO
II SEMESTRE 2017





# Tabla de Contenidos


1. [Introducción](#introducción)

2. [Descripción del problema](#descripción-del-problema)

3. [Definición de estructuras de Datos](#definición-de-estructuras-de-Datos)

4. [Descripción detallada y explicación de los componentes principales del programa](#descripción-detallada-y-explicación-de-los-componentes-principales-del-programa)

    4.1. [Mecanismo de acceso a archivos y directorios](#mecanismo-de-acceso-a-archivos-y-directorios)

    4.2. [Estructura de la tabla de asignación de espacio](#estructura-de-la-tabla-de-asignación-de-espacio)

    4.3. [Estrategia de administración de bloques libres](#estrategia-de-administración-de-bloques-libres)

    4.4. [Procedimiento de desfragmentación del archivo (opcional)](#procedimiento-de-desfragmentación-del-archivo-opcional)

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


## Implementación


    Se deberá programar el comando star (“simple tar”, NO “estrella”), de tal forma que acepte los comandos básicos mostrados anteriormente. Note que debe permitir tanto el empacar archivos individuales como directorios completos en forma recursiva. Para desarrollar su programa usted debe tomar en cuenta los siguientes aspectos:


- El archivo “tar” deberá estar estructurado mediante bloques de espacio continuo que serán asignados en forma individual a los archivos que se deben almacenar.

- Al crear un archivo empacado éste se crea del tamaño necesario para almacenar los archivos agregados. Cuando se borra algún contenido, el archivo empacado no cambia de tamaño sino que se lleva registro de los espacios (huecos) liberados. Si posteriormente se agrega nuevo contenido entonces se reutiliza el espacio libre. Si aún así el nuevo contenido no cabe, se hace crecer el archivo empacado.
- En el archivo tar se almacenará una tabla que indique la dirección en donde empieza cada archivo y su tamaño. Internamente también se debe llevar registro del contenido de los directorios. Para ello se deberá utilizar espacio adicional en el archivo empacado. El directorio se almacenará como cualquier archivo pero internamente contará con secuencia de nombres de archivo y sus posiciones en el archivo “tar”. Puede asumir que los nombres de archivo no son mayores a los 256 caracteres.
- Para asignar el espacio a cada archivo se debe buscar un hueco en el archivo tar en el cual se pueda introducir. En este caso se utilizará la estrategia de “el primer ajuste” para realizar esta asignación. Para ello se debe llevar un control interno de los huecos sin utilizar en el archivo y se debe realizar la fusión de dichos huecos cuando sea necesario.
- Si no hay un hueco del tamaño adecuado en el archivo se debe utilizar alguno de los siguientes mecanismos: (a) expandir el tamaño del archivo ó (b) realizar la compactación del archivo para eliminar huecos pequeños (opcional)
- Tome en cuenta que un archivo que se agrega puede ya existir en el archivo empacado. Es decir, lo que se desea hacer es actualizar su contenido. Para esto existe la opción update (-u) que sobrescribirá el contenido de un archivo siempre y cuando su fecha de modificación sea posterior a la del archivo empacado.
- Debe programar una solución eficiente, es decir, debe minimizar la cantidad de espacio utilizado tanto por los archivos como por las estructuras internas.
- Este programa no utilizará los derechos de acceso, que normalmente almacenaría un archivo empacado tar en ambiente UNIX.
- No utilice ningún archivo adicional (ni siquiera archivos temporales o intermedios) para implantar este programa, con excepción del archivo .tar.



# Definición de estructuras de Datos


## Uso de structs

Se define el uso de structs como la unidad básica de estructura del sistema. Se define una estructura para los componentes básicos del sistema, como lo es las colas de archivos que se van a comprimir.

Dicha struct de nombre de tar, presenta datos básicos sobre los archivos siguiendo el formato POSIX-1998, dicho formato está representado como una estructura internada dentro del struct tar principal. Además de contar con un indicador de donde comienza el archivo dentro del tar, un char que representa la información o encabezado del archivo que sirve de insumo para agregar los datos al formato posix. Para finalizar, un puntero a la información del siguiente archivo
	
## Almacenamiento de datos

Los datos de contenidos que se leen de los archivos que se comprimen son almacenados en el sistema usando structs y listas simples. Al inicio del archivo, se almacena información que hace referencia a los archivos o bloques vacíos de memoria. Luego de esto, comienza la información de cada uno de los archivos, cuando estos finalizan, se le agrega un bloque extra de información representando por “0”, esto para cumplir con estándares de escritura de los archivos tar.


# Descripción detallada y explicación de los componentes principales del programa:

## Mecanismo de acceso a archivos y directorios

Los archivos son leídos a través de una estructura que lee el contenido de los archivos.

Se implementa el método `open()` , el cual logra leer los archivos de prueba que serán comprimidos en el sistema.

También está el método `openFile()`, que logra leer la estructura del tar, y que retorna la cantidad de elementos que posee el archivo.


## Estructura de la tabla de asignación de espacio

La tabla de asignación del espacio es manejada en un struct o estructura que almacena los espacios ocupados y los espacios libres del tar. Estos espacios se muestran en una tabla que se imprime en consola.

## Estrategia de administración de bloques libres

Los bloques libres son administrados a través de un método que une o comprime los espacios para que sean asignados posteriormente a un nuevo archivo que se agregue al tar.

Los espacios están indicados de manera que se pueda acceder a ellos cuando se requiera hacer uso de ellos.

## Procedimiento de desfragmentación del archivo (opcional)

Se implementa un procedimiento de compresión, mencionado en el punto anterior, con el que se unen todos los espacios libres, a través del método `joinSpace()`, con el que se contempla el archivo tar y los archivos incluidos dentro del tar.

# Pruebas de correctitud

Para determinar el estado de un archivo, se realizará la implementación del comando `-d (-dump)` que mostrará el contenido del archivo y los espacios libres que posee este.
A partir de esta definición, se establecen las siguientes pruebas que verifican el correcto estado del archivo:


| Prueba        | Resultado     | Comentarios   |
| ------------- | ------------- | ------------- |
| Muestra de contenido de archivos agregados al tar  | Implementación incompleto  | 
Se implementa el método, pero no se logra hacer la llamada directamente desde el main. |
| Muestra de espacios vacíos en el tar  | Implementación incompleto  | 
Se implementa el método, pero no se logra hacer la llamada directamente desde el main.
|
| Se muestra la información de los datos editados | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con archivos| Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con archivos| Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main. | 
| Se muestra el dump en pantalla después de extraer del tar uno o varios archivos | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main. |
| Se muestra el dump en pantalla después de listar los archivos del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de borrar archivos del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
|Se muestra el dump en pantalla después de actualizar el tar con archivos | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de agregar archivos al tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con directorios | Implementación incompleto |Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de extraer directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.
| Se muestra el dump en pantalla después de listar los directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.
| Se muestra el dump en pantalla después de borrar directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de actualizar los directorios del tar | Implementación incompleto| Se implementa el método, pero no se logra hacer la llamada directamente desde el main. | 
| Se muestra el dump en pantalla después de agregar directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|


# Análisis de Pruebas

En este punto se describirá y analizará cada una de las pruebas realizadas a cada unas de las funciones que debe realizar el sistema.
Las pruebas a realizar se describirán a como sigue en una tabla, mencionando el nombre de la prueba, los resultados obtenidos y comentarios acerca del resultado de la prueba, entre otros puntos por comentar.


| Prueba| Resultado | Comentarios | 
| ------------- | ------------- | ------------- |
| Se crea el tar con archivos | Implementación completa | Sin comentarios | 
| Se extrae del tar uno o varios archivos del tar | Implementación completa |Sin comentarios | 
| Se enlista del tar los archivos agregados | Implementación completa | Sin comentarios |
| Se borra uno o más archivos comprimidos dentro del tar | Implementación completa | Sin comentarios|
| Se actualizan archivos en el tar | Implementación completa | Sin comentarios | 
| Se agregan archivos al tar | Implementación completa |Sin comentarios |
| Se crea el tar con directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.| 
| Se extraen del tar uno varios directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se enlistan del tar uno o varios directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se borra uno o más directorios comprimidos del tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se actualizan los directorios en el tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.| 
| Se agregan los directorios al tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se desfragmentan los archivos | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|


# Conclusiones sobre rendimiento

De la implementación del comando star, se puede extraer las siguientes conclusiones:
1. La creación de un archivo tar implementado en C,permite una gran cantidad de opciones con respecto al manejo de memoria que quizás no estén disponibles o sean más complejas en lenguajes de más alto nivel.
2. Durante la investigación, se descubrió que el formato tar en variante y que existen más de una estructura válida que debe de seguir el encabezado de los archivos para poder ser mostrados correctamente por el sistema operativo.
3. A pesar de que el programa no cumple con todas las especificaciones pedidas, se considera que tiene un buen manejo de memoria con respecto a la estructura interna del tar.
