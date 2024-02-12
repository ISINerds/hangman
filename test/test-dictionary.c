#include "test-dictionary.h"

int main(){

    Dictionary* dic = NULL;

// 1. TESTING ADD WORD
displaySeperator("addWord(dictionary,word)");
    // a. TEST CASE 1 : ADDING A RANDO WORD
    dic = testAddWord(dic,"sad"); // TEST SHOULD PASS 
    // b. TEST CASE 2 : ADDING ANOTHER RANDOM WORD
    dic = testAddWord(dic,"Supercalifragilisticexpialidocious"); // TEST SHOULD PASS
    // c. TEST CASE 3 : ADDING AN EXISTANT WORD
    dic = testAddWord(dic,"sad"); // TEST SHOULD PASS

// 2. TESTING ADD ALL
displaySeperator("addAll(dictionary,path)");
    dic = testAddAll(dic); // TEST SHOULD PASS

// 3. TESTING EXISTS
displaySeperator("exists(dictionary,word)");
    // a. TEST CASE 1 : TEST WITH AN EXISTANT WORD
    dic = testExists(dic,"resplendent"); // TEST SHOULD PASS
    // b. TEST CASE 2 : TEST WITH AN UNEXISTANT WORD
    dic = testExists(dic,"blabla"); // TEST SHOULD FAIL
    // C. TEST CASE 3 : TEST WITH AN UNEXISTANT WORD THAT LOOKS SIMILLAR 
    dic = testExists(dic,"iluminate"); // TEST SHOULD FAIL

// 5. TESTING SEARCH LETTER
displaySeperator("searchLetter(dictionary,word,letter)");
    // a. TEST CASE 1 : TEST WITH A LETTER THAT OCCURS ONCE IN A GIVEN WORD
    int array[] = {2,-1,-1,-1,-1,-1,-1};
    testSearchLetter(dic,"harmony",'m',array);

    return 0;
}