// huffman.c
#include <time.h>
#include "huffman.h"

// Fonction erreur si fichier non trouvé
void raise_open_error(char const *file_path) {
    printf("Error : can't open '%s'\n", file_path);
    exit(1);
}


//Fonction ergonomique affiche une aide
int check_help(int argc, char const *argv[]) {
    int i;

    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-help") == 0)
        {
            return 1;
        }
    }
    return 0;
}

void print_help() {
    printf("------------------------HELP---------------------\n");
    printf("Exemple : ./huffman input.txt -e -o output.bin\n");
    printf("input.txt is the input file path\n");
    printf("-e is for encoding (-d to decode)\n");
    printf("-o (Optional) to change the output file path\n");
    printf("output.bin is the output file path\n");
    printf("/!\\ '\1' and '\2' can't be in the input text\n");
    printf("-------------------------------------------------\n");
}

//Recupere les argument d'entrés
char const* get_input(int argc, char const *argv[]) {
    char const *input;
    
    if (argc < 2) {
        raise_error("no input file specified");
    }
    
    input = argv[1];
    return input;
}


char const* get_output(int argc, char const *argv[], char mode) {
    char const *output;
    int i;
    
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (argc < i + 2) {
                raise_error("output syntax invalid");
            } else {
                output = argv[i + 1];
                return output;
            }
        }
    }
    if (mode == 'e')
    {
        // output par default si encode
        output = "encoded_text.bin";
    }
    else
    {
        // output par default si decode
        output = "decoded_text.txt";
    }
    return output;
}

//Permet de choisir l'encodage ou le decodage
char get_mode(int argc, char const *argv[]) {
    int i;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            return 'e';
        } else if (strcmp(argv[i], "-d") == 0) {
            return 'd';
        }
    }

    raise_error("no mode specified");
}

//recupere notre texte d'entré
char* get_text(FILE *file) {
    char text[100000];
    int c;

    int i=0;
    while ((c = fgetc(file)) != EOF) {
        text[i] = (char)c;
        i++;
    }

    text[i] = '\0';
    return text;
}

char* get_bin(Bin_file *file) {
     char text[100000];
    int c;

    int i=0;
    while ((c = bin_read(file)) != EOF) {
        text[i] = (char)c;
        i++;
    }

    
    text[i] = '\0';
    //printf("%s", text);
    return text;
}

float encode(char const *input_path, char const *output_path) {
    char *raw_text, *encoded_text, *serial_huffman;
    arbre arbre_huffman;
    float comp_rate;
    int i;

    FILE *input;
    Bin_file *output;

    //open files
    input = fopen(input_path, "r");
    if (input == NULL)
    {   
        raise_open_error(input_path);
    }
    output = bin_open(output_path, 'w');
    if (output == NULL)
    {
        // erreur theoriquement impossible car "w" open tout le temps
        raise_open_error(output_path);
    }

    raw_text = get_text(input);         // recuperer le chaine de caracteres du texte input
    arbre_huffman = huffman(raw_text);  // algorithme huffman
    encoded_text = encode_text(arbre_huffman, raw_text);    // encodage du texte avec l'arbre
    serial_huffman = serializer_arbre(arbre_huffman);         //serialisation de l'arbre

    // save output
    str_write(output, serial_huffman);          //ecrire serial_huffman dans output
    str_write(output, "\2");          //ecrire separateur
    for (i = 0; i < strlen(encoded_text); i++)
    {
        bin_write(output, encoded_text[i]);         //ecrire texte
    }
    
    // closing files
    fclose(input);
    bin_close(output);

    // returning comp_rate for stats
    comp_rate = (float)strlen(encoded_text) / (8 * (float)strlen(raw_text));
    comp_rate *= 100;
    return comp_rate;
}

void decode(char const *input_path, char const *output_path) {
    char *raw_text, *encoded_text, serial_huffman[10000];
    int c;
    arbre arbre_huffman;

    Bin_file *input;
    FILE *output;

    //open files
    input = bin_open(input_path, 'r');
    if (input == NULL)
    {
        raise_open_error(input_path);
    }
    output = fopen(output_path, "w");
    if (output == NULL)
    {
        // erreur theoriquement impossible car "w" open tout le temps
        raise_open_error(output_path);
    }
    
    // read char until '\2' (separator) is found
    int i = 0;
    while ((c = char_read(input)) != '\2') {
        serial_huffman[i] = (char)c;
        i++;
    }
    serial_huffman[i] = '\0';
    // read encoded_text
    encoded_text = get_bin(input);

    // deserialiser arbre
    arbre_huffman = deserializer_arbre(serial_huffman);
    raw_text = decode_text(arbre_huffman, encoded_text);

    // save result
    fprintf(output, "%s", raw_text);

    // close files
    bin_close(input);
    fclose(output);
}

int main(int argc, char const *argv[]) {
    char const *input_path, *output_path;
    char mode;

    clock_t t0, t1;
    double total_t;

    //check if need help
    if(check_help(argc, argv)) {
        print_help();
        exit(0);
    }
    
    // get io
    mode = get_mode(argc, argv);
    input_path = get_input(argc, argv);
    output_path = get_output(argc, argv, mode);

    // encode or decode
    if (mode == 'e')
    {
        printf("Input : %s\n", input_path);
        printf("Encoding...\n");

        t0 = clock();
        float comp_rate = encode(input_path, output_path);
        t1 = clock();
        printf("Done !\n");

        total_t = (double)(t1 - t0) / CLOCKS_PER_SEC;
        // print stats
        printf("Output : %s\n", output_path);
        printf("Compresion rate : %.2f %%\n", comp_rate);
        printf("Encoding time : %.lf ms\n", total_t * 1000);
    }
    else
    {
        printf("Input : %s\n", input_path);
        printf("Decoding...\n");
        
        t0 = clock();
        decode(input_path, output_path);
        t1 = clock();
        printf("Done !\n");

        total_t = (double)(t1 - t0) / CLOCKS_PER_SEC;
        // print stats
        printf("Output : %s\n", output_path);
        printf("Decoding time : %.lf ms\n", total_t * 1000);
    }

    // end
    return 0;
}