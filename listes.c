#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;


cellule_t* nouvelleCellule (void)
{
    /* À compléter (utiliser malloc) */
    cellule_t *cel = malloc(sizeof(cellule_t)); 
    return cel;
}


void detruireCellule (cellule_t* cel)
{
    /* À compléter (utiliser free) */
    free(cel);
}

void transform(char *c, sequence_t *seq){
  cellule_t *cel = nouvelleCellule();
  cel->command = *c;
  cel->suivant = NULL;

  cellule_t *cur;
  cur = seq->tete;
  while(cur!=NULL){
    cur = cur->suivant;
    conversion(&cur->command,seq);
  }
  cur = cel;
}

void conversion (char *texte, sequence_t *seq)
{
  int i = 0;
  while(texte[i]!='\n'){
    transform(&texte[i],seq);
    i++;
  }
}

void afficher (sequence_t* seq)
{
    assert (seq); /* Le pointeur doit être valide */
    /* À compléter */
    cellule_t *cur;
    cur = seq->tete;
    while(cur!=NULL){
      printf("%c ",cur->command);
      cur = cur->suivant;
    }
}


