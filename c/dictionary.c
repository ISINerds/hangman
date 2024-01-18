#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Dictionary {
    char* value;
    struct Dictionary* swap;  
    struct Dictionary* next;  
} Dictionary;

typedef struct {
    char ** wordsArray;
    int wordsArraySize;
} Words;
//---- Mehrez
void freeWordsArray(Words words){
    for (int i = 0; i < words.wordsArraySize; i++) {
        free(words.wordsArray[i]);
    }
    free(words.wordsArray);
}

Words parser(char* path){

    Words words;
    words.wordsArraySize=0;
    FILE *file = fopen(path, "r");
    char word[100]; 

    if (file == NULL) {
        printf("ERROR : Could not open the file %s\n", path);
        exit(EXIT_FAILURE);
    }

    // Count the words
    while (fscanf(file, "%s", word) == 1) {
        words.wordsArraySize++;
    }

    // Reset the file position to the beginning
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the words array
    words.wordsArray = (char **)malloc(words.wordsArraySize * sizeof(char *));
    if (words.wordsArray == NULL) {
        fclose(file);
        printf("ERROR : Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read each word again and store it in the words array
    for (int i = 0; i < words.wordsArraySize; i++) {

        if (fscanf(file, "%s", word) != 1) {
            freeWordsArray(words);
            fclose(file);
            printf("ERROR : Reading words from the file failed \n");
            exit(EXIT_FAILURE);
        }

        words.wordsArray[i] = (char *)malloc((strlen(word) + 1) * sizeof(char));
        if (words.wordsArray[i] == NULL) {
            freeWordsArray(words);
            fclose(file);
            printf("ERROR : Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(words.wordsArray[i], word);
    }
    fclose(file);
    return words;
}
//---- N3dhir

int main(){
    
    return 0;
}