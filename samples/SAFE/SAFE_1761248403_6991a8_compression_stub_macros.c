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
        if (compressed_pos >= max_output_size - 2) {
            free(*output);
            *output = NULL;
            return 0;
        }

        unsigned char current = input[i];
        unsigned char count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < UCHAR_MAX) {
            count++;
            i++;
        }

        (*output)[compressed_pos++] = current;
        (*output)[compressed_pos++] = count;
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

    size_t decompressed_pos = 0;
    size_t read_pos = HEADER_SIZE;

    while (read_pos < input_size && decompressed_pos < header->original_size) {
        if (read_pos + 1 >= input_size) {
            free(*output);
            *output = NULL;
            return 0;
        }

        unsigned char value = input[read_pos++];
        unsigned char count = input[read_pos++];

        if (decompressed_pos + count > header->original_size) {
            free(*output);
            *output = NULL;
            return 0;
        }

        for (unsigned char j = 0; j < count; j++) {
            (*output)[decompressed_pos++] = value;
        }
    }

    if (decompressed_pos != header->original_size) {
        free(*output);
        *output = NULL;
        return 0;
    }

    *output_size = header->original_size;
    return 1;
}

int main(void) {
    unsigned char test_data[] = "AAAABBBCCDAA";
    size_t test_size = sizeof(test_data) - 1;

    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%c", test_data[i]);
    }
    printf(" (%zu bytes)\n", test_size);

    unsigned char* compressed = NULL;
    size_t compressed_size = 0;

    if (compress_data(test_data, test_size, &compressed, &compressed_size)) {
        printf("Compressed to %zu bytes\n", compressed_size);
        
        unsigned char* decompressed = NULL;
        size_t decompressed_size = 0;
        
        if (decompress_data(compressed, compressed_size, &decompressed, &decompressed_size)) {
            printf("Decompressed data: ");
            for (size_t i = 0; i < decompressed_size; i++) {
                printf("%c", decompressed[i]);
            }
            printf(" (%zu bytes)\n", decompressed_size);
            
            if (decompressed_size == test_size && memcmp(test_data, decompressed, test_size) == 0) {
                printf("Compression/decompression successful!\n");
            } else {
                printf("Data mismatch!\n");
            }
            free(decompressed);
        } else {
            printf("Decompression failed!\n");
        }
        free(compressed);
    } else {
        printf("Compression failed!\n");
    }

    return 0;
}