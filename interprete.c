#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}


int interprete (sequence_t* seq, bool debug)
{
    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    if (!silent_mode){
      printf ("Programme:");
      afficher(seq);
      printf ("\n");
    }
    if (debug) stop();

    int ret;

    char commande;

    cellule_t *current_cel;
    current_cel = seq->tete;
    pile *pile_calculette = creer_pile();

    while (current_cel != NULL) { 
        //printf("%c\n",current_cel->command);
        commande = current_cel->command;
        switch (commande) {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE){
                  detruire_pile_double(pile_calculette);
                  return VICTOIRE;
                }
                if (ret == RATE){
                  detruire_pile_double(pile_calculette);
                  return RATE;
                }   
                break;
            case 'D':
                droite();
                break;
            case 'G':
                gauche();
                break;
            case 'P':
                pose(depiler_int(pile_calculette));
                break;
            case 'M':
                empiler_int(pile_calculette,mesure(depiler_int(pile_calculette)));
                break;
            case '+':
                addition(pile_calculette);
                break;
            case '-':
                soustraction(pile_calculette);
                break;
            case '*':
                multiplication(pile_calculette);
                break;
            case '?':
                condition(pile_calculette, current_cel);
                break;
            case '{':// ça se peut que je casse tout
                empiler_groupe_de_commande(pile_calculette, current_cel->groupe_de_commande);
                break;
            case '!':
                //printf("entree d'exec\n");
                execute(pile_calculette, current_cel);
                //printf("sortie d'exec\n");
                break;
            case 'X':
                echange(pile_calculette);
                break;
            case 'C':
                clone(pile_calculette);
                break;
            case 'B':
                boucle(pile_calculette, current_cel);
                break;
            case 'R':
                rotation(pile_calculette);
                break;
            case 'I':
                ignore(pile_calculette);
                break;
            case 'Z':
                zinversion(pile_calculette);
                break;
            default:
                //printf("Caractère spécial: '%c'\n", commande);
                if (commande <= '9' && '0' <= commande){
                  empiler_int(pile_calculette,(commande-'0'));
                }
                break;
        }
        //printf("Pile : ");
        //afficher_pile_double(pile_calculette);
        seq->tete = current_cel->suivant;
        free(current_cel); // current_cel->groupe_de_command est sois déjà free sois déjà utilisé
        current_cel = seq->tete;
        // ce serait bien de free la précédente cel
        /* Affichage pour faciliter le debug */
        //afficherCarte();
        //printf ("Programme:");
        //afficher(seq);
        //printf ("\n");
        if (debug) stop();
    }
    
    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    detruire_pile_double(pile_calculette);
    return CIBLERATEE;
}
