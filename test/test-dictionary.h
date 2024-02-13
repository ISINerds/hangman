#pragma once
#include "../src/includes/data-structures/dictionary.h"
#include <time.h>

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

#define DATE_TIME_FORMAT "[%04d-%02d-%02d %02d:%02d:%02d]"
#define WORDS_PATH "test/test-words.txt"
#define GRAPH_PATH "test/test-graph.dot"

int testPassed = 0;
int testFailed =0;

struct tm* displayCurrentTime(int test){
    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    if(test){
        printf(COLOR_GREEN "[%04d-%02d-%02d %02d:%02d:%02d]",
        localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
        localTime->tm_hour, localTime->tm_min, localTime->tm_sec); 
    }
    else{
        printf(COLOR_RED "[%04d-%02d-%02d %02d:%02d:%02d]",
        localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
        localTime->tm_hour, localTime->tm_min, localTime->tm_sec); 
    }
}
void displaySeperator(char* functionToTest){
    printf("\n \t -- Testing  %s :\n\n",functionToTest);
}
Dictionary* testAddWord(Dictionary* dic, char* word){
    const char* testCase = "(TESTING ADD WORD)";
    dic =  addWord(dic,word);
    int testIfWordAdded = exists(dic,word);
    if(testIfWordAdded){
        displayCurrentTime(1);
        printf(" %s The Word '%s' Was Added Successfully!\n"COLOR_RESET,testCase,word);
        testPassed++;
    }
    else{
        displayCurrentTime(0);
        printf(" %s Could Not Add The word %s To The Dictionary!\n"COLOR_RESET,testCase,word);
        testFailed++;
    }
    return dic;
}

Dictionary* testAddAll(Dictionary* dic){
    const char* testCase = "(TESTING ADD ALL)";
    Words words = parser(WORDS_PATH);
    dic = AddAll(dic,WORDS_PATH);
    for(int i=0;i<words.wordsArraySize;i++){
        int testIfWordAdded = exists(dic,words.wordsArray[i]);
        if(testIfWordAdded){
            displayCurrentTime(1);
            testPassed++;
            printf(" %s The Word '%s' Was Added Successfully!\n"COLOR_RESET,testCase,words.wordsArray[i]);
        }
        else{
            displayCurrentTime(0);
            testFailed++;
            printf(" %s Could Not Add The word %s To The Dictionary!\n"COLOR_RESET,testCase,words.wordsArray[i]);
        }
    }
    return dic;
}

Dictionary* testExists(Dictionary* dic, char* word){
    const char* testCase = "(TESTING EXISTS)";
    if(exists(dic,word)){
        displayCurrentTime(1);
        testPassed++;
        printf(" %s The Word '%s' Exists In The Dictionary!\n"COLOR_RESET,testCase,word);
    }
    else{
        displayCurrentTime(0);
        testFailed++;
        printf(" %s The Word '%s' Does Not Exists In The Dictionary!\n"COLOR_RESET,testCase,word);
    }
    
    return dic;
}


void testSearchLetter(Dictionary* dic,char * word,char c,int expectedResult[]){
    const char* testCase = "(TESTING SEARCH LETTER)";
    int * positons = searchLetter(dic,word,c);
    if(positons == NULL){
        testFailed++;
        displayCurrentTime(0);
        printf(" %s The word '%s' Does Not Exist In The Dictionary!\n"COLOR_RESET,testCase,word);
        return;
    }
    for(int i=0;i<strlen(word);i++){
        if((expectedResult[i]==positons[i]) &&(positons[i]!=-1)){
            testPassed++;
            displayCurrentTime(1);
            printf(" %s The Letter '%c' Exists In The Word '%s' At The Postion %d!\n"COLOR_RESET,testCase,c,word,positons[i]);
        }
        else if(expectedResult[i]!=positons[i]){
            displayCurrentTime(0);
            testFailed++;
            if(positons[i]==-1){
                printf(" %s The Letter '%c' Does Not Exist In The Word '%s' Not At The Postion %d!\n"COLOR_RESET,testCase,c,word,expectedResult[i]);
            }
            else{
                printf(" %s The Letter '%c' Exists In The Word '%s' At The Postion %d Not At The Postion %d!\n"COLOR_RESET,testCase,c,word,positons[i],expectedResult[i]);
            }
        }
}
}
Dictionary* testRemove(Dictionary* dic,char *word){
    const char* testCase = "(TESTING REMOVE WORD)";
    dic =  removeWord(dic,word);
    int test = exists(dic,word);
    if(!test){
        displayCurrentTime(1);
        testPassed++;
        printf(" %s The Word '%s' Was Removed Successfully From The Dictionary!\n"COLOR_RESET,testCase,word);
    }
    else{
        displayCurrentTime(0);
        testFailed++;
        printf(" %s Could Not Remove The word %s To The Dictionary!\n"COLOR_RESET,testCase,word);
    }
    return dic;
}