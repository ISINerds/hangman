#include "./includes/utils/words-handler.h"
int main(){

   Level l = HARD;

   Level * levelWords = (Level*)malloc(5 * sizeof(Level)); 
   levelWords[0] = HARD;
   printf("%d",levelWords[0]);


   return 0;
}