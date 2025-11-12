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
#define MASK_BYTE 0xAA

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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += COMPRESSION_RATIO) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t compressed = (chunk[i] & 0xF0) | ((chunk[i + 1] >> 4) & 0x0F);
            output[compressed_size++] = compressed ^ MASK_BYTE;
        } else {
            output[compressed_size++] = chunk[i] ^ MASK_BYTE;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / COMPRESSION_RATIO; i++) {
        uint8_t decoded = chunk[i] ^ MASK_BYTE;
        output[decompressed_size++] = (decoded & 0xF0) | 0x0F;
        if (decompressed_size < CHUNK_SIZE) {
            output[decompressed_size++] = ((decoded & 0x0F) << 4) | 0x0F;
        }
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t total_compressed = 0;

    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = (input_size - i < CHUNK_SIZE) ? input_size - i : CHUNK_SIZE;
        memcpy(chunk, input + i, chunk_size);
        if (chunk_size < CHUNK_SIZE) {
            memset(chunk + chunk_size, 0, CHUNK_SIZE - chunk_size);
        }

        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);

        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + total_compressed, compressed_chunk, compressed_size);
        total_compressed += compressed_size;
    }

    output->size = total_compressed;
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t total_decompressed = 0;

    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        uint8_t chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t chunk_size = (input_size - i < CHUNK_SIZE / COMPRESSION_RATIO) ? input_size - i : CHUNK_SIZE / COMPRESSION_RATIO;
        memcpy(chunk, input + i, chunk_size);
        if (chunk_size < CHUNK_SIZE / COMPRESSION_RATIO) {
            memset(chunk + chunk_size, 0, (CHUNK_SIZE / COMPRESSION_RATIO) - chunk_size);
        }

        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_size = decompress_chunk(chunk, decompressed_chunk);

        if (total_decompressed + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + total_decompressed, decompressed_chunk, decompressed_size);
        total_decompressed += decompressed_size;
    }

    output->size = total_decompressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};