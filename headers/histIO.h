#ifndef HISTIO_H
#define HISTIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "rdjpeg.h"
#include "proc.h"

#define NR 4 
#define NG 4 
#define NB 4 

/**
 * @brief      Creates the list of filenames given an open file containing each filename on every line.
 *
 * @param      listfile  The open file
 * @param      list      The list of filenames
 * @param      length    The length of the list
 */
char** createList(FILE* listfile,  int* length);

/**
 * @brief      Builds the histogram of the given image
 *
 * @param      name       The name of the image file
 * @return     The built histogram
 */
float* buildHistogram(char* name);

/**
 * @brief      Prints the histograms in the given file.
 *
 * @param      resultfile  The file in which printing 
 * @param      hist        The histogram to print
 */
void writeHist(char* filename, float* hist);

/**
 * @brief      Returns an histogram stored in 'filename' file
 *
 * @param      filename  The filename
 *
 * @return     { description_of_the_return_value }
 */
float* readHist (char* filename);

/**
 * @brief      Returns the distance between to vectors (of size NR * NG * NB)
 *
 * @param      hist1  The first vector
 * @param      hist2  The second vector
 *
 * @return     Distance between the two vectors
 */
float distance(float * hist1, float * hist2);

#endif
