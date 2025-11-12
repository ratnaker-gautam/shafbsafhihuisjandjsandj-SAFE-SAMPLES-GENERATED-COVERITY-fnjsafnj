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
#define SHIFT_AMOUNT 4

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
    if (chunk == NULL || output == NULL) return 0;
    
    uint16_t compressed = 0;
    for (int i = 0; i < CHUNK_SIZE; i++) {
        uint8_t val = chunk[i] >> SHIFT_AMOUNT;
        compressed = (compressed << 2) | (val & 0x3);
    }
    
    output[0] = (compressed >> 8) & BYTE_MASK;
    output[1] = compressed & BYTE_MASK;
    return 2;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        memcpy(chunk, current, chunk_size);
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(chunk, &output->data[output->size]);
        if (compressed_size == 0) return 0;
        
        output->size += compressed_size;
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter data to compress (up to %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    size_t total_size = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos != '\0' && total_size < max_size) {
            while (*pos == ' ' || *pos == '\t') pos++;
            
            if (*pos == '\0' || *pos == '\n') break;
            
            if (!((*pos >= '0' && *pos <= '9') || 
                  (*pos >= 'A' && *pos <= 'F') || 
                  (*pos >= 'a' && *pos <= 'f'))) {
                return 0;
            }
            
            char hex[3] = {0};
            hex[0] = *pos++;
            if (*pos != '\0' && *pos != ' ' && *pos != '\t' && *pos != '\n') {
                hex[1] = *pos++;
            }
            
            char* endptr;
            long value = strtol(hex, &endptr, 16);
            if (endptr == hex || value < 0 || value > 255) return 0;
            
            buffer[total_size++] = (uint8_t)value;
        }
    }
    
    return total_size;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output;
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: Invalid or no input data\n");
        return EXIT_FAILURE;
    }
    
    printf("Read %d bytes of input data\n", input_size);
    
    if (!compress_data(input, input_size, &output)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", output.size);
    print_buffer(&output);
    
    double ratio = (double)output.size / (double)input_size;
    printf("Compression ratio: %.2f\n", ratio);
    
    return EXIT_SUCCESS;
}