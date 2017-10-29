/*star.h 
 * FILE VERSION 007
 * April 23, 2017
 * 
 * Operative Systems's Principles
 * Project II
 *
 */



struct tar{
    int beginFile;
    char completeName[256];
    union {
        struct {
            char name[100];             // file name
            char mode[8];               // permissions
            char uid[8];                // user id (octal)
            char gid[8];                // group id (octal)
            char size[12];              // size (octal)
            char mtime[12];             // modification time (octal)
            char check[8];              // sum of unsigned characters in block block, with spaces in the check field while calculation is done (octal)
            char link;                  // link indicator
            char link_name[100];        // name of linked file
        };
        char blockInformation[512];
    };


    struct tar * next;

};


int calculatePad(int size){
    return 512 - size % 512;
}

//Method that appends to a final of the archive
int writeEnd(int fd){
    for (int i = 0;i<1024;i++){
        if (write(fd,"\0",1)!=1){
            printf("Error de escritura\n");
            break;
        }

    }
}

int writeFileInformation(int fd,char * buf, int size){
    int wrote = 0, rc;
    while ((wrote < size) && ((rc = write(fd, buf + wrote, size - wrote)) > 0)){
        wrote += rc;
    }

    return wrote;

}


unsigned int oct2uint(char * oct, unsigned int size){
    unsigned int out = 0;
    int i = 0;
    while ((i < size) && oct[i]){
        out = (out << 3) | (unsigned int) (oct[i++] - '0');
    }
    return out;
}


int writeTableInformation(int fd, const char * files[], int total){
    int start = 278 * total;
    int oldSize = 0;

    printf("===========> Display table information \n");


    for (int i =0;i<total;i++){

        char size[12];
        char buffer[256];
        memset(buffer,0,256);

        struct stat actualFile;
        if (lstat(files[i],&actualFile)){
            return -1;
        }
        sprintf(size, "%011o", (int) actualFile.st_size);
        printf("Size: %s\n",size);


        char startPosition[10];
        memset(startPosition,0,10);

        write(fd,files[i],strlen(files[i]));
        printf("LEn size%zu\n",strlen(files[i]));
        for (int j=0;j<=(256-strlen(files[i]));j++){
            write(fd,"\0",1);
        }
        write(fd,size,12);
        if (i==0){
            start = start +2;
            oldSize = oct2uint(size,11);


        } else{
            start = start +512;
            int padSize = calculatePad(oldSize);
            if (padSize!=512){
                start = start + padSize;
            }
            start = start + oldSize;

        }
        sprintf(startPosition,"%d",start);

        write(fd,startPosition,10);
        oldSize = oct2uint(size,11);
    }
    return start;

}



// Calculate the checksum of the file
unsigned int calculate_checksum(struct tar * entry){
    // use 8 spaces (' ', char 0x20) in place of checksum string
    memset(entry -> check, ' ', sizeof(char) * 8);

    // sum of entire metadata
    unsigned int check = 0;
    for(int i = 0; i < 500; i++){
        check += (unsigned char) entry -> blockInformation[i];
    }
    sprintf(entry -> check, "%06o", check);

    entry -> check[6] = '\0';
    entry -> check[7] = ' ';
    printf("FISHIN\n");
    return check;
}

//Insert data in the archive
int  insertData(struct tar * actualTar, const char * file){

    printf("===========> Inserting data into the file tar \n");
    struct stat stFile;
    if (lstat(file,&stFile)){
        printf("PROBLEMAS\n");
        return -1;
    }
    

    strncpy(actualTar -> completeName, file, 100);
    strncpy(actualTar -> name, file, 100);
    printf("======> Inserting the file %s\n", actualTar->name);
    sprintf(actualTar -> mode, "%07o", stFile.st_mode & 0777);
    sprintf(actualTar -> uid, "%07o", stFile.st_uid);
    sprintf(actualTar -> gid, "%07o", stFile.st_gid);
    sprintf(actualTar -> size, "%011o", (int) stFile.st_size);
    sprintf(actualTar -> mtime, "%011o", (int) stFile.st_mtime);


    switch (stFile.st_mode & S_IFMT){
        case S_IFDIR:
            printf("HERE");
            break;
    }

    calculate_checksum(actualTar);
}

//Method that read data of the file
int readData(int fd,char * buf, int size){
    int got = 0, rc;
    while ((got < size) && ((rc = read(fd, buf + got, size - got)) > 0)) {
        got += rc;
    }

    return got;
}






int readZero(char * buf, size_t size){
    for(size_t i = 0; i < size; buf++, i++){
        if (* (char *) buf){
            return 0;
        }
    }
    return 1;
}

//Create an archive and create files inside it.
int writeFile(int fd, struct  tar ** archive, int totalFiles,const char * files[],int start){

    printf("===========> Inserting into the tar file...\n");
    struct tar ** actualTar = archive;

    char buf[512];
    int  position = 0;
    for (int i=0;i<totalFiles;i++) {
        *actualTar = malloc(sizeof(struct tar));
        insertData(*actualTar, files[i]);


        printf("Position %d\n",position);
        (*actualTar)->beginFile = position;



        writeFileInformation(fd, (*actualTar)->blockInformation, 512);



        printf("BUF %s\n",(*actualTar)->blockInformation);


        int f = open((*actualTar)->name, O_RDONLY);
        if (f < 0) {
            printf("Error: Could not open %s\n", files[i]);
        }


        int r = 0;
        while ((r = readData(f, buf, 512)) > 0) {
            if (writeFileInformation(fd, buf, r) != r) {

                printf("Could not write data\n");
            }
        }

        close(f);


        const unsigned int size = oct2uint((*actualTar) -> size, 11);

        printf("Size in files %d\n",size);

        const unsigned int pad = 512 - size % 512;

        if (pad != 512) {
            printf("PAD %d\n",pad);
            for (unsigned int j = 0; j < pad; j++) {

                if (writeFileInformation(fd, "\0", 1) != 1) {
                    printf("Error: Could not write padding data\n");
                }
            }
            position += pad;
        }
        position += size;
        actualTar = &((*actualTar)->next);


        // add metadata size
        position += 512;
    }
    writeEnd(fd);


}

int minValue( int x, int y){
    if (x<y){
        return x;
    }
    return y;
}

//Extract a file from tar archive
void extractFile(struct tar * actualfile,int fd){
    const unsigned int size = oct2uint(actualfile -> size, 11);
    int f = open(actualfile -> name, O_WRONLY | O_CREAT | O_TRUNC, oct2uint(actualfile -> mode, 7) & 0777);


    // move archive pointer to data location
    if (lseek(fd, 512 + actualfile->beginFile, SEEK_SET) == (off_t) (-1)){
        printf("No se puede extraer el archivo");
    }

    // copy data to file
    char buf[512];
    int got = 0;
    while (got < size){
        int r;
        if ((r = readData(fd, buf, minValue(size - got, 512))) < 0){
            printf("No se puede leer el archivo");
        }

        if (write(f, buf, r) != r){
            printf( "No se puede extraer el archivo: %s\n", actualfile->name);
        }

        got += r;
    }
    close(f);
}

//Extract all the files from tar archive
void extractFiles(struct tar * archive,int fd){
    struct tar * actualTar = archive;
    while (actualTar != NULL){
        extractFile(actualTar,fd);
        actualTar = actualTar->next;


    }
}

//List files from tar
void listFiles(int fd,struct tar * archive){
    struct tar * actualTar = archive;
    printf("HERE in list\n");
    while (actualTar!=NULL){
        printf ("%s\n",actualTar->name);
        actualTar = actualTar ->next;
    }

}


//Open file from a tar
int openFile(const int fd, struct tar ** archive) {

    unsigned int offset = 0;
    int count = 0;

    struct tar ** tar = archive;
    *tar = malloc(sizeof(struct tar));

    int startFilePointer = 269;
    char update = 1;
    for(count = 0;count<3 ; count++) {
        *tar = malloc(sizeof(struct tar));

        char buffer[512];
        lseek(fd, startFilePointer, SEEK_SET);
        read(fd,buffer,10);
        offset = atoi(buffer) +1;
        printf("Posicion: %d\n",offset);


        lseek(fd,offset, SEEK_SET);



        int result = readData(fd, (*tar)->blockInformation, 512);
        printf("Result of readData %d\n",result);


        printf("Tar name in reading %s\n",(*tar)->mtime);
        (*tar)->beginFile = offset;

        tar = &((*tar) -> next);
        startFilePointer = startFilePointer + 279;
    }
    return count;

}

struct tar * existFile(struct tar * archive,const char * fileName){
    struct tar * file = archive;
    while(file != NULL){
        if (strcmp(file->name,fileName)==0) {
            return file;
        }
        file = file->next;
    }
    return NULL;
}

void changeTableInformation(int totalFiles, int fd,const char * filename){
    int position = 0;

    for (int i=0;i<totalFiles;i++){
        char name[256];
        printf("position %d\n",position);

        lseek(fd, position, SEEK_SET);
        read(fd,name,256);
        printf("Name :%s\n",name);
        if (strcmp(name,filename)==0){
            lseek(fd, position, SEEK_SET);
            for (int j=0;j<257;j++){
                write(fd,"\0",1);
            }
            lseek(fd, position, SEEK_SET);
            write(fd,"---",3);
        }
        position = position + 279;
    }
}

void join (int start,int finishRead,int fd,int totalSize){
    int startPosition = start * 279;
    int finish = finishRead * 279;
    int size1 =0;
    int size2 = 0;
    char buffer[12];
    lseek(fd, startPosition+257, SEEK_SET);
    read(fd,buffer,12);
    size1 = oct2uint(buffer,11);
    lseek(fd,finish+257, SEEK_SET);
    read(fd,buffer,12);
    size2 = oct2uint(buffer,11);
    int newSize = size1 + size2;
    char saveSize[12];
    sprintf(saveSize, "%011o", newSize);
    lseek(fd, startPosition+257, SEEK_SET);
    for (int i =0;i<12;i++){
        write(fd,"\0",1);
    }
    lseek(fd, startPosition+257, SEEK_SET);
    write(fd,saveSize,12);

    int newPosition =startPosition + 279;
    for (int i=0;i<finishRead-start;i++){
        lseek(fd,newPosition,SEEK_SET);
        for (int j=0;j<278;j++){
            write(fd,"*",1);
        }
    }
}

//Join the space in the tar
void joinSpace(int fd,int totalFiles){
    int  whiteSpace[totalFiles];
    int position = 0;
    int startSpace = 0;
    for (int i =0;i<totalFiles;i++){
        char bufferName[3];
        int size = 0;
        char bufferSize[12];
        char bufferPosition[10];

        lseek(fd, position, SEEK_SET);
        read(fd,bufferName,3);
        if (strcmp(bufferName, "---")==0){
            whiteSpace[i] = 0;
        }
        else{
            whiteSpace[i] = 1;
        }
        position = position + 279;

    }
    int startEmpty = 0;
    int block=0;

    for (int i =0;i<totalFiles;i++){
        if (whiteSpace[i]==0 && block==0 ){
            startEmpty = i;
            block = 1;
        }
        if(block!=0 && whiteSpace[i]==1){
            if (i-startEmpty != 0){
                join(startEmpty,i,fd,totalFiles);
            }
            block = 0;
        }
    }
}

//Delete files inside a tar archive
void deleteFiles(struct tar ** archive, int fd, const char * files[],int totalFilesDelete,int totalFilesInTar){
    struct tar * newFile = *archive;
    for (int i =0;i<totalFilesDelete;i++){
        struct tar * deleteFile =  existFile(newFile,files[i]);
        if (deleteFile!=NULL){

            lseek(fd, deleteFile->beginFile, SEEK_SET);
            int position = deleteFile->beginFile;
            int size = oct2uint(deleteFile->size,11);
            int stop = 0;
            if (deleteFile->next->beginFile != 0){
                stop = deleteFile->next->beginFile;

            }
            else{

                stop = position +512;
                int padSize = calculatePad(size);
                if (padSize!=512){
                    stop = stop + padSize;
                }
                stop = stop  + size;
            }
            for (int j =position;j<stop;j++){
                write(fd,"\0",1);
            }
            changeTableInformation(totalFilesInTar,fd,files[i]);
        }
        newFile = newFile ->next;
    }
    joinSpace(fd,totalFilesInTar);
}

// Get name of a directory [Its necessary use argv[]]
char* getNameDirectory(char* fileName){
    char buf[PATH_MAX];
    char* nameDirectory = realpath(fileName, buf);
    return nameDirectory;
}

//Get the files inside of a directory
int getFileNames(char* oneFile){
	DIR *direction;
	struct dirent *ent;
	if ((direction = opendir (getNameDirectory(oneFile))) != NULL) {
	  // print all the files and directories inside the directory
		while ((ent = readdir (direction)) != NULL) {
		printf ("%s\n", ent->d_name);
	  }
	  closedir (direction);
	} else {
	   //Error opening the file
	  perror ("");
	  return EXIT_FAILURE;
	}
}

int searchSpace(int sizeDATA,int fd,const  char * fileName){
    int position = 0;
    printf("sizeData %d\n",sizeDATA);
    while(position!=1) {
        lseek(fd, position, SEEK_SET);
        char name[256];
        memset(name,0,256);
        char size[12];

        read(fd, name, 256);
        printf("NAME: %s\n",name);

        if (strcmp(name, "---")==0) {

            char size[12];
            lseek(fd, position + 257, SEEK_SET);
            read(fd, size, 12);
            int sizeNumber = oct2uint(size, 11);
            char positionFile[10];
            read(fd,positionFile,10);
            if (sizeDATA <= sizeNumber) {
                lseek(fd,position,SEEK_SET);
                write(fd,fileName,strlen(fileName));
                return atoi(positionFile);
            }

        }

        if (strcmp(name,"")==0){
            return 0;
        }

        position = position + 279;

    }

    return 0;

}


int addFile(const char * filename[],int fd,int total,struct tar ** archive){
    struct tar * file;
    struct tar * tar = *archive;
    int alternativeSize = 0;
    for (int i =0;i<total;i++){

        file = malloc(sizeof(struct  tar));
        insertData(file,filename[i]);
        int position = searchSpace(oct2uint(file->size,11),fd,filename[i]);

        if (position ==0){
            while(tar!=NULL){
                if (tar->beginFile != 0){
                    position = tar->beginFile;
                    alternativeSize = oct2uint(tar->size,11);
                }
                tar = tar->next;

            }
            position = position + 512 + alternativeSize;
            int pad = calculatePad(alternativeSize);
            if (pad != 512){
                position = position + pad;
            }
        }
        else{
            printf("HERE %d\n",position);
            char buf[512];
            printf("position %d\n",position);
            lseek(fd,position,SEEK_SET);
            writeFileInformation(fd, file->blockInformation, 512);


            int f = open(file->name, O_RDONLY);
            if (f < 0) {
                printf("Error: Could not open %s\n", filename[i]);
            }


            int r = 0;
            while ((r = readData(f, buf, 512)) > 0) {
                if (writeFileInformation(fd, buf, r) != r) {

                    printf("Could not write data\n");
                }
            }

            close(f);


        }
    }
}


