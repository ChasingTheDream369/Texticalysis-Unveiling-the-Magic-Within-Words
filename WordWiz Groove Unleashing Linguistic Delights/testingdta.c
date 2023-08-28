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

typedef struct _DictNode *Link;
typedef char* new_word;

typedef struct  _DictNode {
   WFreq  data;
   Link   left;
   Link   right;
} DictNode;

struct _DictRep {
   Link tree;
};

//FUNCTION PROTOTYPES-
static Link doDictInsert (Link d, new_word w);
static int height (Link d);
static Link rotateLeft(Link n);
static Link rotateRight(Link n);
static Link AddNode(new_word w);
WFreq *DoDictFind (Link d, new_word w);
int dofindTopN (Link d, WFreq *wfs, int n, int i, int j);

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
   while (getline(&line, &size, in) != -1) {
      DictInsert(stopwords, line);
   }
   
   // scan File, up to start of text
   word_read = newDict();
   dictionary = fopen(fileName , "r");
   line_dict = (char *) malloc (size);
   char *compare_start;
   while (getline(&line_dict, &size, dictionary) != -1) {
      compare_start = strndup(line_dict, 12);
      if (strcmp(compare_start, STARTING) == 0) {
         break;
      }
   }
   getline(&line_dict, &size, dictionary);
   
   char* compare_end;
   
   while (getline(&line_dict, &size, dictionary) != -1) {
      
      compare_end = strndup(line_dict, 10);
      if (strcmp(compare_end, ENDING) == 0) {
         break;
      }
      dict_word = strtok(line_dict, " ,;?:.\"*!_{}[]()`~><=_|@&#$^\\\t\n");
      while (dict_word != NULL) {
         int i = 0;
         while (dict_word[i] != '\0') {

            if (isupper(dict_word[i])) {
               dict_word[i] = tolower(dict_word[i]);
            }
            i++;
         }
         WFreq* added_word;
         int length_word = strlen(dict_word);
         if (DictFind(stopwords, dict_word) == NULL) {
            int new_length = stem(dict_word, 0, length_word);
            char *stem_word = strndup(dict_word, new_length);
            added_word = DictInsert(word_read, stem_word);
            added_word->freq = added_word->freq + 1;
         }

         dict_word = strtok(NULL, " ,;?:.\"*!_{}[]()`~><=_|@&#$^\\\t\n");
      }
   
   }

   // compute and display the top N words
   results = malloc (sizeof(*results)*nWords);
   findTopN(word_read, results, nWords);
   // done
   return EXIT_SUCCESS;
}

Dict newDict()
{
   Dict t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    t->tree = NULL;
    return t;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, new_word w)
{
   // TODO
   Link inserted = doDictInsert(d->tree, w);
   return &(inserted->data);
}

static Link doDictInsert (Link d, new_word w) {

   if (d == NULL) {
      return AddNode(w);
   }

    // insert recursively
   int lexical_insert = strcmp(w, d->data.word);
   if (lexical_insert < 0) {
      d->left = doDictInsert(d->left, w);
   } 
   else if (lexical_insert > 0) {
      d->right = doDictInsert(d->right, w);
   } 
   else { // (cmp == 0)
        // if time is already in the tree,
        // we can return straight away
        return d;
   }

   // insertion done
   // correct the height of the current subtree
    
   // rebalance the tree
   // TODO: Add your code here and change
   //       the return statement if needed
   int balance_factor = height(d->left) - height(d->right);
   if (balance_factor > 1) {
      if (strcmp(w, d->left->data.word) > 0) {
         d->left = rotateLeft(d->left);
      }
      d = rotateRight(d);
   }
  
   // Right Left Case 
   if (balance_factor < -1) 
   {   
      if (strcmp(w, d->right->data.word) < 0) {
         d->right = rotateRight(d->right);
      }
      d = rotateLeft(d); 
   } 
  
    /* return the (unchanged) node pointer */ 
   return d;

}

static int height (Link d) {
   
   if (d == NULL) {
      return -1;
   }

   else if (d->right == NULL && d->left == NULL) {
      return 0;
   }

   int left = height(d->left);
   int right = height(d->right);
   int greater_height = (left > right) ? left : right;

   return  1 + greater_height;
   
}

static Link rotateLeft(Link n) {
   // TODO: Add your code here and change
   //       the return statement if needed
   if (n == NULL) {
      return NULL;
   }
   //assert(n != NULL);
   else if (n->right == NULL) {
      return n;
   }
   
   //assert(n->right!= NULL);
    
   else { 
      Link rotate = n->right;
      Link rotate_new = rotate->left;
      rotate->left = n;
      n->right = rotate_new;
        
      return rotate; 
   }

   return n;
}

static Link rotateRight(Link n) {
    // TODO: Add your code here and change
    //       the return statement if needed

   if (n == NULL) {
      return NULL;
   }

   else if (n->left == NULL) {
      return n;
   }

   else {
      Link rotate = n->left;
      Link rotate_new = rotate->right;
      rotate->right = n;
      n->left = rotate_new; 

        
      return rotate;
   }
    
   return n;
    
}

static Link AddNode(new_word w) {
   
   Link n = malloc(sizeof(*n));
   if (n == NULL) {
      fprintf(stderr, "Insufficient memory!\n");
      exit(EXIT_FAILURE);
   }

   n->data.word = strdup(w);
   n->data.freq = 0;
   n->left = NULL;
   n->right = NULL;
   return n;

}
// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   // TODO
   WFreq *search_word = DoDictFind(d->tree, w); 
   return search_word;
}

WFreq *DoDictFind (Link d, new_word w) {

   if (d == NULL)
		return NULL;
   else if (strcmp(w,d->data.word) == 0)
		return &(d->data);
	else if (strcmp(w,d->data.word) < 0)
		return DoDictFind(d->left, w);
	else if (strcmp(w,d->data.word) > 0)
		return DoDictFind(d->right, w);
	
   return NULL;

}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   int s = dofindTopN(d->tree, wfs, n, 0, 0);
   int k = (s%n);
   for (int f = k; f >= 0; f--) {
      printf ("%7d %s\n", wfs[k].freq, wfs[k].word);
   }
   for (int e = k; e < n; e++) {
      printf ("%7d %s\n", wfs[k].freq, wfs[k].word);
   }
   return k;
}

int dofindTopN (Link d, WFreq *wfs, int n, int i, int j) {
   if (d == NULL) {
      return j;
   }
   dofindTopN(d->left, wfs, n, i, j);
   if (i == 0) {
      if (j != 0) {
         if (d->data.freq >= wfs[i].freq) {
            wfs[i] = d->data;
         }
      }
      else {
         wfs[i] = d->data;
      }
      i++;
   }
   else if(i < n) { 
      if (d->data.freq >= wfs[i-1].freq)  {
         wfs[i] = d->data;
         i++;
      }
   }
   else if (i >= 10) {
      i = 0;
      j++;
   } 
   dofindTopN(d->right, wfs, n, i, j);
   return j;

}

// print a dictionary
void showDict(Dict d)
{
   // TODO if you need to display Dict's for debugging
   return;
}

