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
    if (max_output_size == SIZE_MAX || max_output_size < HEADER_SIZE) {
        return 0;
    }

    *output = malloc(max_output_size);
    if (*output == NULL) {
        return 0;
    }

    CompressHeader* header = (CompressHeader*)*output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t out_pos = HEADER_SIZE;
    for (size_t i = 0; i < input_size && out_pos < max_output_size - 1; i++) {
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            size_t run_length = 2;
            while (i + run_length < input_size && input[i] == input[i + run_length] && run_length < UCHAR_MAX) {
                run_length++;
            }
            if (out_pos + 2 >= max_output_size) break;
            (*output)[out_pos++] = input[i];
            (*output)[out_pos++] = (unsigned char)run_length;
            i += run_length - 1;
        } else {
            if (out_pos >= max_output_size - 1) break;
            (*output)[out_pos++] = input[i];
        }
    }

    *output_size = out_pos;
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

    *output = malloc(header->original_size);
    if (*output == NULL) {
        return 0;
    }

    size_t in_pos = HEADER_SIZE;
    size_t out_pos = 0;
    while (in_pos < input_size && out_pos < header->original_size) {
        if (in_pos + 1 < input_size && (in_pos + 2 > input_size || input[in_pos] != input[in_pos + 1])) {
            unsigned char value = input[in_pos++];
            unsigned char count = 1;
            if (in_pos < input_size && input[in_pos] != value) {
                count = input[in_pos++];
            }
            for (unsigned char j = 0; j < count && out_pos < header->original_size; j++) {
                (*output)[out_pos++] = value;
            }
        } else {
            if (out_pos >= header->original_size) break;
            (*output)[out_pos++] = input[in_pos++];
        }
    }

    *output_size = out_pos;
    return out_pos == header->original_size;
}

int main() {
    unsigned char test_data[] = "AAAABBBCCDAA";
    size_t test_size = sizeof(test_data) - 1;

    unsigned char* compressed = NULL;
    size_t compressed_size = 0;
    if (!compress_data(test_data, test_size, &compressed, &compressed_size)) {
        printf("Compression failed\n");
        return 1;
    }

    printf("Original size: %zu, Compressed size: %zu\n", test_size, compressed_size);

    unsigned char* decompressed = NULL;
    size_t decompressed_size = 0;
    if (!decompress_data(compressed, compressed_size, &decompressed, &decompressed_size)) {
        printf("Decompression failed\n");
        free(compressed);
        return 1;
    }

    if (decompressed_size != test_size || memcmp(test_data, decompressed, test_size) != 0) {
        printf("Data mismatch after decompression\n");
    } else {
        printf("Compression and decompression successful\n");
    }

    free(compressed);
    free(decompressed);
    return 0;
}