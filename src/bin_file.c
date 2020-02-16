#include "bin_file.h"

Bin_file *bin_open(char const* path, char mode) {
	Bin_file *file;

	file = malloc(sizeof(Bin_file));

	file->file = fopen(path, &mode);
	file->mode = mode;
	if (mode == 'r') {
		file->buffer_i = BLOCK_SIZE;
		file->bit_buffer_i = 8;
	}
	else {
		file->buffer_i = 0;
		file->bit_buffer_i = 0;
	}
	file->file_size = 0;

	return file;
}

void empty_buffer(Bin_file *file) {
	int c;
	
	file->buffer_i = 0;
	while (file->buffer_i < BLOCK_SIZE)
	{
		c = file->buffer[file->buffer_i];
		fputc(c, file->file);
		file->buffer_i++;
	}
	file->buffer_i = 0;
}

void fill_buffer(Bin_file *file) {
	int c;

	file->buffer_i = 0;
	while (file->buffer_i < BLOCK_SIZE && (c = fgetc(file->file)) != EOF) {
		file->buffer[file->buffer_i] = (char)c;
		file->buffer_i++;
	}

	// Detection of the EOF
	if (c == EOF) {
		file->buffer[file->buffer_i] = '\0';
	}
	file->buffer_i = 0;
}

char bin_read_char(Bin_file *file) {
	char c;
	
	if (file->buffer_i >= BLOCK_SIZE) {
		fill_buffer(file);
	}
	c = file->buffer[file->buffer_i];
	file->buffer_i++;
	file->file_size++;
	return c;
}

void bin_write_char(Bin_file *file, char byte) {
	if (file->buffer_i >= BLOCK_SIZE)
	{
		empty_buffer(file);
	}
	file->buffer[file->buffer_i] = byte;
	file->buffer_i++;
	file->file_size++;
}

// Fill the bit_buffer with the bit representation of the next char in buffer
void fill_bit_buffer(Bin_file *file) {
	unsigned char sliding_bit, c;
	int i;

	c = bin_read_char(file);
	// sliding_bit = 1000 0000
	sliding_bit = 0x80;
	for (i = 0; i < 8; i++) {
		if ((c | sliding_bit) == c) {
			file->bit_buffer[i] = '1';
		}
		else {
			file->bit_buffer[i] = '0';
		}
		// Offset the bits to the right (division by 2)
		sliding_bit = sliding_bit>>1;
	}
	file->bit_buffer_i = 0;
}

// Empty the bit_buffer and put the char representation of the bits in buffer
void empty_bit_buffer(Bin_file *file) {
	unsigned char byte, sliding_bit;
	int i;

	byte = 0;
	// sliding_bit = 1000 0000
	sliding_bit = 0x80;
	for (i = 0; i < 8; i++) {
		if (file->bit_buffer[i] == '1') {
			// binary OR
			byte = byte|sliding_bit;
		}
		// Offset the bits to the right (division by 2)
		sliding_bit = sliding_bit>>1;
	}
	file->bit_buffer_i = 0;
	bin_write_char(file, byte);
}

char bin_read_bin(Bin_file *file) {
	if (file->bit_buffer_i >= 8) {
		fill_bit_buffer(file);
	}
	char c = file->bit_buffer[file->bit_buffer_i];
	file->bit_buffer_i++;
	return c;
}
void bin_write_bin(Bin_file *file, char bit) {
	if (file->bit_buffer_i >= 8) {
		empty_bit_buffer(file);
	}
	file->bit_buffer[file->bit_buffer_i] = bit;
	file->bit_buffer_i++;
}

int bin_close(Bin_file *file) {
	if (file->mode == 'w') {
		// if bits not written
		if (file->bit_buffer_i != 0) {
			empty_bit_buffer(file);
		}

		// if bytes not written
		if (file->buffer_i != 0) {
			empty_buffer(file);
		}
	}
	fclose(file->file);
	int file_size = file->file_size;
	free(file);
	return file_size;
}

int bin_get_file_size(Bin_file *file) {
	return file->file_size;
}

// Return a random int a <= x < b
int randint(int a, int b)
{
  return(a + ((b - a) * (double)rand() / RAND_MAX));
}

// test for block_size input
// ATTENTION : é : multi-char, maybe cause problems with huffman
#define LTEST 2*BLOCK_SIZE
int main(int argc, char const *argv[])
{
	Bin_file *file;
	char c, in_char[LTEST], out_char[LTEST+1], in_bin[LTEST], out_bin[LTEST+1];
	int  i, errors;
	srand(1);
	// ---------------------------Bin write
	file = bin_open("texts/test_bin.bin", 'w');
	for (i = 0; i < LTEST; i++) {
		c = '0'+randint(0, 2);
		in_bin[i] = c;
		bin_write_bin(file, c);
	}
	// printf("input bin:  %s\n", in_bin);
	printf("File size: %i\n", bin_close(file));
	// ---------------------------Bin read
	file = bin_open("texts/test_bin.bin", 'r');
	for (i = 0; i < LTEST; i++) {
		out_bin[i] = bin_read_bin(file);
	}
	out_bin[i] = '\0';
	// printf("output bin: %s\n", out_bin);
	printf("File size: %i\n", bin_close(file));
	// ---------------------------Calc errors
	errors = 0;
	for (i = 0; i < LTEST; i++) {
		errors += abs(out_bin[i] - in_bin[i]);
	}
	printf("Bin Errors: %i\n", errors);

	// ---------------------------Char write
	file = bin_open("texts/test_char.txt", 'w');
	for (i = 0; i < LTEST; i++) {
		c = (char)randint(1, 256);
		in_char[i] = c;
		bin_write_char(file, c);
	}
	// printf("input str:  %s\n", in_char);
	printf("File size: %i\n", bin_close(file));
	// ---------------------------Char read
	file = bin_open("texts/test_char.txt", 'r');
	i = 0;
	while ((c = bin_read_char(file)) != '\0') {
		out_char[i] = c;
		i++;
	}
	out_char[i] = '\0';
	// printf("output str: %s\n", out_char);
	printf("File size: %i\n", bin_close(file)-1); // -1 because of the \0 readed (not in file)
	// ---------------------------Calc errors
	errors = 0;
	i = 0;
	while (out_char[i] != '\0') {
		errors += abs(out_char[i] - in_char[i]);
		i++;
	}
	printf("Char Errors: %i\n", errors);
	
	return 0;
}