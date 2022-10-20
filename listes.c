#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"

#define GDC 0 //est une groupe de commande.
#define INT 1

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;



cellule_t* nouvelleCellule (void)
{
    cellule_t *cel = malloc(sizeof(cellule_t)); 
    return cel;
}

void detruireCellule (cellule_t* cel)
{
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


void afficher_pile(pile * p){
    assert (p); /* Le pointeur doit être valide */
    cellule_t *cur;
    cur = p->tete;
    while(cur!=NULL){
      printf("%d ",cur->command);
      cur = cur->suivant;
    }
}

// Definition méthodes pour act4 :

cellule_double* nouvelleCelluleDouble(void)
{
    cellule_double *cel = malloc(sizeof(cellule_double)); 
    cel->valeur_entier = -1;
    cel->groupe_de_commande = NULL;
    return cel;
}

void detruireGroupeDeCommande(cellule_double *cel_db){
  cellule_t *prec;
  cellule_t *cur = cel_db->groupe_de_commande;
  while (cur != NULL){
    prec = cur;
    cur = cur->suivant;
    free(prec);
  }
}

// version 2 pas sur de l'implementation
void detruireCellule_t(cellule_t *cur){
  cellule_t *prec;
  while (cur != NULL)
  {
    prec = cur;
    cur = cur->suivant;
    free(prec);
  }
}

void detruireCelluleDouble (cellule_double* cel)
{
  if (cel->groupe_de_commande != NULL){
    detruireGroupeDeCommande(cel);
  }
  free(cel);
}



void empiler_int(pile *p, int x){
  cellule_t *cel = nouvelleCelluleDouble();
  cel->valeur_entier = x;
  cel->groupe_de_commande = NULL;
  cel->suivant = p->tete;
  p->tete = cel;
}


void empiler_groupe_de_commande(pile *p, cellule_t *groupe_de_commande){ // Passer en argument un groupe de commande de la forme A->G->D->A->NULL
  if (groupe_de_commande == NULL){
    cellule_t *cel_espace = nouvelleCellule();
    cel_espace->command = ' ';
    cel_espace->suivant = NULL;
    groupe_de_commande = cel_espace;
  }
  cellule_double *cel = nouvelleCelluleDouble();
  cel->groupe_de_commande = groupe_de_commande;
  cel->suivant = p->tete;
  p->tete = cel;
}

int depiler(pile *p, int *valeur_entier, cellule_t *groupe_de_commande){
  if (p->tete == NULL){
    return -1;
  }

  int resultat;

  cellule_double *cel = p->tete;

  if (cel->groupe_de_commande == NULL){
    valeur_entier = cel->valeur_entier;
    resultat = INT;
    
  }else{
    groupe_de_commande = cel->groupe_de_commande;
    resultat = GDC;
  }

  p->tete = p->tete->suivant;
  detruireCelluleDouble(cel);
  return resultat;
}

cellule_t *dernier_suite_cellule_t(cellule_t *cur){
  if (cur == NULL) {
    return NULL
  }
  while (cur->suivant != NULL)
  {
    cur = cur->suivant;
  }
  return cur;
}

void afficher_suites_de_cellule_t(cellule_t *cel){
  while(cel !=NULL){
    printf("%c ",cel->command);
    cel = cel->suivant;
  }
}

void afficher_pile_double(pile * p){
    assert (p);
    cellule_double *cur;
    cur = p->tete;
    while(cur!=NULL){
      if (cel->groupe_de_commande == NULL){
        printf("%d ",cur->valeur_entier);
      }else{
        printf("{");
        afficher_suites_de_cellule_t(cur->groupe_de_commande);
        printf("} ");
      }
      cur = cur->suivant;
    }
}

int type_cellule_double(cellule_double *cel_db){
  if(cel_db->groupe_de_commande == NULL){
    return INT;
  }
  return GDC; 
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



//fonction d'actions:
void condition(pile *Pile, cellule_t *Routine){
  if (Pile->tete == NULL){ // On va verifie que la pile contient bien 3 éléments (du bon type lol)
    return;
  }
  if (Pile->tete->suivant == NULL || type_cellule_double(Pile->tete) == INT){
    return;
  }
  if (Pile->tete->suivant->suivant == NULL || type_cellule_double(Pile->tete->suivant) == INT){
    return;
  }
  if (type_cellule_double(Pile->tete->suivant->suivant)==GDC){
    return;
  }
  //tous est bon lancer la fonction:
  int booleen;
  sequence_t *choix1;
  sequence_t *choix2;
  sequence_t *poubelle;
  depiler(Pile, &booleen, choix1);//va dans choix1 (on a vérifié)
  depiler(Pile, &booleen, choix2);//va dans choix2 (on a vérifié)
  depiler(Pile, &booleen, poubelle);//va dans booleen (on a vérifié)
  sequence_t *der;
  if (booleen)
  {//on met choix1 dans la routine et on free choix2
    der = dernier(choix1); // implémentation de la la fonction "sequance_t *sernier(sequence_t *prems)" à faire
    der->suivant = Routine->suivant;
    Routine->suivant = choix1;
    detruireCellule_t(choix2);
    return;
  }
  else
  {//et inversement
    der = dernier(choix2); // implémentation de la la fonction "sequance_t *sernier(sequence_t *prems)" à faire
    der->suivant = Routine->suivant;
    Routine->suivant = choix2;
    detruireCellule_t(choix1);
  }
}
