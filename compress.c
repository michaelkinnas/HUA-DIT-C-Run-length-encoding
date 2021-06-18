#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char c;
    int count;
} run;

int main(int argc, char **argv) {
    FILE *fpu = fopen(argv[1], "r");
    if (fpu == NULL) {
        printf("Error opening file \"%s\"\n", argv[1]);
        return 0;
    }

    FILE *fpc = fopen(argv[2], "w");
    if (fpc == NULL) {
        printf("Error opening file \"%s\"\n", argv[2]);
        return 0;
    }

    run chunk; //this is the run struct    
    
    //compression algorithm
    char current, last;
    int charCounter = 1;
    int totalCharCounter = 0;
    int totalRunCounter = 0;
    last = fgetc(fpu);     
    do {     
        current = fgetc(fpu);
        totalCharCounter++;
        if (last != current) {
            chunk.c = last;
            chunk.count = charCounter;
            charCounter = 1;
            last = current;
            fwrite(&chunk, sizeof(chunk), 1, fpc);            
            totalRunCounter++;            
        } else {
            charCounter++;
        }
    } while (current != EOF);

    fclose(fpu);
    fclose(fpc);

   
    printf("\nFile \"%s\" size uncompressed is %ld bytes\n", argv[1], sizeof(char) * totalCharCounter);
    printf("File \"%s\" size compressed is %ld bytes\n", argv[2], sizeof(run) * totalRunCounter);
    printf("Operation complete.\n");
}