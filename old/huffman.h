void raise_error(char *s) {
    printf("Error : %s\n", s);
    printf("For more info use ./huffman -help\n");
    exit(1);
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
