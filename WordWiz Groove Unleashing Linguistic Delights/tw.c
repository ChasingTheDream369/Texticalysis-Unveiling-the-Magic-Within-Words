// COMP2521 20T2 Assignment 1,
//BY SHUBHAM JOHAR-z5284381.
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
#define RE_INITIALIZE 0
#define COMPARE_START 12
#define COMPARE_END 10

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int main( int argc, char *argv[]) {

   // File pointer for stopwords dictionary.
   FILE  *read_stopwords;
   
   // File pointer for the dictionary of 
   //words read from the book.         
   FILE  *add_dictionary;
   
   //Dictionary ADT representation of the words
   // read from stopwords.txt.         
   Dict  stopwords;
   
   //Dictioanry ADT representation of the words 
   //read from the textbook for analysis.            
   Dict  word_read;               
   
   // name of file containing textbook for analysis
   char *fileName;
   
   // number of top frequency words to show from the textbook.                
   uint64_t   nWords;                  
   
   // current input line/word read from the 
   //stopwords file.
   char   *add_stopword; 
   
   // most recent word read from the textbook to 
   //be stored/added in the dictionary ADT.                 
   char   dict_word[MAXWORD];
   
   //current line read from the textbook     
   char   *line_dict; 
   
   /*calcultes the number of unique words/number of nodes 
   in the Dictionary ADT rep. of textbook used for analysis.*/            
   int num_of_words = 0; 
  
   //Pointer to the recently added (word,frequency) pair.         
   WFreq* added_word;
    
   // process command-line args
   switch (argc) {
      
      case 2:
         
         //print only the top 10 (word,freq) pair if no input 
         //for nWords.
         nWords = 10;
         
         //sets the filename to the second argument on command line.
         fileName = argv[1];
         
         //break condition for processing the first case only.
         break;

      case 3:
         
         //when two args areadded sets nWords as the first argeument
         nWords = atoi(argv[1]);
         
         //default nWords
         if (nWords < 10) nWords = 10;
         
         //sets fileName to the third argument on cmd.
         fileName = argv[2];
         
         break;
      
      //Usage case if no args added.
      default:
         fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
         exit(EXIT_FAILURE);

   }

   // Creates a initially empty stopwords dictionary.
   stopwords = newDict();

   //sets maximum size of stopword to be read, defined for fgets().
   int size_stopword = MAXWORD;
   
   //mallocs the enough bytes for the add_stopword to 
   //read words from the stopwords file.
   add_stopword = (char *) malloc (size_stopword);

   //opens the stopwords file in reading mode to read 
   //stopwords in read_stopwords pointer.
   read_stopwords = fopen(STOPWORDS , "r");
   
   //if stopwords file is empty then prints the error message and exits.
   if (read_stopwords == NULL) {
      
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);

   }
   
   //reads the words from stopwords file and copies it into add_stopword.
   while (fgets(add_stopword, size_stopword, read_stopwords)) {
      
      //removing the trailing '\n' read by fgets.
      add_stopword[strlen(add_stopword) - 1] = '\0';
      
      //inserts the readed word into stopwords Dictinary ADT. 
		DictInsert(stopwords, add_stopword);
   
   }
   
   //close the read_stopwords file-pointer to stop reading
   //the file.
   fclose(read_stopwords);
   
   // Creates a initially empty  word_read dictionary ADT.
   word_read = newDict();
   
   //opens the desired file in reading mode to read words 
   //in add_dictionary pointer.
   add_dictionary = fopen(fileName , "r");
   
   //if desired file is empty then print the error message and exit.
   if (add_dictionary == NULL) {
      fprintf(stderr, "Can't open %s\n", fileName);
      exit(EXIT_FAILURE);
   }
   
   //maximum size of line to be read from the file, defined for fgets().
   int size_dict_add = MAXLINE;

   //mallocs the enough bytes for the line_dict to
   //read lines from the desired file.
   line_dict = (char *) malloc (size_dict_add);

   //reads the line from the line until it finds STARTING  
   while (fgets(line_dict, size_dict_add, add_dictionary) != NULL) {
      
      //removes the trailing '\n' from line_dict.
      line_dict[strlen(line_dict) - 1] = '\0'; 
      
      //As soon *** START OF is copied to line_dict break the loop 
      //and start reading from the next line.Here strncmp compares
      //character b/w first two arguments only till a pre-defined n 
      //value as third arg. For more info on strncmp refer- (man strncmp) -
      //https://man7.org/linux/man-pages/man3/strncmp.3p.html
       
      if (strncmp(line_dict, STARTING, COMPARE_START) == 0) {
         break;
      }

   }
   
   //if *** START OF is not encountered in the file,its not a gutenberg file.
   
   if (strncmp(line_dict, STARTING, COMPARE_START) != 0) {
      
      fprintf(stderr, "Not a Project Gutenberg book\n");
      exit(EXIT_FAILURE);
   
   }
    
   // scan the File till end and add words to dictionary by accumualting counts.
   while (fgets(line_dict, size_dict_add, add_dictionary) != NULL) {
      
      //remove the trailing '\n' from the line_dict.
      line_dict[strlen(line_dict)-1] = '\0';

      //As soon *** END OF is copied to line_dict break the loop and
      //stop adding to the dictionary ADT.
      if (strncmp(line_dict, ENDING, COMPARE_END) == 0) break;

      //sets the char_in_line to 0, to start reading the line_dict array
      //from first character copied to it from the file. 
      int char_in_line = 0;

      //sets char_copied to 0, used to copy only isWordChar type elements
      //in the separate string.
      int char_copied = 0;
      
      //reading the line_dict till '\0',find unique words to be added in Dict.
      while (line_dict[char_in_line] != '\0') {
         
         /*When a character which is 'isWordChar' is encountered add to dict_word,
         this strategy helps in tokenising and removing every character 
         other than isWordChar type.*/
         
         if (isWordChar(line_dict[char_in_line])) {
            
            //when the character is an alphabet then normalise 
            //it by converting to lower case.
            if (isalpha(line_dict[char_in_line])) {
               dict_word[char_copied] = tolower(line_dict[char_in_line]);
            }
            
            //every other character of isWordChar type is directly
            //copied to dict_word.
            else {
               dict_word[char_copied] = line_dict[char_in_line];
            }
            
            //increment the dict_word copying pointer 
            char_copied++;
         }

         /*When a character which is not the first character on line and 
         is not isWordChar is found,and also string length of dict_word is not 0,
         then use the copied dict_word string for analysis.*/   
         
         if (strlen(dict_word) >= 1 && !isWordChar(line_dict[char_in_line])
         && char_in_line >= 1) {
            
            //null terminating the copied characters  to make it a string.
            dict_word[char_copied] = '\0';

            //finding the length of dict_word for stemming.    
            int length_word = strlen(dict_word);
            
            /*searching the dict_word in the stopwords dictionary ADT and 
            if not in stopword dictionary then stem and add it to word_read dict.*/

            if (DictFind(stopwords, dict_word) == NULL) {
               
               /*find the stemmed length, and then set the new_length+1
               element of dict_word to '\0'.*/

               int new_length = stem(dict_word, 0, length_word-1);
               dict_word[new_length + 1] = '\0';
               
               /*When the length of dict_word is more than 1 after stemming,
               then insert the word into word_read dictionary ADT.*/

               if (strlen(dict_word) > 1) {
                        
                  added_word = DictInsert(word_read, dict_word);
                  
                  //increment the frequency of the (word,freq) pair 
                  //every time added.
                  added_word->freq = added_word->freq + 1;
                  
                  /*count the num_of_words/nodes in word_read Dict ADT
                  every time an unique element is added.*/
                  
                  if (added_word->freq == 1) {
                     num_of_words++;
                  }

               }
               
            }
            
            /*After inserting the word in DICT ADT, reset the entire 
            dict_word to '/0',to prevent the overlapping of next_word 
            on line into the existing dict_word. as length of words might vary.*/
            
            for (int reset_word = 0; reset_word < MAXWORD; reset_word++) {
               dict_word[reset_word] = 0;
            }
            
            //re-initialize the copying pointer/var for copying the next 
            //word on line from beginning into dict_word. 
            char_copied = RE_INITIALIZE;
                
         }
         
         //increment char_in_line to find the next char on line.
         char_in_line++;

      }
   
   }

   // compute and display the top N words by storing the result
   //in (word,freq) pair results array.
   WFreq *results;
   
   //when the argument of nWords is more than unique words in dict, 
   //then calloc the memory only for num_of_words/num of nodes in Dict ADT.
   
   if (nWords > num_of_words) {
      
      //calloc set's the memory for 'num_of_words' bytes in this 
      //case and also initialises (freq, word) pair to (0, NULL)
      //in the entire results array. For more reference on calloc- refer 
      //https://man7.org/linux/man-pages/man3/calloc.3p.html (man calloc).

      results = calloc(num_of_words, sizeof(*results));
      
      /*find top frequencies for the entire set of nodes/
      total num_of_words in this condition.*/

      findTopN(word_read, results, num_of_words);

   }
   
   //calloc's the memory for nWords otherwise.
   else {

      results = calloc(nWords, sizeof(*results));
      //find top nWords frequencies.
      findTopN(word_read, results, nWords);

   }
   
   //close the add_dictionary pointer.
   fclose(add_dictionary);

   // done
   return EXIT_SUCCESS;
}