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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (compressed_size >= MAX_OUTPUT_SIZE) break;
        output[compressed_size++] = chunk[i] ^ MASK_BYTE;
    }
    return compressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed = compress_chunk(&input[i], chunk_size, &output->data[output->size]);
        if (output->size + compressed > MAX_OUTPUT_SIZE) return 0;
        output->size += compressed;
    }
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output;
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        if (sscanf(token, "%02lX", &value) != 1) {
            fprintf(stderr, "Invalid input format\n");
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Input value out of range\n");
            return EXIT_FAILURE;
        }
        input[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input provided\n");
        return EXIT_FAILURE;
    }
    
    if (!compress_data(input, input_size, &output)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed: ");
    print_hex(output.data, output.size);
    
    return EXIT_SUCCESS;
}