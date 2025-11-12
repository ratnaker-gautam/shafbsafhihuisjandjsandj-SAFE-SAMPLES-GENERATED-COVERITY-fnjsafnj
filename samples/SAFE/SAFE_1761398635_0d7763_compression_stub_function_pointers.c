//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_choice(void) {
    char input[16];
    printf("Choose operation:\n1. Compress\n2. Decompress\n> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (choice < 1 || choice > 2) return -1;
    
    return (int)choice;
}

int get_input_data(uint8_t *buffer, size_t max_size, size_t *actual_size) {
    printf("Enter input data (max %zu bytes): ", max_size);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    *actual_size = len;
    return 1;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t output[MAX_OUTPUT_SIZE];
    size_t input_size, output_size;
    
    int choice = get_user_choice();
    if (choice == -1) {
        printf("Invalid choice\n");
        return;
    }
    
    if (!get_input_data(input, MAX_INPUT_SIZE, &input_size)) {
        printf("Failed to read input\n");
        return;
    }
    
    if (input_size == 0) {
        printf("No input data provided\n");
        return;
    }
    
    if (choice == 1) {
        output_size = compress(input, input_size, output, MAX_OUTPUT_SIZE);
        if (output_size == 0) {
            printf("Compression failed\n");
            return;
        }
        
        printf("Compressed data (%zu bytes): ", output_size);
        for (size_t i = 0; i < output_size; i++) {
            printf("%02X ", output[i]);
        }
        printf("\n");
        
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - output_size) / input_size * 100.0f);
    } else {
        output_size = decompress(input, input_size, output, MAX_OUTPUT_SIZE);
        if (output_size == 0) {
            printf("Decompression failed\n");
            return;
        }
        
        printf("Decompressed data (%zu bytes): ", output_size);
        for (size_t i = 0; i < output_size; i++) {
            printf("%c", output[i]);
        }
        printf("\n");
    }
}

int main(void) {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}