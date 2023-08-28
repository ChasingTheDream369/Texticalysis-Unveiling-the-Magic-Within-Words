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
D
   //printf ("Please enter a string: ");

   /* These 2 lines are very important. */
   stopwords = newDict();
   uint64_t size = MAXLINE;
   line = (char *) malloc (size);
   in = fopen(STOPWORDS , "r");
   while (getline(&line, &size, in) != -1) {
      DictInsert(stopwords, line);
      //printf ("%s", returned->word);
      
   }


   string = (char *) malloc (size);
   FILE* in;
   char* compare_start = NULL;
   char* dict_word = NULL ;
   char * begin = NULL;
   //char *end = NULL;

   in = fopen("data/2600.txt" , "r");
   while (getline(&string, &size, in) != -1) {
         begin = strndup(string, 12);
         if (strcmp(begin, STARTING) == 0) {
            break;
         }
         free(begin);
   }
   getline(&string, &size, in);
   int j = 0;
   while(fgets(string, MAXLINE, in) != NULL)
	{
		if(strcmp(string,"\n"  ) != 0 && strcmp(string,"\r\n") != 0 && strcmp(string,"\0"  ) != 0 && 1)
		{
			if(strncmp(string, ENDING, strlen(ENDING)) == 0)	break;
			
			for(int i = 0; i < strlen(string)-1; i++)
			{
				if(string[i] != ' ' && isWordChar(string[i]))
				{
					if(string[i] >= 65 && string[i] <= 90)	dict_word[j] = string[i] + 32;
					else	dict_word[j] = string[i];
					j++;
				}
				else if( ( string[i] == ' ' || !isWordChar(string[i]) ) && j > 1)
				{
					if(DictFind(stopwordDict, word) == NULL) 
					{
						k = stem(word, 0, (strlen(word)-1));
						word[k+1] = '\0';
						DictInsert(gutenburg, word);
					}			
					for(j = 0; j < MAXWORD; j++)	word[j] = '\0';
					j = 0;
				}
				else	j = 0;
			}
		}	
		else continue;
	}