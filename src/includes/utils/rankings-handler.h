#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player{
    char* username;
    float score;
} Player;

typedef struct Rankings{
    Player* players;
    int numberOfPlayers;
} Rankings;

void addRanking(char *path,Player player);
Rankings parser(char * path);


void addRanking(char *path,Player player){
    FILE *file = fopen(path, "a");
    if (!file) {
        printf("ERROR : Could not open the file %s\n", path);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s;%d\n", player.username, player.score);
    fclose(file); 
}

Rankings parser(char * path){
    Rankings rankings;
    rankings.numberOfPlayers=0;
    FILE *file = fopen(path, "r");
    char line[100]; 

    if (file == NULL) {
        printf("ERROR : Could not open the file %s\n", path);
        exit(EXIT_FAILURE);
    }

    // Count the lines
    while (fscanf(file, "%s", line) == 1) {
        rankings.numberOfPlayers++;
    }

    // Reset the file position to the beginning
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the words array
    rankings.players = (Player*)malloc(rankings.numberOfPlayers * sizeof(Player));

    if (rankings.players == NULL) {
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

void freePlayersArray(Rankings rankings){
    for (int i = 0; i < rankings.numberOfPlayers; i++) {
        free(rankings.players[i]);
    }
    free(words.wordsArray);
}