#include "bin_file.h"

void fill_buffer(Bin_file *file) {
	int c;

	file->buffer_i = 0;
	while (file->buffer_i < BLOCK_SIZE && (c = fgetc(file->file)) != EOF) {
		file->buffer[file->buffer_i] = (unsigned char)c;
		file->buffer_i++;
	}

	if (c == EOF) {
		file->buffer[file->buffer_i] = '\0';
		file->buffer_i++;
	}
	file->file_size += file->buffer_i;
	file->buffer_i = 0;
}

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
	while (file->buffer_i < BLOCK_SIZE && file->buffer[file->buffer_i] != '\0')
	{
		c = file->buffer[file->buffer_i];
		fputc(c, file->file);
		file->buffer_i++;
	}
}

unsigned char bin_read_char(Bin_file *file) {
	unsigned char c;
	
	if (file->buffer_i >= BLOCK_SIZE) {
		fill_buffer(file);
	}
	c = file->buffer[file->buffer_i];
	if (c != '\0') {
		file->buffer_i++;
	}
	return c;
}

void bin_write_char(Bin_file *file, unsigned char byte) {
	if (file->buffer_i >= BLOCK_SIZE)
	{
		empty_buffer(file);
	}
	file->buffer[file->buffer_i] = byte;
	file->buffer_i++;
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
	if (file->mode = 'w') {
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

// get all text and put it in a char*
unsigned char* file_to_string(Bin_file *file) {
	unsigned char *text;
	int c, text_i, file_size;

	file_size = BLOCK_SIZE*sizeof(char);
	text = malloc(file_size);
	text_i = 0;
	while ((c = fgetc(file->file)) != EOF) {
		text[text_i] = (unsigned char)c;
		text_i++;
		if (text_i >= file_size)
		{
			file_size += BLOCK_SIZE*sizeof(char);
			text = realloc(text, file_size);
		}
	}

	if (text_i >= file_size)
	{
		file_size += sizeof(char);
		text = realloc(text, file_size);
	}
	text[text_i] = '\0';
	text_i++;
	file->file_size = text_i;
	return text;
}

int main(int argc, char const *argv[])
{
	Bin_file *file;
	char *test_bin = "01101001110101101010";
	unsigned char *test_char = "1234azerty&é(-è)";
	int i, len_bin = strlen(test_bin), len_char = strlen(test_char);
	// Bin write
	file = bin_open("texts/test_bin.bin", 'w');
	printf("input:  ");
	for (i = 0; i < len_bin; i++) {
		bin_write_bin(file, test_bin[i]);
		printf("%c", test_bin[i]);
	}
	bin_close(file);
	printf("\n");

	// Bin read
	file = bin_open("texts/test_bin.bin", 'r');
	printf("output: ");
	for (i = 0; i < len_bin; i++) {
		printf("%c", bin_read_bin(file));
	}
	bin_close(file);
	printf("\n");

	// Char write
	file = bin_open("texts/test_char.txt", 'w');
	printf("input:  ");
	for (i = 0; i < len_char; i++) {
		bin_write_char(file, test_char[i]);
		printf("%c|", test_char[i]);
	}
	bin_close(file);
	printf("\n");
	// Char read
	file = bin_open("texts/test_char.txt", 'r');
	printf("output: ");
	for (i = 0; i < len_char; i++) {
		printf("%c|", bin_read_char(file));
	}
	bin_close(file);
	printf("\n");

	return 0;
}