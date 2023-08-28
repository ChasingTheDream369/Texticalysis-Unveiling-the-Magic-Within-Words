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
    dict_word = (char *) malloc (MAXWORD);
    //char * begin = NULL;
    //char *stem_word = NULL;
    //char* real_word = NULL;

    in = fopen("data/0011.txt" , "r");
   /* while (getline(&string, &size, in) != -1) {
        begin = strndup(string, 12);
        if (strcmp(begin, STARTING) == 0) {
            break;
        }
    }*/
    while (getline(&string, &size, in) != -1) {
        while (*string != '\0')
        {
            if(!isspace(*string) && isWordChar(*string))
            {
            *dict_word = *string;
            dict_word++;
            }
            string++;
        }
        *dict_word = '\0';
        printf ("%s\n", dict_word);

    }
    
    fclose(in);
    return 0;
}