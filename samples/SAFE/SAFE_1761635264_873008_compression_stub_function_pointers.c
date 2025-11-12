//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
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
    printf("Choose operation (1=compress, 2=decompress): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) {
        return -1;
    }
    
    if (input[0] == '1') return 1;
    if (input[0] == '2') return 2;
    return -1;
}

int read_input_data(uint8_t *buffer, size_t max_size) {
    printf("Enter input data (max %zu bytes): ", max_size);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len;
}

void display_hex(const uint8_t *data, size_t size) {
    printf("Result (%zu bytes): ", size);
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02X ", data[i]);
    }
    if (size > 32) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Simple Run-Length Compression Demo\n");
    printf("==================================\n");
    
    int choice = get_user_choice();
    if (choice == -1) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    int input_size = read_input_data(input_data, sizeof(input_data));
    if (input_size <= 0) {
        printf("Invalid input data.\n");
        return 1;
    }
    
    size_t result_size = 0;
    
    if (choice == 1) {
        result_size = compress_func(input_data, input_size, output_data, sizeof(output_data));
        if (result_size == 0) {
            printf("Compression failed or output buffer too small.\n");
            return 1;
        }
        printf("Compressed ");
    } else {
        result_size = decompress_func(input_data, input_size, output_data, sizeof(output_data));
        if (result_size == 0) {
            printf("Decompression failed or output buffer too small.\n");
            return 1;
        }
        printf("Decompressed ");
    }
    
    display_hex(output_data, result_size);
    
    printf("Text output: ");
    for (size_t i = 0; i < result_size && i < 64; i++) {
        if (isprint(output_data[i])) {
            printf("%c", output_data[i]);
        } else {
            printf(".");
        }
    }
    if (result_size > 64) printf("...");
    printf("\n");
    
    return 0;
}