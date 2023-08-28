// COMP2521 20T2 Assignment 1
// Dict.c ... implementsation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "Dict.h"
#include "WFreq.h"

typedef struct _DictNode *Link;
typedef char* new_word;

typedef struct  _DictNode {
   WFreq  data;
   Link   left;
   Link   right;
   int    height;
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
static void dofindTopN (Link d, WFreq *wfs, int n);
static int max(int a, int b);
void doshowDict(Link d);
WFreq* FindmaxFrequency (Link d, int max_frequency, WFreq* most_frequent);

// create new empty Dictionary
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
   d->tree = doDictInsert(d->tree, w);
   WFreq* recently_addedd = DictFind(d, w);
   return recently_addedd;
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
   d->height = 1 + max(height(d->left), height(d->right));
    
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

static int height(Link n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

static int max(int a, int b) {
    return a > b ? a : b;
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

      n->height = 1 + max(height(n->left), height(n->right));
      rotate->height = 1 + max(height(rotate->left), height(rotate->right));
        
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

      n->height = 1 + max(height(n->left), height(n->right));
      rotate->height = 1 + max(height(rotate->left), height(rotate->right));

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
   else if (strcmp(w,d->data.word) < 0)
		return DoDictFind(d->left, w);
	else if (strcmp(w,d->data.word) > 0)
		return DoDictFind(d->right, w);
   else {
		return &(d->data);
   }
	
   return &(d->data);

}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   WFreq* most_frequent = FindmaxFrequency(d->tree, d->tree->data.freq, &(d->tree->data));
   wfs[n-1] = *most_frequent;
   dofindTopN(d->tree, wfs, n);
   for (int i = n; i > 0 ; i--) {
      printf ("%7d %s\n", wfs[i].freq, wfs[i].word);
   }
   return n;
   
}

void dofindTopN (Link d, WFreq *wfs, int n) {
   
   if (d == NULL) {
      return;
   }
   
   dofindTopN(d->left, wfs, n);

   int k = n;
   while (d->data.freq <= wfs[k].freq && k >= 0) k--;
   for (int i = 0; i < k; i++) {
      wfs[i] = wfs[i+1];
   }
   wfs[k] = d->data;

   dofindTopN(d->right, wfs, n);

}

// print a dictionary
void showDict(Dict d) {

   doshowDict(d->tree);
   
}

void doshowDict(Link d) {
   if (d == NULL) {
      return;
   }

   doshowDict(d->left);
   printf("Element : %s, Freq : %d\n", d->data.word,d->data.freq);
   doshowDict(d->right);

}

WFreq* FindmaxFrequency (Link d, int max_frequency, WFreq*  most_frequent) {
   
   if (d == NULL) {
      return NULL;
   }
   
   FindmaxFrequency(d->left, max_frequency, most_frequent);
   FindmaxFrequency(d->right, max_frequency, most_frequent);
   
   if (d->data.freq >= max_frequency) {
      max_frequency = d->data.freq;
      most_frequent = &(d->data);
   }

   return most_frequent;

}

int k = n-1;
   
   while (k >= 0) {
      
      if (d->data.freq > wfs[k].freq) {
         break;
      }
      k--;

   }
   
   if (k > 0) {
      for (int i = 0; i < k; i++) {
         wfs[i] = wfs[i+1];
      }
   
   }
   
   wfs[k] = d->data;