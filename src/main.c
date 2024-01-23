#include <stdio.h>
#include "./includes/data-structures/dictionary.h"
int main(){

    Dictionary* dictionary = NULL;
    dictionary = addWord(dictionary, "ce");
    dictionary = addWord(dictionary, "ci");
    dictionary = addWord(dictionary, "ces");
    dictionary = addWord(dictionary, "di");
    dictionary = addWord(dictionary, "de");
    dictionary = addWord(dictionary, "des");
dictionary = addWord(dictionary, "ce");
    dictionary = addWord(dictionary, "ci");
    dictionary = addWord(dictionary, "ces");
    dictionary = addWord(dictionary, "di");
    dictionary = addWord(dictionary, "de");
    dictionary = addWord(dictionary, "des");
    dictionary = addWord(dictionary, "siisiiisis");
    dictionary = addWord(dictionary, "s");
    dictionary = addWord(dictionary, "si");
    dictionary = addWord(dictionary, "sia");
    dictionary = addWord(dictionary, "siabc");
    dictionary = addWord(dictionary, "siabcd");
    dictionary = addWord(dictionary, "siabcef");
    dictionary = addWord(dictionary, "siabfsd");
    dictionary = addWord(dictionary, "siabfsr");
    dictionary = addWord(dictionary, "siabfsrt");
    visualize(dictionary, "graph.dot");
    return 0;
}