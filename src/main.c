#include <stdio.h>
#include "./includes/data-structures/dictionary.h"
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
    // dictionary = addWord(dictionary, "isissssiisiiisis");
    // displayDictionary(dictionary);
    // showAll(dictionary);

    //visualize the graph
    // visualize(dictionary, "graph.txt");

    //search positions of a letter in a word
    // int* arr = searchLetter(dictionary, "isissssiisiiisis", 's');
    // if(arr != NULL) {
    //     for(int i=0;i<strlen("isissssiisiiisis") && (arr[i] != -1);i++) {
    //         printf("%d ", arr[i]);
    //     }
    // }
    // else printf("sorry but the word doesn't exist in the dictionary!!");
    // if(arr) free(arr);

    //remove a word from the dictionary
    // dictionary = removeWord(dictionary, "ces");
    dictionary = removeWord(dictionary, "di");
    // dictionary = removeWord(dictionary, "de");
    dictionary = removeWord(dictionary, "c");
    dictionary = removeWord(dictionary, "diii");
    // dictionary = removeWord(dictionary, "ci");
    // dictionary = removeWord(dictionary, "ce");
    // dictionary = removeWord(dictionary, "des");
    // dictionary = removeWord(dictionary, "ci");
    dictionary = removeWord(dictionary, "cii");
    // visualize(dictionary, "graph.txt");
    return 0;
}