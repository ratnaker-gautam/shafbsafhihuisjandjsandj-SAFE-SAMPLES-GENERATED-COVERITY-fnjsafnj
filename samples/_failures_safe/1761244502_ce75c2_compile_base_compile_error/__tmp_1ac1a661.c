//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    if (input_size > output_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

int get_user_choice(void) {
    char input[16];
    printf("Choose compression method (1=RLE, 2=None): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    
    if (endptr == input || (*endptr != '\n' && *endptr != '\0')) return -1;
    if (choice < 1 || choice > 2) return -1;
    
    return (int)choice;
}

int get_operation(void) {
    char input[16];
    printf("Choose operation (1=Compress, 2=Decompress): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long op = strtol(input, &endptr, 10);
    
    if (endptr == input || (*endptr != '\n' && *endptr != '\0')) return -1;
    if (op < 1 || op > 2) return -1;
    
    return (int)op;
}

int read_input_data(uint8_t *buffer, size_t max_size, size_t *actual_size) {
    printf("Enter input data (max %zu bytes as hex, space separated): ", max_size);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t idx = 0;
    char *token = strtok(line, " \t\n");
    
    while (token != NULL && idx < max_size) {
        if (strlen(token) != 2) return 0;
        
        char *endptr;
        long value = strtol(token, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) return 0;
        
        buffer[idx++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    *actual_size = idx;
    return 1;
}

void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    
    int method = get_user_choice();
    if (method == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    int operation = get_operation();
    if (operation == -1) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (!read_input_data(input