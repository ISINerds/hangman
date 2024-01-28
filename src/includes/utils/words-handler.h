#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define score ranges for difficulty levels
// #define EASY_LEVEL_LIMIT 10
#define MEDIUM_LEVEL_LIMIT 20
#define DIFFICULT_LEVEL_LIMIT 30

typedef struct {
    char ** wordsArray;
    int wordsArraySize;
} Words;

typedef enum {
    EASY = 1,
    MEDIUM = 2,
    HARD =3,
} Level;

Words parser(char* path);
void freeWordsArray(Words words);
// char * randomWord(char * path);

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

void freeWordsArray(Words words){
    for (int i = 0; i < words.wordsArraySize; i++) {
        free(words.wordsArray[i]);
    }
    free(words.wordsArray);
}

// char * randomWord(char * path){
//     srand(time(NULL));
//     Words words = parser(path);
//     char * result = (char*) malloc(100*sizeof(char));
//     strcpy(result,words.wordsArray[rand() % words.wordsArraySize]);
//     freeWordsArray(words);
//     return result;
// }

float evaluateWord(char * word){
    return 0.0;
}
char* randomWord(Words words, Level level){
    Level* levelWords = (Level*)malloc(words.wordsArraySize * sizeof(Level)); // array of level of each word
    int easyWordsCpt =0;
    int mediumWordsCpt = 0;
    int hardWordsCpt = 0;

    // We will fill an array of level and calculate the number of words of each level
    for(int i=0; i<words.wordsArraySize ; i++){

        char* word = words.wordsArray[i];

        float scoreWord = evaluateWord(word);

        if(scoreWord<MEDIUM_LEVEL_LIMIT){
            easyWordsCpt++;
            levelWords[i] = EASY;
        }
        else if(scoreWord < DIFFICULT_LEVEL_LIMIT){
            mediumWordsCpt++;
            levelWords[i]= MEDIUM;
        }
        else{
            hardWordsCpt++;
            levelWords[i]=HARD;
        }
    }

    // We will choose a random number between 1 and the numberOfWords of a certain level
    srand((unsigned int)time(NULL));
    int randomIndex = -1;
    if(level == EASY){
        randomIndex = (rand() % easyWordsCpt) +1;
    }
    else if(level == MEDIUM){
        randomIndex = (rand() % mediumWordsCpt) +1;
    }
    else{
        randomIndex = (rand() % hardWordsCpt) +1;
    }


    // We will choose the word with randomIndex 
    for(int i=0;i<words.wordsArraySize;i++){
        // If the level of this word is the same as the level mentionned
        if(levelWords[i] == level){
            randomIndex--;
        }
        if(randomIndex==0){
            // the selected word
            char* word  = (char*)  malloc(100 * sizeof(char));
            strcpy(word, words.wordsArray[i]);
            return word;
        }
    }
    exit(EXIT_FAILURE); // In Case No Word was choosen

}