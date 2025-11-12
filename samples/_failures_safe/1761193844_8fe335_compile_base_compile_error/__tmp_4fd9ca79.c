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
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        if (!VALID_CHAR(data[i])) {
            return 0;
        }
    }
    return 1;
}

static size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size > CHUNK_SIZE) {
        input_size = CHUNK_SIZE;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (i + 1 < input_size) {
            uint8_t compressed = (input[i] & 0xF0) | ((input[i + 1] >> 4) & 0x0F);
            output[output_size++] = compressed;
        } else {
            output[output_size++] = input[i];
        }
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t current = input[i];
        if (current & 0x0F) {
            output[output_size++] = (current & 0xF0) | ((current & 0x0F) << 4);
            if (output_size < MAX_OUTPUT_SIZE) {
                output[output_size++] = (current & 0x0F) | 0x30;
            }
        } else {
            output[output_size++] = current;
        }
        if (output_size >= MAX_OUTPUT_SIZE) {
            break;
        }
    }
    return output_size;
}

static int perform_compression(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i < CHUNK_SIZE) ? input->size - i : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input->data + i, chunk_size, output->data + output->size);
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        output->size += compressed_size;
    }
    return 1;
}

static int perform_decompression(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i < CHUNK_SIZE) ? input->size - i : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(input->data + i, chunk_size, output->data + output->size);
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer(const Buffer *buf) {
    if (buf == NULL) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input.size < MAX_INPUT_SIZE) {
        if (VALID_CHAR(c)) {
            input.data[input.size++] = (uint8_t)c;
        }
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No valid input provided\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(input.data, input.size)) {
        fprintf(stderr, "Error: Invalid input data\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    print_buffer(&input);
    
    if (!perform_compression(&input, &compressed)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed: ");
    print_buffer(&compressed);
    
    if (!perform_decompression(&compressed, &decompressed)) {
        fprintf(stderr, "Error: Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("