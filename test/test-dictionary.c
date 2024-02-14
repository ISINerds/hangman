#include "test-dictionary.h"

int main(){

    Dictionary* dic = NULL;

// 1. TESTING ADD WORD
displaySeperator("addWord(dictionary,word)");
    // a. TEST CASE 1 : ADDING A RANDO WORD
    dic = testAddWord(dic,"cup"); // TEST SHOULD PASS 
    // b. TEST CASE 2 : ADDING ANOTHER RANDOM WORD
    dic = testAddWord(dic,"headphones"); // TEST SHOULD PASS
    // c. TEST CASE 3 : ADDING AN EXISTANT WORD
    dic = testAddWord(dic,"cup"); // TEST SHOULD PASS

// 2. TESTING ADD ALL
displaySeperator("addAll(dictionary,path)");
    dic = testAddAll(dic); // TEST SHOULD PASS

// 3. TESTING EXISTS
displaySeperator("exists(dictionary,word)");
    // a. TEST CASE 1 : TEST WITH AN EXISTANT WORD
    dic = testExists(dic,"cheetah"); // TEST SHOULD PASS
    // b. TEST CASE 2 : TEST WITH AN UNEXISTANT WORD
    //dic = testExists(dic,"blabla"); // TEST SHOULD FAIL
    // C. TEST CASE 3 : TEST WITH AN UNEXISTANT WORD THAT LOOKS SIMILLAR 
    //dic = testExists(dic,"buterfly"); // TEST SHOULD FAIL

// 4. TESTING SEARCH LETTER
displaySeperator("searchLetter(dictionary,word,letter)");
    // a. TEST CASE 1 : TEST WITH A LETTER THAT OCCURS ONCE OR MORE IN A GIVEN WORD
    int array1[] = {2,3,5,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    testSearchLetter(dic,"hippopotamus",'p',array1);// TEST SHOULD PASS
     // b. TEST CASE 2 : TEST WITH A WORD THAT DOES NOT EXIST IN THE DICTIONARY
    int array2[] = {1,-1,-1};
    //testSearchLetter(dic,"job",'o',array1); // TEST SHOULD FAIL
    // c. TEST CASE 3 : TEST WITH A WRONG EXPECTED REUSULT
    int array3[] = {2,4,-1,-1,-1,-1};
    //testSearchLetter(dic,"rabbit",'b',array3);// TEST SHOULD FAIL
    // d. TEST CASE 4 : TEST WITH A LETTER THAT DOES NOT OCCUR IN A GIVEN WORD
    int array4[] = {1,-1,-1};
    //testSearchLetter(dic,"cat",'e',array4);// TEST SHOULD FAIL
// 5. TESTING REMOVE WORD
displaySeperator("removeWord(dictionary,word)");
    // a. TEST CASE 1 : REMOVING A RANDOM WORD
    //dic = testRemove(dic,"cup"); // TEST SHOULD PASS 
    // b. TEST CASE 2 : REMOVING ANOTHER RANDOM WORD
    dic = testRemove(dic,"headphones"); // TEST SHOULD PASS
    // c. TEST CASE 3 : REMOVING  AN INEXISTANT WORD
    //dic = testRemove(dic,"cup"); // TEST SHOULD PASS


    // RESULT 
    printf("\n RESULT: \n %d TESTS PASSED OUT OF %d \n %d TESTS FAILED OUT OF %d\n \n",testPassed,testPassed+testFailed,testFailed,testPassed+testFailed);
    
    char choice;

    printf("Do you want to read all the words from the file? (Y/N): ");
    scanf(" %c", &choice);
    while(1){
        if (choice == 'Y' || choice == 'y') {
            showAllWords(dic);
            break;
        } else if (choice == 'N' || choice == 'n') {
            printf("\n");
            break;
        } else {
            printf("Invalid choice. Please enter Y or N.\n");
            printf("Do you want to read all the words from the file? (Y/N): ");
            scanf(" %c", &choice);
        }
    }

    printf("Do you want to read display the words tree? (Y/N): ");
    scanf(" %c", &choice);
    while(1){
        if (choice == 'Y' || choice == 'y') {
            visualize(dic);
                system("wslview  output.png || xdg-open output.png");
            break;
        } else if (choice == 'N' || choice == 'n') {
            break;
        } else {
            printf("Invalid choice. Please enter Y or N.\n");
            printf("Do you want to read display the words tree? (Y/N): ");
            scanf(" %c", &choice);
        }
    }
    return 0;
}