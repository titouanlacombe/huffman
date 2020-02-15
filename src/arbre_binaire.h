// arbre_binaire.h

typedef char Elt;

typedef struct znoeud
{
    Elt elt;                            // lettre encoder par le noeud
    int poids;                          // poids noeud (nombre d'apparaition de sa lettre)
    char* path;
    struct znoeud *fils_gauche;
    struct znoeud * fils_droit;
}noeud;

typedef struct znoeud * arbre;

// Type abstrait de données
arbre creer_arbre_vide (void);
int est_arbre_vide(arbre);
arbre creer_arbre(arbre, arbre);
arbre fils_gauche(arbre);
arbre fils_droit(arbre);
Elt racine(arbre);

// Fonctions outils
arbre creer_feuille(Elt, int);
int est_feuille(arbre);
//char* strfus(char*, char*);
char *serializer_arbre(arbre);
arbre deserializer_arbre(char*);
void free_noeud(arbre);
void free_arbre(arbre);
void print_arbre(arbre);
void print_liste_arbre(arbre[], int);
