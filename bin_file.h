#include <stdio.h>

#define BLOCK_SIZE 4096         // 1 bloc sur disque = 4096 octets

typedef struct {
    FILE * file;                // identification du fichier
    char mode;                  // lecture 'r' ou écriture 'w'

    unsigned char record[BLOCK_SIZE]; // tampon pour lire ou écriture
    int record_length;          // nombre d'éléments dans le tampon
    int i_record;               // indice dans le tampon
    int EOF_reached;            // indique si la fin du fichier a ete atteint

    char octet[8];              // pour expension d'un octet en 8 caractères
    int i_octet;                // indice dans octet

    int nb_octets;              // nombre d'octets lus ou écrits
} Bin_file;

Bin_file* bin_open (char const*, char);
int bin_close (Bin_file *);

void bin_write (Bin_file *, char);
void str_write(Bin_file *, char*);

int bin_read (Bin_file *);
int char_read(Bin_file *);
