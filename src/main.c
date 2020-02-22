#include<stdio.h>
#include<time.h>

#include"huffman.h"

void print_line() {
	printf("-------------------------\n");
}

void print_stats() {
	
}

void print_help() {
	print_line();
	printf("Exemple : ./huffman input.txt -e -o output.bin\n");
	printf("input.txt is the input file path\n");
	printf("-e is for encoding (-d to decode)\n");
	printf("-o (Optional) to change the output file path\n");
	printf("output.bin is the output file path\n");
	printf("/!\\ '\1' and '\2' can't be in the input text\n");
	print_line();
}

void interpret_args(int argc, char const *argv[]) {
	// Check help
	// get input/out
	// get mode
}

int main(int argc, char const *argv[])
{
	
	return 0;
}
