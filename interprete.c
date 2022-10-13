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
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;

    cellule_t *current_cel;
    current_cel = seq->tete;

    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();


    int ret;         //utilisée pour les valeurs de retour

    while (current_cel != NULL) { //à modifier: condition de boucle
        commande = current_cel->command;
        switch (commande) {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                break; /* à ne jamais oublier !!! */
            case 'D':
                droite();
                break;
            case 'G':
                gauche();
                break;
            case 'P':
                break;
            case 'M':
                break;
            default:
                eprintf("Caractère inconnu: '%c'\n", commande);
        }
        current_cel = current_cel->suivant;
        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
