#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Dictionary {
    char value;
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

void testParser(){
    Words words = parser("../words.txt");
    for(int i=0;i<words.wordsArraySize;i++){
        printf("%d-%s \n",i+1,words.wordsArray[i]);
    }
    freeWordsArray(words);
}
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
        printf("current = %c\n",*word);
        if(dictionary==NULL){
            dictionary = createNode(*word);
            printf("test1");
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
        else if(dictionary->value < *word){
            printf("test2");
            dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
        }
        else if(dictionary->value == *word){
            printf("test3");
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
            printf("test4");
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
    }

    else{
        if(dictionary == NULL){
            printf("test5");
            dictionary = createNode('\0');
            return dictionary;
        }
        else{
            dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
            printf("test6");
        }
    }
}

void displayDictionary(Dictionary* dic){
    if (dic != NULL) {
        printf("%c ", dic->value);
        displayDictionary(dic->next);
        displayDictionary(dic->swap);
    }
}
Dictionary* AddAll(Dictionary* dictionary, char * path){
    Words words = parser(path);
    // I will add exixts function ( I will not add the existing words)
    for(int i=0;i<words.wordsArraySize;i++){
        dictionary = addWord(dictionary,words.wordsArray[i]);
    }
    freeWordsArray(words);
    return dictionary;
}
//---- N3dhir

int main(){
    Dictionary* dictionary = NULL;
    dictionary = AddAll(dictionary,"C:\\Users\\Mehrez\\hangman\\words.txt");
    displayDictionary(dictionary);
    return 0;
}