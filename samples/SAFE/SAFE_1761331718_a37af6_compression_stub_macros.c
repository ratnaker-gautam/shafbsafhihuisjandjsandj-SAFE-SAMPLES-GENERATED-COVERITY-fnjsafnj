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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_index++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_index++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (!validate_input(input, input_size)) return result;
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(&input[i], chunk_size, &result.data[output_index]);
        if (output_index + compressed_size >= MAX_OUTPUT_SIZE) break;
        output_index += compressed_size;
    }
    result.size = output_index;
    return result;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos != '\0' && input_size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\t' || *pos == '\n') {
                pos++;
                continue;
            }
            
            if (pos[0] == '\0' || pos[1] == '\0') break;
            
            unsigned int byte_val;
            if (sscanf(pos, "%2x", &byte_val) != 1) break;
            
            input_data[input_size++] = (uint8_t)byte_val;
            pos += 2;
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    Buffer compressed = compress_data(input_data, input_size);
    
    if (compressed.size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    double ratio = (double)compressed.size / (double)input_size;
    printf("\nCompression ratio: %.2f\n", ratio);
    
    return 0;
}