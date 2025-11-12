//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_index++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_index++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t current_chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (total_compressed + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(&input[i], current_chunk_size, 
                                              &output->data[total_compressed]);
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    printf("Enter data to compress (max %zu bytes, end with empty line):\n", max_size);
    
    size_t total_read = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') break;
        
        size_t line_len = strlen(line);
        if (line[line_len - 1] == '\n') line_len--;
        
        if (total_read + line_len > max_size) {
            printf("Input too large\n");
            return 0;
        }
        
        memcpy(&buffer[total_read], line, line_len);
        total_read += line_len;
    }
    
    *actual_size = total_read;
    return total_read > 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed_data;
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE, &input_size)) {
        printf("No valid input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_buffer, input_size);
    
    if (!compress_data(input_buffer, input_size, &compressed_data)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_data.size);
    print_hex(compressed_data.data, compressed_data.size);
    
    printf("Compression ratio: %.2f\n", 
           (float)input_size / (compressed_data.size > 0 ? compressed_data.size : 1));
    
    return 0;
}