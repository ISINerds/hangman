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
        // printf("%d-%s \n",i+1,words.wordsArray[i]);
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
        // printf("current = %c\n",*word);
        if(dictionary==NULL){
            dictionary = createNode(*word);
            // printf("test1");
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
        else if(dictionary->value < *word){
            // printf("test2");
            dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
        }
        else if(dictionary->value == *word){
            // printf("test3");
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
            // printf("test4");
            dictionary->next = addWord(dictionary->next,word+1);
            return dictionary;
        }
    }

    else{
        if(dictionary == NULL){
            // printf("test5");
            dictionary = createNode('\0');
            return dictionary;
        }
        else{
            dictionary->swap = addWord(dictionary->swap,word);
            return dictionary;
            // printf("test6");
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

void showAll(Dictionary *dic) {
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
    system("cat graph.txt | dot -Tsvg > output.svg");

    printf("Data written to the file successfully.\n");
}

int main(){
    Dictionary* dictionary = NULL;
    // dictionary = AddAll(dictionary,"../words.txt");
    // displayDictionary(dictionary);
    dictionary = addWord(dictionary, "ce");
    dictionary = addWord(dictionary, "ci");
    dictionary = addWord(dictionary, "ces");
    dictionary = addWord(dictionary, "di");
    dictionary = addWord(dictionary, "de");
    dictionary = addWord(dictionary, "des");
    // displayDictionary(dictionary);
    // showAll(dictionary);
    visualize(dictionary, "graph.txt");
    return 0;
}