// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int main( int argc, char *argv[])
{
   // TODO ... add any variables you need

    FILE  *in;         // currently open file
    FILE  *dictionary;
    Dict  stopwords;  // dictionary of stopwords
    Dict  word_read;     // dictionary of words from book
    WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

    char *fileName;    // name of file containing book text
    int   nWords;      // number of top frequency words to show

    char   *line;  // current input line
    char   *dict_word = NULL;  // current word
    char   *line_dict;
    int max_frequency = 0;
    WFreq* most_frequent; 
    int num_of_words = 0;
    WFreq* added_word;
    
    // process command-line args
    switch (argc) {
    case 2:
        nWords = 10;
        fileName = argv[1];

        break;
    case 3:
        nWords = atoi(argv[1]);
        if (nWords < 10) nWords = 10;
        fileName = argv[2];
        break;
    default:
        fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // build stopword dictionary
    stopwords = newDict();
    uint64_t size = MAXLINE;
    
    line = (char *) malloc (size);
    in = fopen(STOPWORDS , "r");

    if (in == NULL) {
        fprintf(stderr, "Can't open stopwords\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, size, in)) {
        line[strlen(line) - 1] = '\0';
		DictInsert(stopwords, line);
        //printf ("%s", returned->word);
    }
    
    fclose(in);
    //showDict(stopwords);
    // scan File, up to start of text
    word_read = newDict();
    dictionary = fopen(fileName , "r");

    if (dictionary == NULL) {
        fprintf(stderr, "Can't open %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    line_dict = (char *) malloc (size);
    
    while (fgets(line_dict, size, dictionary)) {
        
        if (strncmp(line_dict, STARTING, sizeof(STARTING)) == 0) {
            break;
        }
    }

    if (line_dict == NULL) {
        fprintf(stderr, "Not a Project Gutenberg book\n");
        exit(EXIT_FAILURE);
    }

    
    // TODO
    
    // scan File reading words and accumualting counts
    
    printf(" ");
    
    while (fgets(line_dict, size, dictionary)) {
        
        line_dict[strlen(line_dict) - 1] = '\0';
        if (strncmp(line_dict, ENDING, sizeof(ENDING)) == 0) break;
        
        int i = 0;
        int j = 0;

        while (line_dict[i] != '\0') {
            
            if (isWordChar(line_dict[i])) {
                dict_word[j] = tolower(line_dict[i]);
                j++;
            }
            
            if (line_dict[i] = ' ') {
                
                int length_word = strlen(dict_word);
            
                if (DictFind(stopwords, dict_word) == NULL) {
               
                    int new_length = stem(dict_word, 0, length_word-1);
                    dict_word[new_length+1] = '\0';
                    
                    if (strlen(dict_word) > 1) {
                        
                        added_word = DictInsert(word_read, dict_word);
                        added_word->freq = added_word->freq + 1;

                    } 
                    
                    if (added_word->freq >= max_frequency) {
                        
                        max_frequency = added_word->freq;
                        most_frequent = added_word;
                        
                    }
                    
                    if (added_word->freq == 1) {
                        num_of_words++;
                    }

                }
                
            }

            i++;

        }
         
    
    }
    //showDict(word_read);
    /*results = malloc(sizeof(*results)*nWords);
    results[0] = *most_frequent;
    findTopN(word_read, results, nWords);*/
    
   // compute and display the top N words
   if (nWords > num_of_words) {
      results = calloc(num_of_words, sizeof(*results));
      results[0] = *most_frequent;
      findTopN(word_read, results, num_of_words);
   }
   else {
      results = calloc(nWords, sizeof(*results));
      results[0] = *most_frequent;
      findTopN(word_read, results, nWords);
   }
   // done
   return EXIT_SUCCESS;
}