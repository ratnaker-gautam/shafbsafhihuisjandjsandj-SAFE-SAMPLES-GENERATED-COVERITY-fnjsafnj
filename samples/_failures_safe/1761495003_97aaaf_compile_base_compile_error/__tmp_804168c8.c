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
} CompressionHeader;

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

    CompressionHeader* header = (CompressionHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t compressed_size = 0;
    for (size_t i = 0; i < input_size && compressed_size < max_output_size; i++) {
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            size_t count = 1;
            while (i + count < input_size && input[i] == input[i + count] && count < UCHAR_MAX) {
                count++;
            }
            if (compressed_size + 2 > max_output_size) {
                break;
            }
            compressed[HEADER_SIZE + compressed_size] = (unsigned char)count;
            compressed[HEADER_SIZE + compressed_size + 1] = input[i];
            compressed_size += 2;
            i += count - 1;
        } else {
            if (compressed_size + 1 > max_output_size) {
                break;
            }
            compressed[HEADER_SIZE + compressed_size] = input[i];
            compressed_size++;
        }
    }

    *output = compressed;
    *output_size = HEADER_SIZE + compressed_size;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return 0;
    }

    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    unsigned char* decompressed = malloc(header->original_size);
    if (decompressed == NULL) {
        return 0;
    }

    size_t decompressed_pos = 0;
    size_t compressed_pos = HEADER_SIZE;

    while (compressed_pos < input_size && decompressed_pos < header->original_size) {
        if (compressed_pos + 1 < input_size && input[compressed_pos] > 1) {
            unsigned char count = input[compressed_pos];
            unsigned char value = input[compressed_pos + 1];
            if (decompressed_pos + count > header->original_size) {
                free(decompressed);
                return 0;
            }
            for (unsigned char i = 0; i < count; i++) {
                decompressed[decompressed_pos++] = value;
            }
            compressed_pos += 2;
        } else {
            decompressed[decompressed_pos++] = input[compressed_pos++];
        }
    }

    if (decompressed_pos != header->original_size) {
        free(decompressed);
        return 0;
    }

    *output = decompressed;
    *output_size = header->original_size;
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }

    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }

    unsigned char* compressed = NULL;
    size_t compressed_size = 0;
    if (!compress_data(input_data, input_size, &compressed, &compressed_size)) {
        printf("Compression failed\n");
        return 1;
    }

    printf("Original size: %zu, Compressed size: %zu\n", input_size, compressed_size);

    unsigned char* decompressed = NULL;
    size_t decompressed_size = 0;
    if (!decompress_data(compressed, compressed