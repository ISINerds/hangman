#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../utils/words-handler.h"
typedef struct Dictionary {
    char value;
    struct Dictionary* swap;  
    struct Dictionary* next;  
} Dictionary;

Dictionary* addWord(Dictionary* dictionary,char * word);
void displayDictionary(Dictionary* dic);
Dictionary* AddAll(Dictionary* dictionary, char * path);
int exists(Dictionary* dictionary, char * word);
void showAllWords(Dictionary *dic);
void visualize(Dictionary*  dic, char* path);
int* searchLetter(Dictionary* dic, char* keyword , char letter);
Dictionary* removeWord(Dictionary *dic, char * word);

Dictionary* createNode(char value){
    Dictionary* node = (Dictionary*)malloc(sizeof(Dictionary));
    if(node){
            node->value = value;
            node->swap = NULL;
            node->next=NULL;
    }
    return node;
}

Dictionary* addWord(Dictionary* dictionary,char * word){
    if(*word!='\0'){
        if(dictionary==NULL){
            dictionary = createNode(*word);
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
        else if(dictionary->value < *word){
            dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
        }
        else if(dictionary->value == *word){
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
        else if(dictionary->value > *word){
            Dictionary* node = (Dictionary*)malloc(sizeof(Dictionary));
            if(node){
                    node->value = *word;
                    node->swap = dictionary;
                    node->next=NULL;
            }
            dictionary = node;
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
    }

    else{
        if(dictionary == NULL){
            dictionary = createNode('\0');
            return dictionary;
        }
        else if(dictionary->value!='\0'){
            Dictionary* temp = dictionary;
            dictionary = createNode('\0');
            dictionary->swap = temp;
            // dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
        }
        else {
            return dictionary;
        }
    }
}

void displayDictionary(Dictionary* dic){
    if (dic != NULL) {
        if(dic->value!='\0'){
            printf("%c ", dic->value);
        }
        else{
            printf("%c",'#');
        }
        displayDictionary(dic->next);
        displayDictionary(dic->swap);
    }
}

Dictionary* AddAll(Dictionary* dictionary, char * path){
    Words words = parser(path);
    for(int i=0;i<words.wordsArraySize;i++){
        dictionary = addWord(dictionary,words.wordsArray[i]);
    }
    freeWordsArray(words);
    return dictionary;
}

int exists(Dictionary* dictionary, char * word){
    if(dictionary == NULL ){
        return 0;
    }
    if(*word =='\0' && dictionary->value=='\0' ){
        return 1 ;
    }
    if((*word =='\0' && dictionary->value!='\0') || (dictionary->value> *word) ){
        return 0;
    }
    else if (dictionary->value < *word){
        return exists(dictionary->swap,word);
    }
    else if(dictionary->value == *word){
        return exists(dictionary->next,word+1);
    }
}

void show(Dictionary *dic, char* word, int idx) {
    if(!dic) return;
    if(dic->value == '\0') {
        printf("%s\n", word);
    }
    word[idx++] = dic->value;
    show(dic->next, word, idx);
    word[--idx] = '\0';
    show(dic->swap, word, idx);
}

void showAllWords(Dictionary *dic) {
    char* word = (char*) malloc(100 * sizeof(char));
    show(dic, word, 0);
    free(word);
}

void printDictionary (Dictionary* dic, char* id, int idx, FILE *file ) {
    if(!dic) {
        return;
    }
    // if(dic->swap != dic->next && (!dic->swap || !dic->next)) {
        // only one of them is null
        // if(!dic->swap) {
            // fprintf(file, "null%s [shape=point]\n", id);
            // fprintf(file, "%s -> null%s \n", id, id);
        // }
    // }
    if(dic->value != '\0') fprintf(file, "%s[label=\"%c\"]\n",id, dic->value);
    else fprintf(file, "%s[label=\"\\\\0\"]\n",id);
    if(dic->next) {
        fprintf(file, "%s -> %s0;\n", id, id);
        id[idx++] = '0';
        printDictionary(dic->next, id, idx, file);
        id[--idx] = '\0';
    }
    if(dic->swap) {
        fprintf(file, "%s -> %s1;\n", id, id);
        fprintf(file, "{rank = same; %s;%s1;}\n", id, id);
        id[idx++] = '1';
        printDictionary(dic->swap, id, idx, file);
        id[--idx] = '\0';
    }
}

void visualize(Dictionary*  dic, char* path){
    // Open a file for writing
    FILE *file = fopen(path, "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("ERROR : Could not open the file %s\n", path);
        exit(EXIT_FAILURE);
    }

    char* id = (char*) malloc(100 * sizeof(char));
    id[0] = '0';
    // Write to the file
    fprintf(file, "digraph {\n");
    printDictionary(dic, id, 1, file);
    fprintf(file, "}\n");

    free(id);
    // Close the file
    fclose(file);

    //generate the output.svg
    system("dot -Tpng -o output.png graph.dot");

    printf("Data written to the file successfully.\n");
}

int search(Dictionary *dic, char* keyword , char letter, int* positions , int indexInWord, int indexInArray) {
    if(!dic) return 0;
    if(dic-> value == '\0' && indexInWord == strlen(keyword)) {
        return 1;
    }
    if(dic->value == keyword[indexInWord]) {
        if(dic->value == letter) positions[indexInArray++] = indexInWord;
        return search(dic->next, keyword, letter, positions, indexInWord+1, indexInArray);
    }
    else if(dic->value < keyword[indexInWord]) {
        return search(dic->swap, keyword, letter, positions, indexInWord, indexInArray);
    }
    else return 0;

}

int* searchLetter(Dictionary* dic, char* keyword , char letter){
    int * positions = (int*) malloc(strlen(keyword) * sizeof(int));
    // initialize the positions array by -1
    for(int i=0;i<strlen(keyword);i++) positions[i] = -1;
    if(search(dic, keyword, letter, positions, 0, 0)) return positions;
    free(positions);
    return NULL;
}

Dictionary* rmWord(Dictionary *dic, char * word, int index) {
    if(!dic) {
        return NULL;
    }
    if(dic->value == '\0' && index == strlen(word)) {
        Dictionary* tmp = dic->swap;
        free(dic);
        return tmp;
    }
    if(dic->value == word[index]) {
        dic->next = rmWord(dic->next, word, index+1);
        if(!dic->next) {
            //remove this node
            Dictionary* tmp = dic->swap;
            free(dic);
            return tmp;
        }
        else {
            return dic;
        }
    }
    else if(dic->value < word[index]) {
        dic->swap = rmWord(dic->swap, word, index);
        return dic;
    }
    else {
        return dic;
    }
}

Dictionary* removeWord(Dictionary *dic, char * word) {
    return rmWord(dic, word, 0);
}