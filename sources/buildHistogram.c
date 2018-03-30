#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>


#include "rdjpeg.h"

#define NR 4 
#define NG 4 
#define NB 4 


/**
 * Builds the histogram of the given image
 */
/**
 * @brief      Builds an histogram.
 *
 * @param      name       The name of the image file
 * @param      histogram  The built histogram
 */
void buildHistogram(char* name, float* histogram){

  int r, g, b, currentHistogramIndex;
  CIMAGE cim ; 

  // Open image 
  read_cimage(name,&cim);

  // Build histogram
  for (int i = 0; i < cim.nx; i++) {
    for (int j = 0; j < cim.ny; j++) {
      r = (cim.r[i][j] * NR) / 256 ;
      g = (cim.g[i][j] * NG) / 256 ;
      b = (cim.b[i][j] * NB) / 256 ;
      currentHistogramIndex = r + NR * g + NR * NG * b ;
      histogram[currentHistogramIndex]++ ;
    }
  }

  // Free image
  free_cimage(name,&cim);
}

/**
 * @brief      Creates the list of filenames given an open file containing each filename on every line.
 *
 * @param      listfile  The open file
 * @param      list      The list of filenames
 * @param      length    The length of the list
 */
char** createList(FILE* listfile,  int* length){

  char** list;
  char* currentFileName = NULL; 
  int lineLength ; 
  size_t len = 0;
  int index = 0 ;
  int numberOfLines = 0 ; 

  // Code from https://www.geeksforgeeks.org/c-program-count-number-lines-file/
  char c ;
  for (c = getc(listfile); c != EOF; c = getc(listfile)){
    if (c == '\n'){
      numberOfLines++;
    }
  }

  list =  malloc(numberOfLines * sizeof(char *));
  
  rewind(listfile);

  // For every line : 
  // - remove trailing '\n'
  // - allocate a corresponding length string in the 'list' array and put the read line into it, 
  // - increment 'length' ("list"'s length)
  while ((lineLength = getline(&currentFileName, &len, listfile)) != -1) {
    if (currentFileName[strlen(currentFileName) - 1] == '\n') {
      currentFileName[strlen(currentFileName) - 1] = '\0';
    }
    list[index] = malloc((lineLength + 1) * sizeof(char));
    list[index] = strdup(currentFileName) ;
    index++;
    
  }

  *length = index ;
  return list;
}

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

/**
 * @brief      Prints the histograms in the given file.
 *
 * @param      resultfile  The file in which printing 
 * @param      hist        The histogram to print
 */
void print(FILE* resultfile, float* hist){
  fprintf(resultfile, "\n");
  for (int i = 0; i < NR * NG * NB; i++) {
    fprintf(resultfile, "%f\n", hist[i]);
  }
}

int main(int argc, char *argv[]){

  if(argc < 3){
    printf("%s <list file> <result file> [-d]\n", argv[0]);
  }
  else{

    FILE* listfile ; 
    FILE* resultfile ;
    // char** list ;
    int length ;
    float* hist = NULL;

    // Load list file
    listfile = fopen(argv[1],"r");
    if (listfile == NULL){
      printf("Couldn't open listfile for reading\n");
      exit(EXIT_FAILURE);
    }

    char** list = createList(listfile, &length);

    if(strcmp(argv[3], "-d")){
      downloadFiles(list, &length);  
    }
    

    // Open resulting file
    resultfile = fopen(argv[2],"w");
    if (resultfile == NULL){
      printf("Couldn't open resultfile for writing\n");
      exit(EXIT_FAILURE);
    }
    
    // For every image of list 
    for (int i = 0; i < length; i++){
      // Build histogram
      buildHistogram(list[i], hist);
      // Print histogram   
      print(resultfile, hist);
    }

    for (int i = 0; i < length; i++){
      free(list[i]);
    }
    free(list);
    fclose(listfile);
    fclose(resultfile);
    exit(EXIT_SUCCESS);
  }
}

//     int i,j,n,nx,ny,nb;
//     CIMAGE cim;
//     FILE* f ;

//     f = fopen(argv[2],"w");
//     if (f == NULL){
//       printf("Couldn't open file for writing\n");
//       exit(EXIT_FAILURE);
//     }


//     /*------------------------------------------------*/
//     /* lecture d'une image requête                    */
//     /*------------------------------------------------*/
//     read_cimage(argv[1],&cim);
//     /*------------------------------------------------*/
//     /* affichage des valeurs pour le premier bloc 8x8 */
//     /* comme exemple de traitement                    */
//     /*------------------------------------------------*/
//     printf("Largeur de l'image : %d\n",cim.nx);
//     printf("Heuteur de l'image : %d\n",cim.ny);

//     float histogram[NR * NG * NB];
//     int r, g, b, currentHistogramIndex;


//     // Building histogram
//     printf("Building histogram\n");
//     for (i = 0; i < cim.nx; i++) {
//       for (j = 0; j < cim.ny; j++) {
//         r = (cim.r[i][j] * NR) / 256 ;
//         g = (cim.g[i][j] * NG) / 256 ;
//         b = (cim.b[i][j] * NB) / 256 ;
//         currentHistogramIndex = r + NR * g + NR * NG * b ;
//         histogram[currentHistogramIndex]++ ;
//       }
//     }

//     // Normalizing and writing histogram
//     printf("Normalizing and writing histogram\n");
//     fprintf(f, "%i\n", cim.nx);
//     fprintf(f, "%i\n", cim.ny);
//     for (i = 0; i < NR * NG * NB; i++) {
//       histogram[i] /= cim.nx * cim.ny ;
//       if(i < 10){
//         fprintf(f, "%f\n", histogram[i]);
//       }
//     }
//     fclose(f);
//     exit(0);

//   }
// }