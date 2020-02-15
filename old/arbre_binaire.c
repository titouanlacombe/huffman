// arbre_binaire.c

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "arbre_binaire.h"

arbre creer_arbre_vide (void)
{
    return NULL;
}
 
int est_arbre_vide(arbre a)
{
    return (a == NULL);
}

arbre creer_arbre(arbre fg, arbre fd)
{
    arbre a;
    a = malloc(sizeof(noeud));

    a->elt = '\0';
    a->fils_droit = fd;
    a->fils_gauche = fg;
    a->poids = fg->poids + fd->poids;
 
    return(a);
}

arbre fils_gauche(arbre a)
{
    assert(!est_arbre_vide(a));
    return(a->fils_gauche);
}
 
arbre fils_droit(arbre a)
{
    assert(!est_arbre_vide(a));
    return(a->fils_droit);
}
 
Elt racine(arbre a)
{
    assert(!est_arbre_vide(a));
    return(a->elt);
}

arbre creer_feuille(Elt e, int poids)
{
    arbre a;
    a = malloc(sizeof(noeud));

    a->elt = e;
    a->poids = poids;
    a->fils_droit = NULL;
    a->fils_gauche = NULL;
 
    return(a);
}

int est_feuille(arbre a)
{
    if (est_arbre_vide(a)) {
        return 0;
    }
    return (est_arbre_vide(a->fils_gauche) && est_arbre_vide(a->fils_droit));
}
 
void print_arbre_aux(arbre a)
{
    if (a->elt == '\0')
    {
        printf("Node :");
        printf("fg(");
        print_arbre_aux(a->fils_gauche);
        printf("), fd(");
        print_arbre_aux(a->fils_droit);
        printf(")");
    } 
    else
    {
        printf("Feuille :%c", a->elt);
        printf(", %i", a->poids);
        printf(", %s", a->path);
    }
}
 
void print_arbre(arbre a)
{
    print_arbre_aux(a);
    printf("\n");
}

void print_liste_arbre(arbre T[], int length) {
    int i;
    
    for (i = 0; i < length; i++)
    {
        if (!est_arbre_vide(T[i])) {
            printf("T[%i] : ", i);
            print_arbre(T[i]);
            printf("\n");
        }
        else
        {
            printf("T[%i] : empty\n", i);
        }
        
    }
}
// Fusionne deux char* dans un troisieme
/*char* strfus(char* s1, char* s2) {
    int n1 = strlen(s1);
    int n2 = strlen(s2);
    int n3 = n1 + n2 + 1;
    int i;
    char *s3;
    s3 = malloc(n3*sizeof(char));

    for (i = 0; i < n1; i++)
    {
        s3[i] = s1[i];
    }
    for (i = 0; i < n2; i++)
    {
        s3[n1 + i] = s2[i];
    }
   // s3[n3 - 1] = '\0';
    return s3;
}*/

//Serialise notre arbre sous format char*
char *serializer_arbre(arbre a) {
    char c, *s1, *s2 ;
    
    
    if (a->elt == '\0')
    {
        c = '\1';
        s1 = serializer_arbre(a->fils_gauche);
        s2 = serializer_arbre(a->fils_droit);
        int n = strlen(s1)+strlen(s2) + 1;
        char s3[n];
        for (int i=0;i<strlen(s1);i++) {
            s3[i]=s1[i];
        }
        for (int i=strlen(s1);i<strlen(s2);i++) {
            s3[i]=s2[i-strlen(s1)];
        }
        s3[n-1]=c;
        return s3;
    } 
    else
    {
        char s3[1];
        s3[0] = a->elt;
        return s3;
    }
}

arbre deserializer_arbre_aux(char* s, int *n) {
    arbre a;

    if (s[*n] != '\1')
    {
        a = creer_feuille(s[*n], 0);
        (*n)++;
        return a;
    }
    else
    {
        a = creer_feuille('\0', 0);
        (*n)++;
        a->fils_gauche = deserializer_arbre_aux(s, n);
        a->fils_droit = deserializer_arbre_aux(s, n);
        return a;
    }
}

arbre deserializer_arbre(char* s) {
    int n = 0;
    arbre a;
    a = deserializer_arbre_aux(s, &n);
    return a;
}

void free_noeud(arbre a)
{
    free(a);
}
 
void free_arbre(arbre a)
{
    if(est_arbre_vide(a)){
        return;
    } else {
        free_arbre(fils_droit(a));
        free_arbre(fils_gauche(a));
        free_noeud(a);
    }
}