//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define MAX_RUN_LENGTH 127

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

static size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_size && 
               run_length < MAX_RUN_LENGTH && 
               input[in_idx + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = (uint8_t)(run_length | 0x80);
            output->data[out_idx++] = current;
            in_idx += run_length;
        } else {
            if (out_idx + run_length + 1 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = (uint8_t)run_length;
            for (size_t i = 0; i < run_length; i++) {
                output->data[out_idx++] = input[in_idx++];
            }
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx >= output_size) return 0;
        
        uint8_t control = input->data[in_idx++];
        
        if (control & 0x80) {
            size_t run_length = control & RUN_LENGTH_MASK;
            if (in_idx >= input->size) return 0;
            
            uint8_t value = input->data[in_idx++];
            
            if (out_idx + run_length > output_size) return 0;
            for (size_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
        } else {
            size_t literal_count = control;
            
            if (in_idx + literal_count > input->size) return 0;
            if (out_idx + literal_count > output_size) return 0;
            
            for (size_t i = 0; i < literal_count; i++) {
                output[out_idx++] = input->data[in_idx++];
            }
        }
    }
    
    return out_idx;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter up to %zu bytes of data (hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return 1;
        }
        input_data[input_size++] = (uint8_t)byte;
        token = strtok(NULL, " \n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    if (!compress_rle(input_data, input_size, &compressed)) {