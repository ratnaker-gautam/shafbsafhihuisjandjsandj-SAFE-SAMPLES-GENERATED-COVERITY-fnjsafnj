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
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
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
    printf("Choose compression method:\n");
    printf("1. Run-length encoding\n");
    printf("2. No compression\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (choice < 1 || choice > 2) return -1;
    
    return (int)choice;
}

int get_operation(void) {
    char input[16];
    printf("Choose operation:\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long op = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (op < 1 || op > 2) return -1;
    
    return (int)op;
}

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    char input[MAX_INPUT_SIZE + 1];
    
    printf("Enter data to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return buf;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return buf;
    
    buf.data = malloc(len);
    if (buf.data == NULL) return buf;
    
    memcpy(buf.data, input, len);
    buf.size = len;
    return buf;
}

void process_data(CompressFunc compress, DecompressFunc decompress, int operation, const Buffer *input) {
    if (input == NULL || input->data == NULL || input->size == 0) {
        printf("Error: Invalid input data\n");
        return;
    }
    
    uint8_t output[MAX_OUTPUT_SIZE];
    size_t result_size;
    
    if (operation == 1) {
        result_size = compress(input->data, input->size, output, sizeof(output));
        printf("Compressed data (%zu bytes): ", result_size);
    } else {
        result_size = decompress(input->data, input->size, output, sizeof(output));
        printf("Decompressed data (%zu bytes): ", result_size);
    }
    
    if (result_size > 0) {
        for (size_t i = 0; i < result_size && i < 64; i++) {
            printf