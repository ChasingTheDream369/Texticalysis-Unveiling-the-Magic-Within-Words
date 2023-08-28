#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "stemmer.h"
#include "Dict.h"

#define STOPWORDS "stopwords"
#define STARTING "*** START OF"
#define ENDING   "*** END OF"
#define MAXLINE 1000
int main() {

//int bytes_read;

   uint64_t size = MAXLINE;

   char *string;

   //printf ("Please enter a string: ");

   /* These 2 lines are very important. */

   string = (char *) malloc (size);
   FILE* in;
   char* compare_start = NULL;
   //char* dict_word = NULL ;
   char * begin = NULL;
   char *end = NULL;

   in = fopen("shubham.txt" , "r");
   while (getline(&string, &size, in) != -1) {
         begin = strndup(string, 12);
         if (strcmp(begin, STARTING) == 0) {
            break;
         }
         
   }
   getline(&string, &size, in);
   while (getline(&string, &size, in) != -1) {
            end = strndup(string, 10);
            if (strcmp(end, ENDING) == 0) {
               break;
            }
            
            compare_start = strtok(string, " ,;?:.\"*!_{}[]()`~><=_|@&#$^\\\t\n");
            while (compare_start != NULL) { 
                  int i = 0;
                  while (compare_start[i] != '\0') {
                     if (isupper(compare_start[i])) {
                        compare_start[i] = tolower(compare_start[i]);
                     }
                     i++;
                  }
                  int length_word = strlen(compare_start);
                  int new_end = stem(compare_start, 0, length_word-1);
                  compare_start[new_end+1] = '\0';
                  printf("%s ", (compare_start));
                  compare_start = strtok(NULL, " ,;?:.\"*!_{}[]()`~><=_|@&#$^\\\t\n");
            }
            
      
   }

return 0;

}