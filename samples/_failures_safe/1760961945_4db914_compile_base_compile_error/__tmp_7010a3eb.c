//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_HEADER 0xDEADBEEF
#define CHUNK_SIZE 8
#define MAX_CHUNKS (MAX_INPUT_SIZE / CHUNK_SIZE)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_CHUNKS];
} compressed_data_t;

static size_t read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) return 0;
    
    printf("Enter data to compress (max %zu bytes): ", max_size - 1);
    if (!fgets((char*)buffer, max_size, stdin)) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    return len;
}

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (!input || !output || len == 0) return -1;
    
    uint8_t compressed = 0;
    for (size_t i = 0; i < len && i < CHUNK_SIZE; i++) {
        if (input[i] > 127) return -1;
        compressed |= (input[i] & 1) << i;
    }
    *output = compressed;
    return 0;
}

static int decompress_chunk(uint8_t compressed, uint8_t *output, size_t len) {
    if (!output || len == 0) return -1;
    
    for (size_t i = 0; i < len && i < CHUNK_SIZE; i++) {
        output[i] = (compressed & (1 << i)) ? '1' : '0';
    }
    return 0;
}

static int perform_compression(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    if (!input || !compressed || input_len > MAX_INPUT_SIZE) return -1;
    
    compressed->header = COMPRESSED_HEADER;
    compressed->original_size = (uint32_t)input_len;
    
    size_t num_chunks = (input_len + CHUNK_SIZE - 1) / CHUNK_SIZE;
    if (num_chunks > MAX_CHUNKS) return -1;
    
    compressed->compressed_size = (uint32_t)num_chunks;
    
    for (size_t i = 0; i < num_chunks; i++) {
        size_t chunk_start = i * CHUNK_SIZE;
        size_t chunk_len = (chunk_start + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - chunk_start;
        
        if (compress_chunk(input + chunk_start, chunk_len, &compressed->data[i]) != 0) {
            return -1;
        }
    }
    return 0;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t max_output) {
    if (!compressed || !output || max_output == 0) return -1;
    if (compressed->header != COMPRESSED_HEADER) return -1;
    if (compressed->original_size > max_output) return -1;
    
    size_t bytes_written = 0;
    for (size_t i = 0; i < compressed->compressed_size; i++) {
        size_t chunk_start = i * CHUNK_SIZE;
        size_t chunk_len = (chunk_start + CHUNK_SIZE <= compressed->original_size) ? 
                          CHUNK_SIZE : compressed->original_size - chunk_start;
        
        if (decompress_chunk(compressed->data[i], output + chunk_start, chunk_len) != 0) {
            return -1;
        }
        bytes_written += chunk_len;
    }
    
    if (bytes_written != compressed->original_size) return -1;
    output[bytes_written] = '\0';
    return 0;
}

static void print_hex(const uint8_t *data, size_t len) {
    if (!data) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    size_t input_len = read_input(input_buffer, sizeof(input_buffer));
    if (input_len == 0) {
        printf("No input received or input too large\n");
        return 1;
    }
    
    printf("Original data: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input_len);
    
    if (perform_compression(input_buffer, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compressed data: ");
    print_hex(compressed.data, compressed.compressed_size);
    
    if (perform_decompression(&compressed, output_buffer, sizeof(output_buffer)) != 0) {
        printf("Decompression failed