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

static void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    output->size = 0;
    size_t chunks = input_size / CHUNK_SIZE;
    
    for (size_t i = 0; i < chunks && output->size < MAX_OUTPUT_SIZE - 2; i++) {
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            size_t idx = i * CHUNK_SIZE + j;
            if (idx < input_size) {
                compressed_byte |= ((input[idx] & 1) << j);
            }
        }
        output->data[output->size++] = compressed_byte;
    }
    
    if (input_size % CHUNK_SIZE != 0 && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t compressed_byte = 0;
        size_t remaining = input_size % CHUNK_SIZE;
        for (size_t j = 0; j < remaining; j++) {
            size_t idx = chunks * CHUNK_SIZE + j;
            if (idx < input_size) {
                compressed_byte |= ((input[idx] & 1) << j);
            }
        }
        output->data[output->size++] = compressed_byte;
        output->data[output->size++] = (uint8_t)remaining;
    } else if (output->size < MAX_OUTPUT_SIZE) {
        output->data[output->size++] = 0xFF;
    }
}

static void obfuscate_data(Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        buffer->data[i] ^= MASK_BYTE;
    }
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    printf("Enter input data (up to %zu bytes as hex digits, empty line to finish):\n", max_size);
    
    char line[256];
    *actual_size = 0;
    
    while (*actual_size < max_size && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos != '\0' && *actual_size < max_size) {
            if ((*pos >= '0' && *pos <= '9') || (*pos >= 'A' && *pos <= 'F') || (*pos >= 'a' && *pos <= 'f')) {
                int digit1 = *pos;
                pos++;
                if (*pos == '\0') break;
                
                if ((*pos >= '0' && *pos <= '9') || (*pos >= 'A' && *pos <= 'F') || (*pos >= 'a' && *pos <= 'f')) {
                    int digit2 = *pos;
                    
                    char hex[3] = {digit1, digit2, '\0'};
                    char* endptr;
                    long value = strtol(hex, &endptr, 16);
                    if (endptr != hex + 2) break;
                    if (value < 0 || value > 255) break;
                    
                    buffer[(*actual_size)++] = (uint8_t)value;
                    pos++;
                } else {
                    break;
                }
            } else if (*pos == ' ' || *pos == '\t' || *pos == '\n') {
                pos++;
            } else {
                break;
            }
        }
    }
    
    return *actual_size > 0;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: No valid input data provided\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Error: Invalid input data\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    simple_compress(input_data, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    obfuscate_data(&compressed);
    
    printf("