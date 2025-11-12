//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} file_header_t;

static size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

static int validate_buffer(const unsigned char *buf, size_t size) {
    return buf != NULL && size > 0 && size <= MAX_INPUT_SIZE;
}

static size_t simple_compress(const unsigned char *input, size_t input_size, unsigned char *output) {
    if (!validate_buffer(input, input_size) || output == NULL) {
        return 0;
    }

    file_header_t *header = (file_header_t *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t output_index = HEADER_SIZE;
    size_t input_index = 0;

    while (input_index < input_size) {
        unsigned char current = input[input_index];
        size_t count = 1;

        while (input_index + count < input_size && count < UCHAR_MAX && 
               input[input_index + count] == current) {
            count++;
        }

        if (count >= 3 || current == 0xFF) {
            if (output_index + 2 > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
                return 0;
            }
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (unsigned char)count;
        } else {
            if (output_index + count > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
                return 0;
            }
            for (size_t i = 0; i < count; i++) {
                output[output_index++] = input[input_index + i];
            }
        }
        input_index += count;
    }

    return output_index;
}

static size_t simple_decompress(const unsigned char *input, size_t input_size, unsigned char *output) {
    if (!validate_buffer(input, input_size) || output == NULL) {
        return 0;
    }

    if (input_size < HEADER_SIZE) {
        return 0;
    }

    file_header_t *header = (file_header_t *)input;
    if (header->magic != MAGIC_NUMBER || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t output_index = 0;
    size_t input_index = HEADER_SIZE;

    while (input_index < input_size && output_index < header->original_size) {
        unsigned char current = input[input_index++];

        if (current == 0xFF && input_index + 1 < input_size) {
            unsigned char value = input[input_index++];
            unsigned char count = input[input_index++];

            if (output_index + count > header->original_size) {
                return 0;
            }

            for (unsigned char i = 0; i < count; i++) {
                output[output_index++] = value;
            }
        } else {
            if (output_index >= header->original_size) {
                return 0;
            }
            output[output_index++] = current;
        }
    }

    return output_index == header->original_size ? output_index : 0;
}

static void print_hex(const unsigned char *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    unsigned char compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    unsigned char decompressed_buffer[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    size_t input_size = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        
        char *pos = line;
        while (*pos != '\0' && input_size < MAX_INPUT_SIZE) {
            if (pos[0] == '\n') {
                break;
            }
            if (pos[1] == '\0') {
                break;
            }
            
            unsigned int byte;
            if (sscanf(pos, "%2x", &byte) == 1) {
                input_buffer[input_size++] = (unsigned char)byte;
                pos += 2;
            } else {
                pos++;
            }
            
            while (*pos == ' ') {
                pos++;
            }
        }
    }

    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }

    printf("\