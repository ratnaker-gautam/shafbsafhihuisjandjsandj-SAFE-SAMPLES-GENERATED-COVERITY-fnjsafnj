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

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[compressed_size] = (first << SHIFT_AMOUNT) | second;
            compressed_size++;
        }
    }
    return compressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = CHUNK_SIZE;
        if (i + CHUNK_SIZE > input_size) {
            chunk_size = input_size - i;
        }
        
        memcpy(chunk, input + i, chunk_size);
        if (chunk_size < CHUNK_SIZE) {
            memset(chunk + chunk_size, 0, CHUNK_SIZE - chunk_size);
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        memcpy(output->data + total_compressed, compressed_chunk, compressed_size);
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size) {
    char input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0) {
        return 0;
    }
    
    size_t bytes = len / 2;
    if (bytes > max_size) {
        return 0;
    }
    
    for (size_t i = 0; i < bytes; i++) {
        char hex[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            return 0;
        }
        buffer[i] = (uint8_t)value;
    }
    
    return bytes;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer output_data;
    
    printf("Enter hex data to compress: ");
    int input_size = read_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!compress_data(input_data, input_size, &output_data)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", output_data.size);
    printf("Compressed data: ");
    print_hex(output_data.data, output_data.size);
    
    return EXIT_SUCCESS;
}