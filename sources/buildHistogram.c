#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "histIO.h"


/**
 * @brief      Downloads every file in the list.
 *
 * @param      list    The file list
 * @param      length  The length of the file list
 */
void downloadFiles(char** list, int* length){
  char *command;

  for (int i = 0; i < *length; i++){
    asprintf(&command,"wget \"%s\" -P ressources/",list[i]);
    if ((popen(command,"r")) == NULL) {
      printf("Can't download file %s",list[i]);
      exit(1);
    }
    free(command);
  }
}




int main(int argc, char *argv[]){

  if(argc < 2){
    printf("%s <list file> [-d]\n", argv[0]);
  }
  else{

    FILE* listfile ; 
    // char** list ;
    int length ;
    float* hist;

    // Load list file
    listfile = fopen(argv[1],"r");
    if (listfile == NULL){
      printf("Couldn't open listfile for reading\n");
      exit(EXIT_FAILURE);
    }

    char** list = createList(listfile, &length);

    if(argc >= 3 && strcmp(argv[2], "-d") == 0){
      printf("Downloading...\n");
      downloadFiles(list, &length);  
    }


    
    // For every image of list 
    for (int i = 0; i < length; i++){
      // Build histogram
      hist = buildHistogram(list[i]);
      // Print histogram   
      writeHist(list[i], hist);
    }

    // !!! FIXME this part is test, uncomment code above for real execution !!!
    // For every image of list 
    // for (int i = 0; i < 10; i++){
    //   // Build histogram
    //   hist = buildHistogram(list[i]);
    //   // Print histogram   
    //   writeHist(list[i], hist);
    // }
    // !!! FIXME !!!


    for (int i = 0; i < length; i++){
      free(list[i]);
    }
    free(list);
    fclose(listfile);


    exit(EXIT_SUCCESS);
  }
}