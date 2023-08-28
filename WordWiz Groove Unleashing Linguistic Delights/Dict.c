// COMP2521 20T2 Assignment 1
//BY SHUBHAM JOHAR-z5284381
// Dict.c ... implementsation of Dictionary ADT

/*REFERENCING- This is to inform you that the some functions in this file namely-Dict.c have been adopted to be used in the-Assignment 1 comp 2521- 20 T2 
from LAB-04 comp 2521 20 T2 which includes the generic insertion, height, rotation and searching functions for the standard AVL tree data structure.The code used 
from LAB 04 has just been used as a reference point for the implementation of this DICT ADT which also uses an AVL tree data structure, and i have tried to 
use the code directly as less as possible and in no way intended to copy the content directly, but the functions namely 'static int height (Link d)', 
static int max(int a, int b), static Link rotateLeft(Link n) and static Link rotateRight(Link n) are almost completely the same as LAB04 counterparts of it, 
and i hererby acknowledge/reference the course authority of comp2521 for functions 'height' and 'max' as they were initially provided by the course authority 
in the LAB04 starter code whereas rotate functions were the part of lab exercise, other functions namely- static Link doDictInsert (Link d, new_word w), 
static Link AddToDict(new_word w); static WFreq *DoDictFind (Link d, new_word w);are also almost the same as their LAB-04 counterparts with slight changes 
made to them with respect to the Dict ADT,header files and special requirements being used in the ASS 1.I would again like to clarify that i have in no way tried to 
copy/plagiarise any code/content completely and tried to use as less already implemented code as possible, and whereever used i have referenced it herewith. */


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
static Link AddToDict(new_word w);
static WFreq *DoDictFind (Link d, new_word w);
static void dofindTopN (Link d, WFreq *wfs, int num_in_result);
static int max(int a, int b);
static void doshowDict(Link d);


// create's and return new empty Dictionary
Dict newDict() {  
   
   //malloc's enough size for the Dict ADT.
   Dict create_dict = malloc(sizeof(*create_dict));
   
   //Handling the error if it can't malloc space for it
   if (create_dict == NULL) {
      fprintf(stderr, "Insufficient memory!\n");
      exit(EXIT_FAILURE);
   }
   
   //set the root/DictNode of Dict ADT as NULL.
   create_dict->tree = NULL;
   return create_dict;

}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word.
WFreq *DictInsert(Dict d, new_word add_word) {
   
   /*Insets the new word by using the helper function recursively
   and sends back the new root of the AVL tree changed due to rotation
   every time and reset it to d->tree.*/ 
   d->tree = doDictInsert(d->tree, add_word);

   //Searches for the recently_added (Word,freq) pair and returns it.
   WFreq* recently_addedd = DictFind(d, add_word);

   return recently_addedd;

}

//Inserting into AVL tree following the standard AVL tree insertion 
//recusrive algorithm.

static Link doDictInsert (Link d, new_word insetDictWord) {
   
   //BASE case: when DictNode is NULL then add node to dict ADT.
   if (d == NULL) {
      return AddToDict(insetDictWord);
   }

    // insert recursively by comparing words lexicographically
   int lexicographic_insert = strcmp(insetDictWord, d->data.word);
   
   //when word is smaller than current nodes word go to left sub-tree
   if (lexicographic_insert < 0) {
      d->left = doDictInsert(d->left, insetDictWord);
   }
   
   //when word is greater than current nodes word go to right sub-tree
   else if (lexicographic_insert > 0) {
      d->right = doDictInsert(d->right, insetDictWord);
   }
   
   //if word to be added is exactly same as the node's word return the node
   //directly.
   else { 
        return d;
   }

   // insertion done
   // correct the height of the current subtree
   d->height = 1 + max(height(d->left), height(d->right));
    
   // rebalance the tree using standard balance factor approach
   int balance_factor = height(d->left) - height(d->right);
   
   //when the balance factor is more than 1 then it's left heavy subtree
   if (balance_factor > 1) {
      
      //First rotate the right sub-tree to left
      if (strcmp(insetDictWord, d->left->data.word) > 0) {
         d->left = rotateLeft(d->left);
      }
      //and then Rotate the tree right after that.
      d = rotateRight(d);

   }
  
   //when the balance factor is more than 1 then it's right heavy subtree 
   if (balance_factor < -1) {   
      
      //First rotate the left sub-tree to right
      if (strcmp(insetDictWord, d->right->data.word) < 0) {
         d->right = rotateRight(d->right);
      }
      
      //and the rotate the tree to left to rebalance it 
      d = rotateLeft(d); 

   } 
  
    /* return the new root after rotation */ 
   return d;

}

//helper height function 
static int height(Link n) {
   
   //returns -1 when there is no node in tree
   if (n == NULL) {
      return -1;
   }
   
   //returns the height of the desired node otherwise.
   else {
      return n->height;
   }

}

//max helper function-used to find the maximum
//between two int values(particularly heights in this case).
static int max(int a, int b) {
   
   return a > b ? a : b;

}

//Standard rotation left algorithm defined by swapping
static Link rotateLeft(Link n) {
   
   //if dictionary is empty then don't rotate anything.
   if (n == NULL) {
      return NULL;
   }
   
   //if the right-sub tree is empty don't rotate and directly 
   //return the node. 
   else if (n->right == NULL) {
      return n;
   }
    
   else { 
      
      //set's the right subtree of current root as rotating variable
      Link rotate = n->right;
      
      //points roate_new to rotate's left sub-tree
      Link rotate_new = rotate->left;
      
      //resets the left-subtree of rotate as the current node(n) to
      //make rotate as the new root of the curret sub-tree.
      rotate->left = n;

      //add the lef-subtree of rotate as the right child of previous root.
      n->right = rotate_new;
      
      //Reset the heights of new_root and previous root after rotation
      n->height = 1 + max(height(n->left), height(n->right));
      rotate->height = 1 + max(height(rotate->left), height(rotate->right));

      //sends back the new root which is the rotate variable  
      return rotate; 
   
   }
   
   return n;
}

//Standard Rotate right algorithm defined by swapping
static Link rotateRight(Link n) {
   
   //if dictionary is empty then don't rotate anything.
   if (n == NULL) {
      return NULL;
   }
   
   //if the left-sub tree is empty don't rotate and directly 
   //return the node.
   else if (n->left == NULL) {
      return n;
   }

   else {

      //set's the left subtree of current root as rotating variable
      Link rotate = n->left;

      //points roate_new to rotate's right sub-tree
      Link rotate_new = rotate->right;

      //resets the right-subtree of rotate as the current node(n) to
      //make rotate as the new root of the curret sub-tree.
      rotate->right = n;

      //add the right-subtree of rotate as the left child of previous root.
      n->left = rotate_new; 
      
      //Reset the heights of rotate and previous root after rotation
      n->height = 1 + max(height(n->left), height(n->right));
      rotate->height = 1 + max(height(rotate->left), height(rotate->right));
      
      //return's new root which is the rotate variable.
      return rotate;
   }
    
   return n;
    
}

//Add's the node to the Dict return the added Node
static Link AddToDict(new_word w) {
   
   //Creates and malloc's desired space for a new_DictNode.
   Link new_DictNode = malloc(sizeof(*new_DictNode));

   //If it does'nt allocate memory print the error and exit
   if (new_DictNode == NULL) {
      fprintf(stderr, "Insufficient memory!\n");
      exit(EXIT_FAILURE);
   }
   
   //copy the new_word to the memory using strdup and point the
   //node's data.word to the added word. For more reference on 
   //strdup- refer https://man7.org/linux/man-pages/man3/strdup.3.html 
   //(man page for strdup).
   new_DictNode->data.word = strdup(w);

   //set the frequency of added node to 0
   new_DictNode->data.freq = 0;

   //set the left and right Sub-tree/children to NULL initially
   new_DictNode->left = NULL;
   new_DictNode->right = NULL;
   
   return new_DictNode;

}

//find Word in Dictionary
//return pointer to (word,freq) pair, if found
//otherwise return NULL

WFreq *DictFind(Dict d, new_word w) {
   
   //Finds the (word,freq) pair and returns it
   WFreq *search_word = DoDictFind(d->tree, w); 
   return search_word;

}

//Helper function defined recursively to find the 
//desired (word,freq) pair.

static WFreq *DoDictFind (Link d, new_word w) {
   
   //Base case: when the DictNode is NULL/or when there are 
   //no elements in dictionary, then return NULL.
   if (d == NULL) {
      return NULL;
   }
   
   //when the word is less than the current node's word it 
   //moves to the left sub-tree. 
   
   else if (strcmp(w,d->data.word) < 0) {
		return DoDictFind(d->left, w);
   }
   
   //when the word is more than the node's word it 
   //moves to the right sub-tree.
	
   else if (strcmp(w,d->data.word) > 0) {
		return DoDictFind(d->right, w);
   }
   
   //if word is exactly the same as the node's word
   //then return the node's (word,freq) pair.
   else {
		return &(d->data);
   }
	
   //returns the node's (word,freq) pair in default case
   return &(d->data);

}

/* Find's top N frequently occurring words in Dict
input: Dictionary, array of WFreqs, size of array
returns: #WFreqs in array, modified array*/

int findTopN(Dict d, WFreq *wfs, int num_in_result) {
   
   //helper function defined to find Top N frequencies
   dofindTopN(d->tree, wfs, num_in_result);
   
   //Prints the top num_in_result frequencies
   for (int i = num_in_result-1; i >= 0 ; i--) {
      printf ("%7d %s\n", wfs[i].freq, wfs[i].word);
   }
   
   //returns the num of elements in wfs array.
   return num_in_result;
   
}

//Helper function defined recursively to find the Top N 
//frequently occuring frequencies by updating the result in wfs array.

static void dofindTopN (Link d, WFreq *wfs, int num_in_result) {
   
   //Base case: when d is NULL then return to previous function call.
   if (d == NULL) {
      return;
   }
   
   //move's to the leftmost element in Dict-rep to start filling the 
   //array in lexicographic order for maintaining the lexicographic 
   //order when frequencies occur for different node's. 
   
   dofindTopN(d->left, wfs, num_in_result);  
   
   //set's freq_cmpre_add to the index of last element in 
   //resluts array.
   int freq_cmpre_add = num_in_result-1;
   
   //starts comparing the elements in the wfs array from the 
   //last_index to zeroth index and as soon as it finds an element in array
   //where current node's frequency is greater than array's element 
   //freq it breaks.

   while (freq_cmpre_add >= 0) {
      
      if (d->data.freq > wfs[freq_cmpre_add].freq) {
         break;
      }

      freq_cmpre_add--;

   }
   
   //Then shift all elements to left from the beginning till the
   //breakpoint of the freq_cmpre_add 
   if (freq_cmpre_add > 0) {
      
      for (int shift_left = 0; shift_left < freq_cmpre_add; shift_left++) {
         wfs[shift_left] = wfs[shift_left+1];
      }
   
   }
   
   //and add the node->data at the fre_cmpre_add position maintaing the order 
   //of freq(highest to lowest) from end to beginning.
   wfs[freq_cmpre_add] = d->data;
   
   //move to the right sub-tree
   dofindTopN(d->right, wfs, num_in_result);

}

// print a dictionary
void showDict(Dict d) {
   
   //helper function to show the dictionary ADT
   doshowDict(d->tree);
   
}

/*inorder traversal of the tree with printing the (Word,Freq) pair
from left to right which automatically prints the elements in 
lexicographic order as the insertion is lexicographic from left to right,*/

static void doshowDict(Link d) {
   
   if (d == NULL) {
      return;
   }

   doshowDict(d->left);
   printf("Word : %s, Freq : %d\n", d->data.word,d->data.freq);
   doshowDict(d->right);

}
