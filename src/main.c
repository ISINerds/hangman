#include "./includes/utils/words-handler.h"
int main(){

   Level l = HARD;

   // Level * levelWords = (Level*)malloc(5 * sizeof(Level)); 
   // levelWords[0] = EASY;
   // printf("%d",levelWords[0]);

   Words words = parser("./src/words.txt");
   printf("-----  result :%s\n",randomWord(words,l));


   return 0;
}