#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "proc.h"
#include "histIO.h"

int main(int argc, char *argv[]){

    if(argc < 5){
        printf("%s <image> <list file> <number of images to display> <html output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else{
        int toDisplay ;
        if (sscanf (argv[3], "%i", &toDisplay) != 1) {
            printf("Third argument must be an integer\n");
            exit(EXIT_FAILURE);
        }

        // Create the result file
        FILE* htmlFile = fopen(argv[4],"w");
        if (htmlFile == NULL){
            printf("Couldn't open htmlFile for reading\n");
            exit(EXIT_FAILURE);
        }

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

        for (int i = 0; i < listLength; i++){
            histograms[i] = readHist(list[i]);
            distances[i].k = i ;
            distances[i].d = distance(imageHist, histograms[i]);
        }
        qsort(distances, listLength, sizeof(KEY), keyCompare);
        writeHTML(argv[1], list, distances, toDisplay, htmlFile);


        // !!! FIXME this part is test, uncomment code above for real execution !!!
        // For every image of list 
        // for (int i = 0; i < 10; i++){
        //     histograms[i] = readHist(list[i]);
        //     distances[i].k = i ;
        //     distances[i].d = distance(imageHist, histograms[i]);
        //     printf("Distance to histograms[%i] = %f\n", i, distances[i].d);
        // }
        // qsort(distances, 10, sizeof(KEY), keyCompare);
        // writeHTML(argv[1], list, distances, 10, htmlFile);
        // writeHTML(argv[1], list, distances, 10, htmlFile);
        // !!! FIXME !!!






        exit(EXIT_SUCCESS);
    }
}