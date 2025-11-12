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
#define MAGIC_HEADER 0xABCD1234

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 12];
} compressed_data_t;

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint32_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size && compressed_len < output_size - 12; i += CHUNK_SIZE) {
        size_t chunk_end = i + CHUNK_SIZE;
        if (chunk_end > input_size) chunk_end = input_size;
        
        uint8_t run_value = input[i];
        uint8_t run_length = 1;
        
        for (size_t j = i + 1; j < chunk_end; j++) {
            if (input[j] == run_value && run_length < UINT8_MAX) {
                run_length++;
            } else {
                break;
            }
        }
        
        if (compressed_len + 2 > output_size - 12) break;
        
        comp->data[compressed_len++] = run_value;
        comp->data[compressed_len++] = run_length;
    }
    
    comp->compressed_size = (uint32_t)compressed_len;
    return compressed_len + 12;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size > output_size) return 0;
    if (comp->compressed_size > input_size - 12) return 0;
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < comp->compressed_size && decompressed_len < output_size; i += 2) {
        if (i + 1 >= comp->compressed_size) break;
        
        uint8_t value = comp->data[i];
        uint8_t count = comp->data[i + 1];
        
        if (decompressed_len + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_len++] = value;
        }
    }
    
    if (decompressed_len != comp->original_size) return 0;
    return decompressed_len;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    size_t input_size = 0;
    
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos != '\0' && input_size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\n') {
                pos++;
                continue;
            }
            
            if (pos[0] == '\0' || pos[1] == '\0') break;
            
            unsigned int byte;
            if (sscanf(pos, "%2x", &byte) == 1) {
                if (byte <= UINT8_MAX) {
                    input_data[input_size++] = (uint8_t)byte;
                }
                pos += 2;
            } else {
                break;
            }
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("\nOriginal