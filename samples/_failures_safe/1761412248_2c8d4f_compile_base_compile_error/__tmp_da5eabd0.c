//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    for (size_t i = 0; i < chunk_size; i += 2) {
        uint8_t first = (i < chunk_size) ? chunk[i] : 0;
        uint8_t second = (i + 1 < chunk_size) ? chunk[i + 1] : 0;
        output[i / 2] = (first & 0xF0) | ((second & 0xF0) >> 4);
    }
}

static size_t compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    size_t compressed_size = 0;
    size_t remaining = input_size;

    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        if (compressed_size + (chunk_size / COMPRESSION_RATIO) >= MAX_OUTPUT_SIZE) break;

        compress_chunk(input, chunk_size, &output->data[compressed_size]);
        compressed_size += chunk_size / COMPRESSION_RATIO;
        input += chunk_size;
        remaining -= chunk_size;
    }

    output->size = compressed_size;
    return compressed_size;
}

static void decompress_chunk(const uint8_t* compressed, size_t comp_size, uint8_t* output) {
    for (size_t i = 0; i < comp_size; i++) {
        uint8_t byte = compressed[i];
        output[i * 2] = byte & 0xF0;
        output[i * 2 + 1] = (byte & 0x0F) << 4;
    }
}

static size_t decompress_data(const uint8_t* compressed, size_t comp_size, Buffer* output) {
    if (!validate_input(compressed, comp_size)) return 0;
    if (output == NULL) return 0;

    size_t decompressed_size = comp_size * COMPRESSION_RATIO;
    if (decompressed_size > MAX_OUTPUT_SIZE) return 0;

    decompress_chunk(compressed, comp_size, output->data);
    output->size = decompressed_size;
    return decompressed_size;
}

static void print_buffer(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

static int read_input(Buffer* input) {
    if (input == NULL) return 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) return 0;
    if (size <= 0 || size > MAX_INPUT_SIZE) return 0;

    printf("Enter %d bytes as hex values: ", size);
    for (int i = 0; i < size; i++) {
        unsigned int byte;
        if (scanf("%02x", &byte) != 1) return 0;
        if (byte > BYTE_MASK) return 0;
        input->data[i] = (uint8_t)byte;
    }
    input->size = size;
    return 1;
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed_data = {0};
    Buffer decompressed_data = {0};

    printf("Compression Demo\n");
    if (!read_input(&input_data)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_buffer(&input_data);

    size_t comp_size = compress_data(input_data.data, input_data.size, &compressed_data);
    if (comp_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("\nCompressed data (%zu bytes):\n", compressed_data.size);
    print_buffer(&compressed_data);

    size_t decomp_size = decompress_data(compressed_data.data, compressed_data.size, &decompressed_data);
    if (decomp_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }

    printf("\nDecompressed data (%zu bytes):\n", decompressed_data.size