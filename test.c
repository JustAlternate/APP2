#include <stdio.h>
#include <stdlib.h>

typedef struct c {
  int truc;
  struct c *suivant;
} cel;

int main(){

  cel *cellule = malloc(sizeof(cel));
  if (cellule->suivant == NULL){
    printf("je suis nul\n");
  }
  if (cellule->suivant->suivant == NULL){
    printf("Je suis doublement nul :( \n");
  }
}
