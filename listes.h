#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales 
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne 
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

typedef struct cellule {

    char command;
    struct cellule *suivant;

} cellule_t;

typedef struct cellule_double{

    int valeur_entier;
    cellule_t *groupe_de_commande;
    struct cellule_double *suivant;

} cellule_double;

struct sequence {
    cellule_t *tete;
};

typedef struct sequence sequence_t;

typedef struct pile{
    cellule_double *tete;
} pile;
    
cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t*);

pile *creer_pile(); 
void vider_pile(pile *p);
void detruire_pile_double(pile *p);



void conversion (char *texte, sequence_t *seq);

void afficher (sequence_t* seq);
void afficher_pile_double (pile *seq);

void empiler_int(pile *p, int x);
void empiler_groupe_de_commande(pile *p, cellule_t *groupe_de_commande);

int depiler(pile *p, int *valeur_entier, cellule_t **groupe_de_commande);
cellule_t *depiler_groupe_de_commande(pile *p);
int depiler_int(pile *p);

void empiler_groupe_de_commande(pile *p, cellule_t *groupe_de_commande);

void addition(pile *p);
void soustraction(pile *p);
void multiplication(pile *p);
void condition(pile *Pile, cellule_t *Routine);
void execute(pile *Pile, cellule_t *Routine);
void echange(pile *Pile);
void clone(pile *p, cellule_t Routine);

#endif
