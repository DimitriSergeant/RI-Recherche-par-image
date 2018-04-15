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
 * @brief      Builds the histogram of the given image
 *
 * @param      name       The name of the image file
 * @return     The built histogram
 */
float* buildHistogram(char* name){

  int r, g, b, currentHistogramIndex;
  CIMAGE cim ; 

  // Open image 
  read_cimage(name,&cim);

  float* histogram = calloc(NR * NG * NB, sizeof(float));
  // for (int i = 0; i < NR * NG * NB; i++){
  //   histogram[i] = (float*) malloc(sizeof(float));
  // }

  printf("Building histogram for %s\n", name);

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

  // Normalizing histogram
  for (int i = 0; i < NR * NG * NB; i++) {
    histogram[i] /= cim.nx * cim.ny ;
    printf("%f\n", histogram[i]);
  }

  // Free image
  free_cimage(name,&cim);

  return(histogram);
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
void printHist(char* filename, float* hist){
  char* prefix = "bin/" ;
  char* prefixedfilename = malloc(strlen(filename) + strlen(prefix) + 1);
  strcpy(prefixedfilename, prefix);
  strcat(prefixedfilename, &filename[33]); //delete url prefix of file

  // Open resulting file
  FILE* resultfile = fopen(prefixedfilename,"w");
  if (resultfile == NULL){
    printf("Couldn't open %s for writing\n", prefixedfilename);
    exit(EXIT_FAILURE);
  }

  printf("Printing in %s\n", prefixedfilename);
  // fprintf(resultfile, "\n");
  // for (int i = 0; i < NR * NG * NB; i++) {
  //   fprintf(resultfile, "%f\n", hist[i]);
  // }

  // Write histogram (NR * NG * NB elements of size sizeof(float)) in resultfile
  fwrite(hist, sizeof(float), NR * NG * NB, resultfile);

  fclose(resultfile);
}

int main(int argc, char *argv[]){

  if(argc < 2){
    printf("%s <list file> <result file> [-d]\n", argv[0]);
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

    
    // // For every image of list 
    // for (int i = 0; i < length; i++){
    //   // Build histogram
    //   hist = buildHistogram(list[i]);
    //   // Print histogram   
    //   printHist(list[i], hist);
    //   printf("\n");
    // }

    // !!! FIXME this part is test, uncomment code above for real execution !!!
    // For every image of list 
    for (int i = 0; i < 10; i++){
      // Build histogram
      hist = buildHistogram(list[i]);
      // Print histogram   
      printHist(list[i], hist);
      printf("\n");
    }
    // !!! FIXME !!!


    for (int i = 0; i < length; i++){
      free(list[i]);
    }
    free(list);
    fclose(listfile);
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