/*star.c 
 * FILE VERSION 007
 * April 23, 2017
 * 
 * Operative Systems's Principles
 * Project II
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include "star.h"



int MAX(int val1, int val2){
    if(val1 > val2){
        return val1;
    }
    else{
        return val2;
    }
}



//Comands to insert in a main query: ./star <options> <outFile> <file1> <file2> ... <fileN>

int main(int argc, char * argv[]){
    //printf("ARGC[]: %d\n", argc);
    if (((argc == 2) && (strncmp(argv[1], "help", MAX(strlen(argv[1]), 4)))) && (argc != 3)){
        fprintf(stderr, "\nCOMANDO USO: %s <OPCIONES> <ARCHIVOSALIDA.tar> <ARCHIVO1> <ARCHIVO2> ...<ARCHIVON>\n", argv[0]);
        fprintf(stderr, "\nAYUDA: %s ayuda\n", argv[0]);
        return -1;
    }

    if (argc == 2 || argc == 1){
        fprintf(stdout, "\nCOMANDO USO: %s <OPCIONES> <ARCHIVOSALIDA.tar> <ARCHIVO1> <ARCHIVO2> ...<ARCHIVON>\n"\
                        "\nAYUDA: %s ayuda\n"\
                        "\n"\
                        "Programa: Simple TAR. \n"\
                        "    OPCIONES:\n"\
                        "        -c -> crea un nuevo archivo TAR.\n"\
                        "        -x -> extraer de un archivo.\n"\
                        "        -t -> listar los contenidos de un archivo.\n"\
                        "        -d -> borrar desde un archivo.\n"\
                        "        -u -> actualiza el contenido del archivo.\n"\
                        "        -v -> ver reporte de las acciones que se van realizando.\n"\
                        "        -f -> empaquetar contenido de archivo o desplegar en pantalla.\n"\
                        "        -r -> agrega contenido a un archivo.\n"\
                        "\n"\
                        "\n"\
                        "EJEMPLO DE EJECUCIÓN: %s -cvf salida.tar archivo1.txt archivo2.c\n\n"\
                      , argv[0], argv[0], argv[0]);
      return 0;
    }

    argc -= 3;

    int rc = 0;
    char c = 0,             // create
         x = 0,             // extract
         t = 0,             // list
         d = 0,             // delete
         u = 0,             // update
         v = 0,             // verbose 0: no print; 1: print file names
         f = 0,             // file
         r = 0;             // append

    // parse options
    int i;
    for(i = 0; argv[1][i]; i++){
        switch (argv[1][i]){
            case 'c': c = 1; break; //Afecta
            case 'x': x = 1; break; //Afecta            
            case 't': t = 1; break; //Afecta
            case 'd': d = 1; break; //Afecta
            case 'u': u = 1; break; //Afecta
            case 'v': v = 1; break; //NO Afecta
            case 'f': f = 1; break; //NO Afecta
            case 'r': r = 1; break; //Afecta
            case '-': break;
            default:
                fprintf(stderr, "Error: Bad option: %c\n", argv[1][i]);
                fprintf(stderr, "Do '%s help' for help\n", argv[0]);
                return 0;
                break;
        }
    }
    //Comprueba que la opcion introducida sea válida.
    const char cadenaOpciones = c + x + t + d + u + r;
    if (cadenaOpciones > 1){
        fprintf(stderr, "Error: OPCIONES INCORRECTAS\n");
        return -1;
    }
    else if (cadenaOpciones < 1){
        fprintf(stderr, "Error: NECESITA ALGUNA OPCION '-c-x-t-d-u-r'\n");
        return -1;
    }

    /*const char * filename = argv[2];
    const char ** files = (const char **) &argv[3];

    // //////////////////////////////////////////

    //struct tar_t * archive = NULL;
    int fd = -1;
    if (c){             // create new file
        if ((fd = open(filename, O_WRONLY|O_CREAT)) == -1){
            fprintf(stderr, "Error: Unable to open file %s\n", filename);
            return -1;
        }

        if (tar_write(fd, &archive, argc, files, verbosity) < 0){
            rc = -1;
        }
    }
    else{
        fprintf(stderr, "Exiting with error due to previous error\n");
    }

    //tar_free(archive);
    close(fd);          // don't bother checking for fd < 0
    */
    printf("OPCION CORRECTA: %s\n", argv[1]);
    return rc;
}
/*
int main(int argc, char *argv[]){
	struct tar  * archive;
	int fd;
		
	if (argc == 1){
        printf("No se encontró comando a ejecutar\nc");
        return 0;
    }

    char command[100];
    strcpy(command,argv[1]);


    if (strcmp(command,"-c")==0){
        printf("=======> Creando archivo tar ...\n");
        const char * filename = argv[2];
        const char ** files = (const char **) &argv[3];

		int fd = open(filename,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

		int start = writeTableInformation(fd,files,argc-3);
		writeFile(fd,&archive,argc-3,files,start);
    }
    
    if (strcmp(command,"-x")==0){
        printf("Extrayendo archivos... \n");
        
        extractFiles(archive,fd);
    }
    
    if (strcmp(command,"-t")==0){
        printf("Listando los archivos dentro del tar...\n");
        
        listFiles(fd, archive);
    }
    
    if (strcmp(command,"-delete")==0){
        printf("Borrando los archivos dentro del tar...\n");
        
        
        int countFilesToDelete;
        
        int i = 0;
        int countParams = argc - 3;
        const char * files[countParams];
        for (i; i < countParams; i++){
			if (i > 1){
				files[i-2] = argv[i];
				countFilesToDelete = countFilesToDelete + 1;			
			}
		}
		
		int totalFilesInTar = openFile(fd, &archive);
        deleteFiles(&archive, fd, files,countFilesToDelete,totalFilesInTar);  
    }
    
    if (strcmp(command,"-u")==0){
        printf("Actualizando un archivo o varios archivos dentro del tar...\n");
        
        
    }
    
    if (strcmp(command,"-v")==0){
        printf("Creando reporte de las acciones realizadas en el tar...\n");        
    }
    
    if (strcmp(command,"-f")==0){
        printf("Empaquetando los archivos de un archivo \n");
    }
    
    if (strcmp(command,"-r")==0){
        printf("Agregar contenido a un archivo\n");
    }
	
    //int fd = open("nuevo.tar",O_RDWR);

    //openFile(fd,&archive);
    //listFiles(fd,archive);

    return 0;
}
*/