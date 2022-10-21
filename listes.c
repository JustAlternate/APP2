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
    cel->suivant = NULL;
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
  while(texte[i]!='\0'){
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

pile *creer_pile(){
  pile *p = malloc(sizeof(pile));
  p->tete = NULL;
  return p;
}

void detruire_pile_double(pile *p){
  cellule_double *cur = p->tete;
  cellule_double *prec;
  while (cur != NULL)
  {
    prec = cur;
    cur = cur->suivant;
    detruireCelluleDouble(prec);
  }
  free(p);
}


void empiler_int(pile *p, int x){
  cellule_double *cel = nouvelleCelluleDouble();
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

int depiler_int(pile *p){
  printf("pile apres depilage (GDC) : ");
  afficher_pile_double(p);
  printf("\n");
  cellule_double *cel = p->tete;
  int resultat = cel->valeur_entier;

  p->tete = p->tete->suivant;
  free(cel);
  printf("pile apres depilage (GDC) : ");
  afficher_pile_double(p);
  printf("\n");
  return resultat;
}

cellule_t *depiler_groupe_de_commande(pile *p){
  printf("pile avant depilage (GDC) : ");
  afficher_pile_double(p);
  printf("\n");
  cellule_double *cel = p->tete;
  cellule_t *resultat = cel->groupe_de_commande;

  p->tete = p->tete->suivant;
  free(cel);
  printf("pile apres depilage (GDC) : ");
  afficher_pile_double(p);
  printf("\n");
  return resultat;
}


int depiler(pile *p, int *valeur_entier, cellule_t **groupe_de_commande){
  if (p->tete == NULL){
    return -1;
  }

  int resultat;

  cellule_double *cel = p->tete;

  if (cel->groupe_de_commande == NULL){
    *valeur_entier = cel->valeur_entier;
    resultat = INT;
    
  }else{
    *groupe_de_commande = cel->groupe_de_commande;
    resultat = GDC;
  }

  p->tete = p->tete->suivant;
  free(cel);
  return resultat;
}

cellule_t *dernier_suite_cellule_t(cellule_t *cur){
  if (cur == NULL) {
    return NULL;
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
      if (cur->groupe_de_commande == NULL){
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

cellule_t *copie_suite_cellule_t(cellule_t *cel){
  // normalement il y a au moins une cellule à copier
  cellule_t *premiere_copie = nouvelleCellule();
  premiere_copie->command = cel->command;
  cellule_t *cur_copie = premiere_copie;
  cellule_t *temp;
  while (cel->suivant !=NULL){
    cel = cel->suivant;
    temp = nouvelleCellule();
    temp->command = cel->command;
    cur_copie->suivant = temp;
    cur_copie = cur_copie->suivant;
  }
  return premiere_copie;
}


// Definition des fonctions arithmetique simple.

void addition(pile *p){
  char last_commande_2 = depiler_int(p);
  char last_commande = depiler_int(p);
  int resultat = last_commande + last_commande_2;
  empiler_int(p,resultat);
}
void soustraction(pile *p){
  char last_commande_2 = depiler_int(p);
  char last_commande = depiler_int(p);
  int resultat = last_commande - last_commande_2;
  empiler_int(p,resultat);
}
void multiplication(pile *p){
  char last_commande_2 = depiler_int(p);
  char last_commande = depiler_int(p);
  int resultat = last_commande * last_commande_2;
  empiler_int(p,resultat);
}


//fonction d'actions:
void condition(pile *p, cellule_t *Routine){
  if (p->tete == NULL){ // On va verifie que la pile contient bien 3 éléments (du bon type lol)
    return;
  }
  if (p->tete->suivant == NULL || type_cellule_double(p->tete) == INT){
    return ;
  }
  if (p->tete->suivant->suivant == NULL || type_cellule_double(p->tete->suivant) == INT){
    return ;
  }
  if (type_cellule_double(p->tete->suivant->suivant)==GDC){
    return ;
  }
  //tous est bon lancer la fonction:
  int booleen;
  cellule_t *choix1 = NULL;
  cellule_t *choix2 = NULL;
  choix1 = depiler_groupe_de_commande(p);//va dans choix1 (on a vérifié)
  choix2 = depiler_groupe_de_commande(p);//va dans choix2 (on a vérifié)
  booleen = depiler_int(p);//va dans booleen (on a vérifié)
  cellule_t *der;
  if (! booleen)
  {//on met choix1 dans la routine et on free choix2
    printf("le premier : %c \n", choix1->command);
    der = dernier_suite_cellule_t(choix1); // implémentation de la la fonction "sequance_t *sernier(sequence_t *prems)" à faire
    printf("le dernier est: %c \n", der->command);
    der->suivant = Routine->suivant;
    Routine->suivant = choix1;
    detruireCellule_t(choix2);
    return;
  }
  else
  {//et inversement
    der = dernier_suite_cellule_t(choix2); // implémentation de la la fonction "sequance_t *sernier(sequence_t *prems)" à faire
    printf("le premier : %c \n", choix1->command);
    der->suivant = Routine->suivant;
    printf("le dernier est: %c \n", der->command);
    Routine->suivant = choix2;
    detruireCellule_t(choix1);
  }
}

void echange(pile *p){ //il y a normalement au moins 2 elements
  cellule_double *cel1 = p->tete;
  p->tete = cel1->suivant;
  cel1->suivant = p->tete->suivant;
  p->tete->suivant = cel1;
}

void execute(pile * p, cellule_t *Routine){
  if(type_cellule_double(p->tete) == INT){
    printf("choix int\n");
    cellule_t *nouvelle_cel = nouvelleCellule();
    nouvelle_cel->command = depiler_int(p);
    nouvelle_cel->suivant = Routine->suivant;
    Routine->suivant = nouvelle_cel;
  }
  else{ // type_cellule_double(p->tete) == GDC
    printf("choix int\n");
    printf("1\n");
    cellule_t *premiere_cel = depiler_groupe_de_commande(p);
    printf("2\n");
    cellule_t *derniere_cel = dernier_suite_cellule_t(premiere_cel);
    printf("3\n");
    derniere_cel->suivant = Routine->suivant;
    printf("4\n");
    Routine->suivant = premiere_cel;
    printf("5\n");
  }
}

void clone(pile *p){
  if (type_cellule_double(p->tete) == INT){
    empiler_int(p, p->tete->valeur_entier);
  }
  else{//type_cellule_double(p->tete) == GDC
    cellule_t *temp = copie_suite_cellule_t(p->tete->groupe_de_commande);
    empiler_groupe_de_commande(p, temp);
  }
}

void boucle(pile *p, cellule_t *Routine){
  int i = depiler_int(p);
  if (i <= 0){
    cellule_t *cel_a_detruire = depiler_groupe_de_commande(p);
    detruireCellule_t(cel_a_detruire);
    return;
  }
  clone(p);
  cellule_t *nouvelle_cel = nouvelleCellule();
  nouvelle_cel->command = 'B';
  nouvelle_cel->suivant = Routine->suivant;
  Routine->suivant = nouvelle_cel;
  execute(p, Routine);
  i--;
  empiler_int(p, i);
}