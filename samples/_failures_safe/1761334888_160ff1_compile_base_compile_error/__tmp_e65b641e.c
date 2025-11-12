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
} CompressHeader;

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
    if (max_output_size == SIZE_MAX) {
        return 0;
    }

    size_t total_size = HEADER_SIZE + max_output_size;
    if (total_size < HEADER_SIZE) {
        return 0;
    }

    unsigned char* compressed = malloc(total_size);
    if (compressed == NULL) {
        return 0;
    }

    CompressHeader* header = (CompressHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        if (out_idx >= total_size - 1) {
            free(compressed);
            return 0;
        }
        compressed[out_idx++] = input[i];
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            unsigned char count = 1;
            while (i + 1 < input_size && input[i] == input[i + 1] && count < UCHAR_MAX) {
                count++;
                i++;
            }
            if (out_idx >= total_size) {
                free(compressed);
                return 0;
            }
            compressed[out_idx++] = count;
        }
    }

    *output_size = out_idx;
    *output = compressed;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return 0;
    }

    const CompressHeader* header = (const CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    unsigned char* decompressed = malloc(header->original_size);
    if (decompressed == NULL) {
        return 0;
    }

    size_t in_idx = HEADER_SIZE;
    size_t out_idx = 0;
    while (in_idx < input_size && out_idx < header->original_size) {
        if (in_idx >= input_size) {
            free(decompressed);
            return 0;
        }
        unsigned char current = input[in_idx++];
        decompressed[out_idx++] = current;

        if (in_idx < input_size && out_idx < header->original_size) {
            if (current == input[in_idx]) {
                unsigned char count = input[in_idx + 1];
                if (count < 2 || count > UCHAR_MAX) {
                    free(decompressed);
                    return 0;
                }
                size_t remaining = header->original_size - out_idx;
                if (count - 1 > remaining) {
                    free(decompressed);
                    return 0;
                }
                for (unsigned char j = 0; j < count - 1; j++) {
                    decompressed[out_idx++] = current;
                }
                in_idx += 2;
            }
        }
    }

    if (out_idx != header->original_size) {
        free(decompressed);
        return 0;
    }

    *output_size = header->original_size;
    *output = decompressed;
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }

    if (input_size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }

    unsigned char* compressed = NULL;
    size_t compressed_size = 0;
    if (!compress_data(input_data, input_size, &compressed, &compressed_size)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    printf("Original size: %zu, Compressed size: %zu\n", input_size, compressed_size);

    unsigned char* decompressed = NULL;
    size_t decompressed