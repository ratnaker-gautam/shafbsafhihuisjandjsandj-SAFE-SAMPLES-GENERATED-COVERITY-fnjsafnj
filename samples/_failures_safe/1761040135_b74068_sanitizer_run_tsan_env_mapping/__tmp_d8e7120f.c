//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define SWAP_BYTES(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0)

static int compress_chunk(const uint8_t *input, uint8_t *output) {
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        CHECK_BOUNDS(i, CHUNK_SIZE);
        output[i] = input[i] ^ MASK_BYTE;
    }
    return EXIT_SUCCESS;
}

static int decompress_chunk(const uint8_t *input, uint8_t *output) {
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        CHECK_BOUNDS(i, CHUNK_SIZE);
        output[i] = input[i] ^ MASK_BYTE;
    }
    return EXIT_SUCCESS;
}

static int process_buffer(const Buffer *input, Buffer *output, int mode) {
    if (!input || !output) return EXIT_FAILURE;
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    size_t processed = 0;
    output->size = 0;
    
    while (processed < input->size) {
        size_t remaining = input->size - processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        uint8_t input_chunk[CHUNK_SIZE] = {0};
        uint8_t output_chunk[CHUNK_SIZE] = {0};
        
        memcpy(input_chunk, input->data + processed, chunk_size);
        
        int result;
        if (mode == 0) {
            result = compress_chunk(input_chunk, output_chunk);
        } else {
            result = decompress_chunk(input_chunk, output_chunk);
        }
        
        if (result != EXIT_SUCCESS) return EXIT_FAILURE;
        
        CHECK_BOUNDS(output->size + CHUNK_SIZE, MAX_OUTPUT_SIZE);
        memcpy(output->data + output->size, output_chunk, CHUNK_SIZE);
        output->size += CHUNK_SIZE;
        processed += chunk_size;
    }
    
    return EXIT_SUCCESS;
}

static int read_input(Buffer *buf) {
    if (!buf) return EXIT_FAILURE;
    
    printf("Enter data to process (max %zu bytes): ", MAX_INPUT_SIZE);
    
    char input[MAX_INPUT_SIZE * 2];
    if (!fgets(input, sizeof(input), stdin)) return EXIT_FAILURE;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return EXIT_FAILURE;
    if (len > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    buf->size = len;
    memcpy(buf->data, input, len);
    return EXIT_SUCCESS;
}

static void print_buffer(const Buffer *buf) {
    if (!buf) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (read_input(&input) != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes): ", input.size);
    print_buffer(&input);
    
    if (process_buffer(&input, &output, 0) != EXIT_SUCCESS) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", output.size);
    print_buffer(&output);
    
    Buffer decompressed = {0};
    if (process_buffer(&output, &decompressed, 1) != EXIT_SUCCESS) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer(&decompressed);
    
    if (input.size != decompressed.size || 
        memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing completed successfully\n");
    return EXIT_SUCCESS;
}