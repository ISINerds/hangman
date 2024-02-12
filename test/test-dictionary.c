#include "test-dictionary.h"

int main(){

    Dictionary* dic = NULL;

// 1. TESTING ADD WORD
displaySeperator("addWord(dictionary,word)");
    // a. TEST CASE 1 : ADDING A RANDO WORD
    dic = testAddWord(dic,"dog"); // TEST SHOULD PASS 
    // b. TEST CASE 2 : ADDING ANOTHER RANDOM WORD
    dic = testAddWord(dic,"incomprehensible"); // TEST SHOULD PASS
    // c. TEST CASE 3 : ADDING AN EXISTANT WORD
    dic = testAddWord(dic,"dog"); // TEST SHOULD PASS

// 2. TESTING ADD ALL
displaySeperator("addAll(dictionary,path)");
    dic = testAddAll(dic); // TEST SHOULD PASS

// 3. TESTING EXISTS
displaySeperator("exists(dictionary,word)");
    // a. TEST CASE 1 : TEST WITH AN EXISTANT WORD
    dic = testExists(dic,"adventure"); // TEST SHOULD PASS
    // b. TEST CASE 2 : TEST WITH AN UNEXISTANT WORD
    dic = testExists(dic,"blabla"); // TEST SHOULD FAIL
    // C. TEST CASE 3 : TEST WITH AN UNEXISTANT WORD THAT LOOKS SIMILLAR 
    dic = testExists(dic,"buterfly"); // TEST SHOULD FAIL

// 4. TESTING SEARCH LETTER
displaySeperator("searchLetter(dictionary,word,letter)");
    // a. TEST CASE 1 : TEST WITH A LETTER THAT OCCURS ONCE OR MORE IN A GIVEN WORD
    int array1[] = {3,6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    testSearchLetter(dic,"accommodation",'o',array1);// TEST SHOULD PASS
     // b. TEST CASE 2 : TEST WITH A WORD THAT DOES NOT EXIST IN THE DICTIONARY
    int array2[] = {1,-1,-1};
    testSearchLetter(dic,"job",'o',array1); // TEST SHOULD FAIL
    // c. TEST CASE 3 : TEST WITH A WRONG EXPECTED REUSULT
    int array3[] = {2,4,-1,-1,-1};
    testSearchLetter(dic,"happy",'p',array3);// TEST SHOULD FAIL
    // d. TEST CASE 4 : TEST WITH A LETTER THAT DOES NOT OCCUR IN A GIVEN WORD
    int array4[] = {1,-1,-1};
    testSearchLetter(dic,"cat",'e',array4);// TEST SHOULD PASS


// // 5. TESTING REMOVE
// displaySeperator("remove(dictionary,word)");
//     // a. TEST CASE 1 : TEST WITH AN EXISTANT WORD
//     dic = testRemove(dic,"dog"); // TEST SHOULD PASS
//     // b. TEST CASE 2 : TEST WITH AN UNEXISTANT WORD
//     dic = testExists(dic,"blabla"); // TEST SHOULD FAIL
//     // C. TEST CASE 3 : TEST WITH AN UNEXISTANT WORD THAT LOOKS SIMILLAR 
//     dic = testExists(dic,"buterfly"); // TEST SHOULD FAIL

    return 0;
}