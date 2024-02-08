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

void addRanking(char *path,char* username,float score);
Rankings parserRankings(char * path);
void freePlayersArray(Rankings rankings);

void addRanking(char *path,char* username,float score){
    Player player = {username,score};
    FILE *file = fopen(path, "a");
    if (!file) {
        printf("ERROR : Could not open the file %s\n", path);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s;%.2f\n", player.username, player.score);
    fclose(file); 
}

Rankings parserRankings(char * path){
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
    for (int i = 0; i < rankings.numberOfPlayers; i++) {

        if (fscanf(file, "%s", line) != 1) {
            freePlayersArray(rankings);
            fclose(file);
            printf("ERROR : Reading lines from the file failed \n");
            exit(EXIT_FAILURE);
        }
        // After reading line , we will extract username and score;
        rankings.players[i].username = (char*)malloc(100*sizeof(char));

        if (rankings.players[i].username == NULL) {
            freePlayersArray(rankings);
            fclose(file);
            printf("ERROR : Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        if (sscanf(line, "%[^;];%f", rankings.players[i].username, &rankings.players[i].score) == 2) {
        }
        else {
            freePlayersArray(rankings);
            fclose(file);
            printf("ERROR : Parsing line failed : %s\n", line);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return rankings;
}

void freePlayersArray(Rankings rankings){
    for (int i = 0; i < rankings.numberOfPlayers; i++) {
        free(rankings.players[i].username);//malloc for username
    }
    free(rankings.players);//malloc for players
}
// #include "./includes/utils/rankings-handler.h"
// int main(){


//    addRanking("./src/rankings.txt","mehrez",01.365);
//    addRanking("./src/rankings.txt","nadhir",0);
//    addRanking("./src/rankings.txt","mohamed",10045.36958);

//    Rankings rankings =  parser("./src/rankings.txt");
//    for(int i=0 ; i<rankings.numberOfPlayers;i++){
//       printf("username = %s ||score %f\n",rankings.players[i].username,rankings.players[i].score);
//    }
//    freePlayersArray(rankings);
//    for(int i=0 ; i<rankings.numberOfPlayers;i++){
//       printf("username = %s ||score %f\n",rankings.players[i].username,rankings.players[i].score);
//    }
//    return 0;
// }