#include "./includes/utils/words-handler.h"
int main(){

   Level l = EASY;

   Level * levelWords = (Level*)malloc(5 * sizeof(Level)); 
   levelWords[0] = EASY;
   printf("%d",levelWords[0]);

   Words words = parser("./src/words.txt");
   printf("%s",randomWord(words,l));


   return 0;
}