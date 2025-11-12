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
} CompressedHeader;

size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

int compress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t max_output_size = safe_multiply(input_size, COMPRESSED_MULTIPLIER);
    if (max_output_size == SIZE_MAX || max_output_size < HEADER_SIZE) {
        return 0;
    }

    *output = malloc(max_output_size);
    if (*output == NULL) {
        return 0;
    }

    CompressedHeader* header = (CompressedHeader*)*output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t compressed_pos = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        if (compressed_pos >= max_output_size - 1) {
            free(*output);
            *output = NULL;
            return 0;
        }
        (*output)[compressed_pos++] = input[i];
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            unsigned char count = 1;
            while (i + 1 < input_size && input[i] == input[i + 1] && count < UCHAR_MAX) {
                count++;
                i++;
            }
            if (compressed_pos >= max_output_size - 1) {
                free(*output);
                *output = NULL;
                return 0;
            }
            (*output)[compressed_pos++] = count;
        }
    }

    *output_size = compressed_pos;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return 0;
    }

    CompressedHeader* header = (CompressedHeader*)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    *output = malloc(header->original_size);
    if (*output == NULL) {
        return 0;
    }

    size_t input_pos = HEADER_SIZE;
    size_t output_pos = 0;
    
    while (input_pos < input_size && output_pos < header->original_size) {
        if (input_pos >= input_size) {
            free(*output);
            *output = NULL;
            return 0;
        }
        
        unsigned char current = input[input_pos++];
        (*output)[output_pos++] = current;
        
        if (input_pos < input_size && output_pos < header->original_size) {
            if (current == input[input_pos]) {
                unsigned char count = input[input_pos++];
                if (count < 2) {
                    free(*output);
                    *output = NULL;
                    return 0;
                }
                for (unsigned char j = 1; j < count && output_pos < header->original_size; j++) {
                    (*output)[output_pos++] = current;
                }
            }
        }
    }

    if (output_pos != header->original_size) {
        free(*output);
        *output = NULL;
        return 0;
    }

    *output_size = header->original_size;
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (unsigned char)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    unsigned char* compressed_data = NULL;
    size_t compressed_size = 0;
    
    if (!compress_data(input_data, input_size, &compressed_data, &compressed_size)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_size) * 100.0);
    
    unsigned char* decompressed_data = NULL;
    size_t decompressed_size = 0;
    
    if (!decompress_data(compressed_data, compressed_size, &decompressed_data, &decompressed_size)) {
        printf("Decompression failed.\n");
        free(compressed