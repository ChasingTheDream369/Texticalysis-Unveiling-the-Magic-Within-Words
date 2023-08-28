#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HTABLE_SIZ 1001
#define MAX_LINE_SIZ 1024

/* Hash Table */
typedef struct node* link;
struct node { char *token; int freq; link next; };

link htable[HTABLE_SIZ] = { NULL }; /* Table of lists (#buckets) */
int size = 0; /* Size (number of elements) of hash table */

unsigned int hash (char *tok );
void insert_ht (char *data);
void clear_ht ( );
void print_ht ( );

void Process(FILE *fp);


int main(int argc, char *argv[])
{
    int i;
    FILE *fp;
    for (i=1; i < argc; i++)
    {
        fp = fopen(argv[i],"r");
        if (NULL == fp)
        {
            fprintf(stderr,"Problem opening file: %s\n",argv[i]);
            continue;
        }
    Process(fp);
    fclose(fp);
    }
    print_ht();
    clear_ht();
    return 0;
}


void Process(FILE *fp)
{
    const char *seperators = " ?!'\";,.:+-*&%(){}[]<>\\\t\n";

    char line[MAX_LINE_SIZ];
    char *s;
    while((fgets(line,MAX_LINE_SIZ, fp)) != NULL)
    {
        for (s=strtok(line,seperators); s; s=strtok(NULL,seperators))
            insert_ht(s);
        }
    }

/* Hash Function */
unsigned int hash(char *tok)
{
    unsigned int hv = 0;
    while (*tok)
        hv = (hv << 4) | toupper(*tok++);
    return hv % HTABLE_SIZ;
}


void insert_ht(char *token)
{
……………………………………………
}
void clear_ht()
{
……………………………………………
}
int compare(const void *elem1, const void *elem2)
{
……………………………………………
}
void print_ht()
{
    int i, j=0;
    link l, *vector = (link*) malloc(sizeof(link)*size);
    for (i=0; i < HTABLE_SIZ; i++)
        for (l=htable[i]; l; l=l->next)
            vector[j++] = l;
        qsort(vector,size,sizeof(link),compare);
        for (i=0; i < size; i++)
            printf("%-50s\t%7d\n",vector[i]->token,vector[i]->freq);
        free(vector);
}
c hashtable frequency words