// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre_binaire.h"
#include "bin_file.h"

#define NB_CHAR 256

void raise_error(char *s) {
    printf("Error : %s\n", s);
    printf("For more info use ./huffman -help\n");
    exit(1);
}

// renvoie la position du caractere c dans la liste T si le caractere n'est pas dans la liste renvoie -1
int search_char(arbre T[], char c, int length) {
    int i;

    for (i = 0; i < length; i++) {
        if (T[i]->elt == c) {
            return i;
        }
    }
    return -1;
}

// insere dans la liste T des feuille contennant des caractere unique
// avec leur poids correspondant a leur nombre d'apparaition dans le texte
// retourne la longueur de T
int repartition(arbre T[], char texte[]) {
    int i, char_pos, n;
    
    n = 0;
    for (i = 0; i < strlen(texte); i++) {
        char_pos = search_char(T, texte[i], n);
        if (char_pos == -1) {
            if (texte[i] == '\1')
            {
                raise_error("\'\1\' is in the input text");
            }
            else if (texte[i] == '\2')
            {
                raise_error("\'\2\' is in the input text");
            }
            
            T[n] = creer_feuille(texte[i], 1);
            n++;
        } else {
            T[char_pos]->poids++;
        }
    }
    return n;
}

// tri les feuille d'une liste selon un poids decroissant
// algo : tri par selection (https://en.wikipedia.org/wiki/Selection_sort)
void tri_feuilles(arbre T[], int length) {
    int i, j, max;
    arbre temp;

    for (i = 0; i < length - 1; i++) {
        // trouver le maximum
        max = i;
        for (j = i + 1; j < length; j++) {
            if (T[j]->poids > T[max]->poids) {
                max = j;
            }
        }
        // echanger le maximum de T[i:lenght] avec T[i]
        temp = T[i];
        T[i] = T[max];
        T[max] = temp;
    }
}

// insere une node dans une liste selon sont poids pour conserver le tri des poids decroissants
void inserer_node(arbre T[], arbre a, int length) {
    int i, j;

    i = length - 1;
    while (T[i]->poids < a->poids && i > 0) {
        i--;
    }

    for (j = length; j > i; j--) {                // decale tout les element d'un cran
        T[j] = T[j - 1];                          // pour faire de la place pour l'insersion
    }
    T[i] = a;
}

arbre construire_arbre(arbre T[], int T_length) {
    arbre a1, a2, a3;

    while (T_length > 1) {
        //prendre les deux derniers
        a1 = T[T_length - 1];
        a2 = T[T_length - 2];

        //les fusionner
        a3 = creer_arbre(a1, a2);

        //inserer le resultat
        inserer_node(T, a3, T_length);
        T_length--;
    }
    return T[0];
}

arbre huffman(char *text) {
    int T_length;
    arbre a, T[NB_CHAR];

    T_length = repartition(T, text);
    tri_feuilles(T, T_length);
    a = construire_arbre(T, T_length);
    return a;
}

void trouver_feuilles_aux(arbre a, arbre feuilles[], int *n, char* path) {
    int len = strlen(path);
    char copy_path1[len + 1], copy_path2[len + 1];

    if (a->elt != '\0')
    {
        feuilles[*n] = a;
        a->path = malloc(strlen(path)*sizeof(char));
        strcpy(a->path, path);
        (*n)++;
    }
    else
    {
        int i;
        strcpy(copy_path1, path);
        /*
        for (i = 0; i < len; i++)
        {
            copy_path1[]
        }
        
        copy_path1 = strfus(copy_path1, "0");
        trouver_feuilles_aux(a->fils_gauche, feuilles, n, copy_path1);

        strcpy(copy_path2, path);
        copy_path2 = strfus(copy_path2, "1");
        trouver_feuilles_aux(a->fils_droit, feuilles, n, copy_path2);
        */
    }
}

int trouver_feuilles(arbre a, arbre feuilles[]) {
    int n = 0;

    trouver_feuilles_aux(a, feuilles, &n, "");
    return n;
}

char* encode_text(arbre a, char *text) {
    char *result, c;
    int i, feuilles_len, pos;
    arbre feuilles[NB_CHAR];

    feuilles_len = trouver_feuilles(a, feuilles);

    result = "";
    for (i = 0; i < strlen(text); i++)
    {
        c = text[i];
        pos = search_char(feuilles, c, feuilles_len);
        if (pos == -1)
        {
            // erreur theoriquement impossible
            printf("debug : oskour\n");
        }
        result = strfus(result, feuilles[pos]->path);
    }
    
    return result;
}

char* decode_text(arbre a, char *binary) {
    char *result = malloc(strlen(binary)*sizeof(char));
    int i, n;
    arbre temp;

    temp = a;
    n = 0;
    // si arbre est une feuille unique
    if (temp->elt != '\0')
    {
        result[n] = a->elt;
        n++;
        temp = a;
    }
    // pour chaque bit de l'encoded
    for (i = 0; i < strlen(binary); i++)
    {
        if (binary[i] == '0') {
            temp = temp->fils_gauche;
        } else {
            temp = temp->fils_droit;
        }
        
        // si temp est une feuille
        if (temp->elt != '\0') {
            result[n] = temp->elt;
            n++;
            temp = a;
        }
    }
    result[n] = '\0';
    return result;
}
