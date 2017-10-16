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

int main(int argc, char *argv[]){
	struct tar  * archive;
	int fd = open("nuevo.tar",O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
		
	if (argc == 1){
        printf("No se encontró comando a ejecutar\nc");
        return 0;
    }

    char command[100];
    strcpy(command,argv[1]);


    if (strcmp(command,"-c")==0){
        printf("Creando archivo tar...\n");
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
