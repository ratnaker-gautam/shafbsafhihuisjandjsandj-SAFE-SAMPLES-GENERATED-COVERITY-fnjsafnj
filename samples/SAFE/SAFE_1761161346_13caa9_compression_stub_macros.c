//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static size_t safe_read_input(char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0 || max_size > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read++] = (char)c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

static void naive_compress(const char *input, size_t input_len, char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = sizeof(comp_header_t);
    comp_header_t *header = (comp_header_t *)output;
    header->magic = MAGIC_HEADER;
    header->original_size = (uint32_t)input_len;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (out_idx >= MAX_OUTPUT_SIZE - 1) {
            break;
        }
        output[out_idx++] = input[i];
    }
    header->compressed_size = (uint32_t)(out_idx - sizeof(comp_header_t));
    *output_len = out_idx;
}

static void naive_decompress(const char *input, size_t input_len, char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len < sizeof(comp_header_t)) {
        *output_len = 0;
        return;
    }
    const comp_header_t *header = (const comp_header_t *)input;
    if (header->magic != MAGIC_HEADER || header->original_size > MAX_INPUT_SIZE || header->compressed_size > MAX_OUTPUT_SIZE) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < header->compressed_size; i++) {
        if (out_idx >= header->original_size) {
            break;
        }
        output[out_idx] = input[sizeof(comp_header_t) + i];
        for (size_t j = 1; j < COMPRESSION_RATIO && out_idx < header->original_size - 1; j++) {
            output[out_idx + j] = output[out_idx];
        }
        out_idx += COMPRESSION_RATIO;
    }
    *output_len = header->original_size;
}

static void print_hex(const char *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        printf("%02X", (unsigned char)data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    char output_buffer[MAX_OUTPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    size_t input_len = safe_read_input(input_buffer, sizeof(input_buffer));
    if (input_len == 0) {
        printf("No input received.\n");
        return EXIT_FAILURE;
    }
    size_t compressed_len;
    naive_compress(input_buffer, input_len, output_buffer, &compressed_len);
    if (compressed_len == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    printf("Compressed data (hex): ");
    print_hex(output_buffer, compressed_len);
    char decompressed_buffer[MAX_INPUT_SIZE];
    size_t decompressed_len;
    naive_decompress(output_buffer, compressed_len, decompressed_buffer, &decompressed_len);
    if (decompressed_len == 0 || decompressed_len != input_len) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Data mismatch after decompression.\n");
        return EXIT_FAILURE;
    }
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed_len; i++) {
        putchar(decompressed_buffer[i]);
    }
    printf("\n");
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed_len);
    return EXIT_SUCCESS;
}