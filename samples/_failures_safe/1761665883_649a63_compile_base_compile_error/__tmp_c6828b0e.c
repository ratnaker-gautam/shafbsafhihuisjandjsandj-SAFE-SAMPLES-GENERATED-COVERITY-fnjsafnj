//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define RUN_LIMIT 255

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < RUN_LIMIT && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            size_t literal_start = i;
            size_t literal_count = 0;
            
            while (i < input_len && literal_count < RUN_LIMIT) {
                if (i + 2 < input_len && input[i] == input[i + 1] && input[i] == input[i + 2]) {
                    break;
                }
                literal_count++;
                i++;
                if (literal_count == RUN_LIMIT) break;
            }
            
            if (literal_count > 0) {
                output->data[out_idx++] = (uint8_t)literal_count;
                for (size_t j = 0; j < literal_count && out_idx < MAX_OUTPUT_SIZE; j++) {
                    output->data[out_idx++] = input[literal_start + j];
                }
            }
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < output_size) {
        if (input->data[in_idx] == 0xFF) {
            if (in_idx + 2 >= input->size) break;
            
            uint8_t value = input->data[in_idx + 1];
            uint8_t count = input->data[in_idx + 2];
            
            if (out_idx + count > output_size) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            uint8_t literal_count = input->data[in_idx];
            
            if (in_idx + literal_count >= input->size) break;
            if (out_idx + literal_count > output_size) break;
            
            in_idx++;
            for (uint8_t j = 0; j < literal_count; j++) {
                output[out_idx++] = input->data[in_idx++];
            }
        }
    }
    
    return out_idx;
}

static void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % CHUNK_SIZE == 0) printf("\n");
        else if (i < len - 1) printf(" ");
    }
    if (len % CHUNK_SIZE != 0) printf("\n");
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter data to compress (hex bytes, space separated): ");
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = 0;
    char* token = strtok(line, " \n");
    
    while (token != NULL && input_len < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return EXIT_FAILURE;
        }
        original[input_len++] = (uint8_t)byte;
        token = strtok(NULL, " \n");
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid input data\n");
        return EXIT_FAILURE;
    }
    
    printf