#include "histIO.h"

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
  }

  // Free image
  free_cimage(name,&cim);

  return(histogram);
}

/**
 * @brief      Prints the histograms in the given file.
 *
 * @param      resultfile  The file in which printing 
 * @param      hist        The histogram to print
 */
void writeHist(char* filename, float* hist){
  char* prefix = "bin/" ;
  char* prefixedfilename = malloc(strlen(filename) + strlen(prefix) + 1);
  strcpy(prefixedfilename, prefix);
  if(strncmp("http://", filename, strlen("http://")) == 0){
    strcat(prefixedfilename, &filename[33]); //delete url prefix of file
  }
  else{
    strcat(prefixedfilename, &filename[11]); //delete url prefix of file
  }

  // Open resulting file
  FILE* resultfile = fopen(prefixedfilename,"w");
  if (resultfile == NULL){
    printf("Couldn't open %s for writing\n", prefixedfilename);
    exit(EXIT_FAILURE);
  }

  // fprintf(resultfile, "\n");
  // for (int i = 0; i < NR * NG * NB; i++) {
  //   fprintf(resultfile, "%f\n", hist[i]);
  // }

  // Write histogram (NR * NG * NB elements of size sizeof(float)) in resultfile
  fwrite(hist, sizeof(float), NR * NG * NB, resultfile);

  fclose(resultfile);
}

/**
 * @brief      Returns an histogram stored in 'filename' file
 *
 * @param      filename  The filename
 *
 * @return     { description_of_the_return_value }
 */
float* readHist(char* filename){

  char* prefix = "bin/" ;
  char* prefixedfilename = malloc(strlen(filename) + strlen(prefix) + 1);
  strcpy(prefixedfilename, prefix);
  if(strncmp("http://", filename, strlen("http://")) == 0){
    strcat(prefixedfilename, &filename[33]); //delete url prefix of file
  }
  else{
    strcat(prefixedfilename, &filename[11]); //delete url prefix of file
  }

  FILE* file = fopen(prefixedfilename, "r");
  if (file == NULL){
    printf("Couldn't open %s for reading\n", prefixedfilename);
    exit(EXIT_FAILURE);
  }
  float* histogram = malloc(NR * NG * NB * sizeof(float));
  fread(histogram, sizeof(float), NR * NG * NB, file);

  fclose(file);

  return(histogram);
}

/**
 * @brief      Returns the distance between to vectors (of size NR * NG * NB)
 *
 * @param      hist1  The first vector
 * @param      hist2  The second vector
 *
 * @return     Distance between the two vectors
 */
float distance(float * hist1, float * hist2){

  // Euclidian distance between two vectors is equal to square root of sum for all dimensions of squares of difference of vector value on this dimension :
  // d(x, y) = sqrt(sum_i((x[i]-y[i])²)
  // http://www.iecl.univ-lorraine.fr/~Francoise.Geandier/topo-aH-chap1.pdf
  float sum ;
  for (int i = 0; i < NR * NG * NB ; i++){
    sum += (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
  }
  return sum ;
}

/**
 * @brief      Creates an html file with the 'size' images with minimal distance to 'source' image
 *
 * @param      source     The image to compare images with
 * @param      urlList    The images' url list
 * @param      distances  The distances KEY array
 * @param[in]  size       The number of images to be written
 * @param      htmlFile   The html output file
 */
void writeHTML(char* source, char** urlList, KEY* distances, int size, FILE* htmlFile){

  fprintf(htmlFile, "<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"utf-8\">\n\t\t<title>Distances</title>\n\t</head>\n\t<body>\n");


  fprintf(htmlFile, "\t\t<img height='380' src='%s' />\n", source);

  for (int i = 0; i < size; i++){
    fprintf(htmlFile, "\t\t<img height='380' src='%s' />\n", urlList[distances[i].k]);    
  }

  fprintf(htmlFile, "\t</body>\n</html>");

}