
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