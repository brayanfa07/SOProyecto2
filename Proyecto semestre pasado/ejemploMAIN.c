#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int MAX(int val1, int val2){
	if(val1 > val2){
		return val1;
	}
	else{
		return val2;
	}
}

int main(int argc, char * argv[]){
	//printf("ARGC[]: %d\n", argc);
    if (((argc == 2) && (strncmp(argv[1], "help", MAX(strlen(argv[1]), 4)))) && (argc != 3)){
        fprintf(stderr, "\nCOMANDO USO: %s <OPCIONES> <ARCHIVOSALIDA.tar> <ARCHIVO1> <ARCHIVO2> ...<ARCHIVON>\n", argv[0]);
        fprintf(stderr, "\nAYUDA: %s ayuda\n", argv[0]);
        return -1;
    }

    if (argc == 2){
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
         f = 0,				// file
         r = 0;     		// append

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
