//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
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
        
        if (out_idx + count > output_size) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t copy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    if (input_size > output_size) {
        return 0;
    }
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

int get_user_choice(void) {
    char input[16];
    printf("Choose compression method:\n");
    printf("1. Run-length encoding\n");
    printf("2. No compression (copy)\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (choice < 1 || choice > 2) {
        return -1;
    }
    
    return (int)choice;
}

int get_operation(void) {
    char input[16];
    printf("Choose operation:\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long op = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (op < 1 || op > 2) {
        return -1;
    }
    
    return (int)op;
}

int main(void) {
    CompressFunc compress_funcs[] = {run_length_encode, copy_compress};
    DecompressFunc decompress_funcs[] = {run_length_decode, copy_decompress};
    const char *method_names[] = {"Run-length encoding", "No compression"};
    
    int method_choice = get_user_choice();
    if (method_choice == -1) {
        fprintf(stderr, "Invalid method choice\n");
        return 1;
    }
    
    int operation = get_operation();
    if (operation == -1) {
        fprintf(stderr, "Invalid operation choice\n");
        return 1;
    }
    
    printf("Enter input data (max %d bytes): ", MAX_INPUT_SIZE);
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_line[MAX_INPUT_SIZE * 2];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data, input_line, input_len);