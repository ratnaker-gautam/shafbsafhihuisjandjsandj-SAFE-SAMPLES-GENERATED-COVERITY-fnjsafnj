//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF

typedef enum {
    COMPRESS,
    DECOMPRESS
} operation_t;

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > MAX_OUTPUT_SIZE) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) {
                return 0;
            }
            
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + count > MAX_OUTPUT_SIZE) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx + 1 > MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static void print_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if (i < len - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter operation (1=compress, 2=decompress): ");
    int op_choice;
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    operation_t op;
    if (op_choice == 1) {
        op = COMPRESS;
    } else if (op_choice == 2) {
        op = DECOMPRESS;
    } else {
        printf("Invalid operation\n");
        return 1;
    }
    
    printf("Enter data as hex bytes (max %d bytes): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 3];
    if (scanf(" %[^\n]", hex_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    size_t input_len = 0;
    char *token = strtok(hex_input, " ");
    while (token != NULL && input_len < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1 || byte > 0xFF) {
            printf("Invalid hex byte: %s\n", token);
            return 1;
        }
        input_buffer[input_len++] = (uint8_t)byte;
        token = strtok(NULL, " ");
    }
    
    if (input_len == 0) {
        printf("No valid input data\n");
        return 1;
    }
    
    size_t result_len = 0;
    
    if (op == COMPRESS) {
        result_len = compress_data(input_buffer, input_len, output_buffer);
        if (result_len == 0) {
            printf("Compression failed\n");
            return 1;
        }
        
        printf("Compressed data (%zu bytes): ", result_len);
        print_hex(output_buffer, result_len);
        
        size_t decomp_len = decompress_data(output_buffer, result_len, decompressed_buffer);
        if (decomp_len == 0 || decomp_len != input_len || 
            memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
            printf("Verification failed