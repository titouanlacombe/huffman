#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BLOCK_SIZE 4096       // 1 block on the disk = 4096 bytes

typedef struct {
	FILE * file;                // file interface
	char mode;                  // 'r':reading, 'w':writing

	char buffer[BLOCK_SIZE];    // buffer to write in the file
	int buffer_i;               // index of the buffer
	char bit_buffer[CHAR_BIT];  // buffer for the convertion of 8 bits in 1 char
	int bit_buffer_i;           // index of the byte_buffer

	int file_size;              // number of bytes readed or wrote
} Bin_file;

Bin_file *bin_open(char const* path, char mode);
int bin_close(Bin_file *file);

void bin_write_bit(Bin_file *file, char bit);
void bin_write_byte(Bin_file *file, char byte);
char bin_read_bit(Bin_file *file);
char bin_read_byte(Bin_file *file);

// Writing a char in the middle of bits operations discards the bits left in the bit_buffer
// So these fonction are here to the user to be able to force the filling and emptying to avoid that
void fill_bit_buffer(Bin_file *file);
void empty_bit_buffer(Bin_file *file);

int bin_get_file_size(Bin_file *file);
char *bin_file_to_string(Bin_file *file);
