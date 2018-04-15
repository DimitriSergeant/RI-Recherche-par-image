#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "proc.h"
#include "histIO.h"

int main(int argc, char *argv[]){

    if(argc < 3){
        printf("%s <image> <list file> [-d]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else{

        // Load list file
        FILE* listfile = fopen(argv[2],"r");
        if (listfile == NULL){
            printf("Couldn't open listfile for reading\n");
            exit(EXIT_FAILURE);
        }
        int listLength;
        char** list = createList(listfile, &listLength);

        float* imageHist = buildHistogram(argv[1]);
        float** histograms = malloc(listLength * sizeof(float *));

        KEY * distances = malloc(listLength * sizeof(KEY));

        // for (int i = 0; i < listLength; i++){
            // histograms[i] = readHist(list[i]);
            // distances[i].k = i ;
            // distances[i].d = distance(imageHist, histograms[i]);
            // printf("Distance to histograms[%i] = %f\n", i, distances[i].d);
        // }
        // qsort(distances, listLength, sizeof(KEY), keyCompare);


        // !!! FIXME this part is test, uncomment code above for real execution !!!
        // For every image of list 
        for (int i = 0; i < 10; i++){
            histograms[i] = readHist(list[i]);
            distances[i].k = i ;
            distances[i].d = distance(imageHist, histograms[i]);
            printf("Distance to histograms[%i] = %f\n", i, distances[i].d);
        }
        qsort(distances, 10, sizeof(KEY), keyCompare);
        for (int i = 0; i < 10; i++){
            printf("%i) %i -> %f\n", i, distances[i].k, distances[i].d);
        }
        // !!! FIXME !!!




        exit(EXIT_SUCCESS);
    }
}