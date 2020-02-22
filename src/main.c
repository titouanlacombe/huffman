#include<stdio.h>
#include<time.h>
#include<string.h>

#include"huffman.h"

#define DEFAULT_OUT_E "huffman_encoded.bin"
#define DEFAULT_OUT_D "huffman_decoded.txt"

// print some beautifull ascii art
void print_title(char *title) {
	printf("------------------%s------------------\n", title);
}

// print the call arguments
void print_args(char *input_path, char *output_path, char mode) {
	if (mode == 'e') {
		print_title("Encoding");
	}
	else {
		print_title("Decoding");
	}
	printf("Input: %s\n", input_path);
	printf("Output: %s\n", output_path);
	printf("Processing...\n");
}

// print the stats pannel
void print_stats(clock_t t1, int input_size, int output_size, char mode) {
	printf("Done!\n");
	print_title("--Stats-");
	if (mode == 'e') {
		printf("Encoding time: %.0lf ms\n", 1000*(double)t1/CLOCKS_PER_SEC);
	} else {
		printf("Decoding time: %.0lf ms\n", 1000*(double)t1/CLOCKS_PER_SEC);
	}
	printf("Input size: %i bytes\n", input_size);
	printf("Output size: %i bytes\n", output_size);
	if (mode == 'e') {
		printf("Compression rate: %.1lf %%\n", 100*((double)output_size/input_size));
	}
}

// print the version pannel
void print_version() {
	printf("Huffman compression By Titouan Lacombe\n");
	printf("Version 1.0\n");
	printf("Use -h for help\n");
}

// print the help pannel
void print_help() {
	print_title("Help");
	printf("Usage : ./huffman input\n");
	printf("Arguments (optional):\n");
	printf("-o : To specifie an output\n");
	printf("-m : To specifie a mode (e: encoding, d: decoding)\n");
	printf("Default values:\n");
	printf("mode: decoding if input is in .bin, encoding else\n");
	printf("encoding output: %s\n", DEFAULT_OUT_E);
	printf("decoding output: %s\n", DEFAULT_OUT_D);
}

// return the file extension in filenames
char *get_file_extension(char *input) {
	for (int i = 0; i < strlen(input)-1; i++) {
		if (input[i] == '.') {
			return &input[i+1];
		}
	}
	return "";
}

// Interpret the arguments given by the user
// Error codes:
// -1: print help asked
// -2: no args
int interpret_args(int argc, char const *argv[], char input_path[], char output_path[], char *mode) {
	int i;

	// --Check help--
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			return -1;
		}
	}
	
	// --Get input--
	if (argc < 2) {
		// if no input
		return -2;
	}
	else {
		// copy input
		strcpy(input_path, argv[1]);
	}
	
	// --Get mode--
	// Automatic
	*mode = 'e';
	if (strcmp(get_file_extension(input_path), "bin") == 0) {
		*mode = 'd';
	}
	// Manual
	for (i = 2; i < argc-1; i++) {
		if (strcmp(argv[i], "-m") == 0) {
			*mode = *argv[i+1];
		}
	}
	
	// --Get output--
	if (*mode == 'e') {
		strcpy(output_path, DEFAULT_OUT_E);
	}
	else {
		strcpy(output_path, DEFAULT_OUT_D);
	}
	
	for (i = 2; i < argc-1; i++) {
		if (strcmp(argv[i], "-o") == 0) {
			// copy output
			strcpy(output_path, argv[i+1]);
		}
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	char input_path[100], output_path[100], mode;
	int code, input_size, output_size;
	double time;
	clock_t t0, t1;
	
	code = interpret_args(argc, argv, input_path, output_path, &mode);
	if (code == 0) {
		print_args(input_path, output_path, mode);
		t0 = clock();
		if (mode == 'e') {
			code = huff_encode(input_path, output_path, &input_size, &output_size);
		}
		else {
			code = huff_decode(input_path, output_path, &input_size, &output_size);
		}
		t1 = clock() - t0;
		if (code == 0) {
			print_stats(t1, input_size, output_size, mode);
		}
		else if (code == -1) {
			printf("Error: unable to open '%s'\n", input_path);
		}
		else if (code == -2) {
			printf("Error: unable to open '%s'\n", output_path);
		}
	}
	else if (code == -1) {
		print_help();
	}
	else if (code == -2) {
		print_version();
	}

	return 0;
}
