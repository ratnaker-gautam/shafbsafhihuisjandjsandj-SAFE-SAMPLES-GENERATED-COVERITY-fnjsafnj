//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCD1234

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 12];
} compressed_data_t;

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t stub_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size < input_size / COMPRESSION_RATIO + 12) return 0;
    
    compressed_data_t* comp = (compressed_data_t*)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint32_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size && compressed_len < output_size - 12; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        uint8_t run_value = input[i];
        uint8_t run_length = 1;
        
        for (size_t j = 1; j < chunk_len; j++) {
            if (input[i + j] == run_value && run_length < UINT8_MAX) {
                run_length++;
            } else {
                break;
            }
        }
        
        if (compressed_len + 2 > output_size - 12) break;
        output[12 + compressed_len++] = run_value;
        output[12 + compressed_len++] = run_length;
        i += (run_length - 1);
    }
    
    comp->compressed_size = (uint32_t)compressed_len;
    return compressed_len + 12;
}

static size_t stub_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < 12) return 0;
    
    const compressed_data_t* comp = (const compressed_data_t*)input;
    if (comp->header != MAGIC_HEADER) return 0;
    if (comp->original_size == 0 || comp->original_size > MAX_INPUT_SIZE) return 0;
    if (comp->compressed_size > input_size - 12) return 0;
    if (output_size < comp->original_size) return 0;
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < comp->compressed_size && decompressed_len < comp->original_size; i += 2) {
        if (i + 1 >= comp->compressed_size) break;
        
        uint8_t value = input[12 + i];
        uint8_t count = input[12 + i + 1];
        
        if (count == 0) continue;
        if (decompressed_len + count > comp->original_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_len++] = value;
        }
    }
    
    return decompressed_len;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02X ", data[i]);
    }
    if (size > 32) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char buffer[MAX_INPUT_SIZE + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_size = stub_compress(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("