#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<stdint.h>
#include <ctype.h>

#define STOPWORDS "stopwords"
#define STARTING "*** START OF"
#define ENDING   "*** END OF"
#define MAXLINE 1000
#define MAXWORD 100
#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')
 int main()
 {
     uint64_t size = MAXLINE;

char *string;

//printf ("Please enter a string: ");

/* These 2 lines are very important. */

string = (char *) malloc (size);
FILE* in;
char* dict_word = NULL;
char * begin = NULL;
char **real_word = {NULL};

in = fopen("data/0011.txt" , "r");
while (getline(&string, &size, in) != -1) {
      begin = strndup(string, 12);
      if (strcmp(begin, STARTING) == 0) {
         break;
      }
   }
   int i = 0;
   int j = 0;
   int k = 0;
    while (fgets(dict_word,MAXLINE, in)) {
        /** if you want to split value and string*/
        k = 0;
        while (dict_word[i] != '\0') {
            j = 0;
            while (dict_word[i] != ' '){
                if (isWordChar(dict_word[i])) {
                    real_word[k][j] = dict_word[i];
                    if (islower(real_word[k][j])) {
                        real_word[k][j] = toupper(real_word[k][j]);
                        j++;
                    }
                }
                i++;  
    
            }
            
            printf ("%s\n", real_word[k]);
            k++;
            i++;
        }
        break;
    }
    fclose(in);
    return 0;
}