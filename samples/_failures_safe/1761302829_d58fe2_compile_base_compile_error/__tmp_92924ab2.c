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
    if (output_size < input_size * 2) return 0;
    
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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_input(CompressFunc compress, DecompressFunc decompress, const uint8_t *input, size_t input_size, int mode) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) return result;
    if (compress == NULL || decompress == NULL) return result;
    
    size_t max_output_size = MAX_OUTPUT_SIZE;
    uint8_t *output_buffer = malloc(max_output_size);
    if (output_buffer == NULL) return result;
    
    size_t processed_size;
    
    if (mode == 0) {
        processed_size = compress(input, input_size, output_buffer, max_output_size);
    } else {
        processed_size = decompress(input, input_size, output_buffer, max_output_size);
    }
    
    if (processed_size == 0 || processed_size > max_output_size) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = processed_size;
    return result;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter input data (hex characters, 0-9A-F): ");
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len % 2 != 0) {
        printf("Invalid input length\n");
        return 1;
    }
    
    size_t binary_size = input_len / 2;
    if (binary_size > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < binary_size; i++) {
        char hex_byte[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            printf("Invalid hex data\n");
            return 1;
        }
        input_buffer[i] = (uint8_t)value;
    }
    
    printf("Choose operation (0=compress, 1=decompress): ");
    int operation;
    if (scanf("%d", &operation) != 1