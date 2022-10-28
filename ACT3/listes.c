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
void vider_liste_t(sequence_t *seq){
  if (seq->tete == NULL){
    return;
  } 
  cellule_t *cur;
  cur = seq->tete;
  cellule_t *suiv = cur->suivant;
  seq->tete = NULL;
  while(suiv != NULL){
    detruireCellule(cur);
    cur = suiv;
    suiv = suiv->suivant;
  }
  detruireCellule(cur);
}

void transform(char *c, sequence_t *seq){
  cellule_t *cel = nouvelleCellule();
  cel->command = *c;
  cel->suivant = NULL;

  cellule_t *cur;
  cur = seq->tete;
  if (cur==NULL){ // Si jamais on est dans le cas spéciale où la LC est vide.
    seq->tete = cel;
    return;
  }
  while(cur->suivant!=NULL){ // Sinon on append a la fin de la LC la nouvelle cellule.
    cur = cur->suivant;
  }
  cur->suivant = cel;
}

void conversion (char *texte, sequence_t *seq)
{
  vider_liste_t(seq);
  int i = 0;
  while(texte[i]!='\n' && texte[i]!='\0'){
    transform(&texte[i],seq);
    i++;
  }
}

void afficher (sequence_t* seq)
{
    assert (seq); /* Le pointeur doit être valide */
    cellule_t *cur;
    cur = seq->tete;
    while(cur!=NULL){
      printf("%c ",cur->command);
      cur = cur->suivant;
    }
}

// Definition des fonctions de pile FILO.


pile *creer_pile(){
  pile *p = malloc(sizeof(pile));
  p->tete = NULL;
  return p;
}

void vider_pile(pile *p){
  cellule_t *cur = p->tete;
  cellule_t *prec;
  while (cur != NULL){
    prec = cur;
    cur = cur->suivant;
    detruireCellule(prec);
  }
}

void detruire_pile(pile *p){
  vider_pile(p);
  free(p);
}

int est_vide(pile *p){
  return(p->tete == NULL);
}

char sommet(pile *p){
  if (est_vide(p)){
    printf("Erreur la pile est vide\n");
    return '!';
  }
  else{
    return p->tete->command; 
  }
}

char depiler(pile *p){
  if (p->tete != NULL){
    cellule_t *cel = p->tete;
    char c = cel->command;
    p->tete = p->tete->suivant;
    detruireCellule(cel);
    return c;
  }
  return '!';
}



void afficher_pile(pile * p){
    assert (p); /* Le pointeur doit être valide */
    cellule_t *cur;
    cur = p->tete;
    while(cur!=NULL){
      printf("%d ",cur->command);
      cur = cur->suivant;
    }
}

void empiler(pile *p, char x){
  cellule_t *cel = nouvelleCellule();
  cel->command = x;
  cel->suivant = p->tete;
  p->tete = cel;
}


// Definition des fonctions arithmetique simple.

void addition(pile *p){
  if (!est_vide(p)){
    char last_commande_2 = depiler(p);
    if (!est_vide(p)){
      char last_commande = depiler(p);
      int resultat = last_commande + last_commande_2;
      empiler(p,resultat);
    }else{
      empiler(p,last_commande_2);
    }
  }
}

void soustraction(pile *p){ 
  if (!est_vide(p)){
    char last_commande_2 = depiler(p);
    if (!est_vide(p)){
      char last_commande = depiler(p);
      char resultat = last_commande - last_commande_2;
      empiler(p, resultat);
    }else{
      empiler(p,last_commande_2);
    }
  }
}
void multiplication(pile *p){
  if (!est_vide(p)){
    char last_commande_2 = depiler(p);
    if (!est_vide(p)){
      char last_commande = depiler(p);
      char resultat = last_commande * last_commande_2;
      empiler(p, resultat);
    }else{
      empiler(p,last_commande_2);
    }
  }
}

// Definition des fonctions poser et mesurer :


