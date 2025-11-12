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
#define MASK_BYTE 0xAA

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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    if (chunk == NULL || output == NULL) return 0;
    
    uint8_t compressed = 0;
    for (int i = 0; i < CHUNK_SIZE; i++) {
        uint8_t bit = (chunk[i] & 1);
        compressed |= (bit << i);
    }
    *output = compressed;
    return 1;
}

static size_t decompress_chunk(uint8_t compressed, uint8_t* output) {
    if (output == NULL) return 0;
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        uint8_t bit = (compressed >> i) & 1;
        output[i] = bit ? 0xFF : 0x00;
    }
    return CHUNK_SIZE;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t chunks = (input_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    
    if (chunks > MAX_OUTPUT_SIZE) return 0;
    
    for (size_t i = 0; i < chunks; i++) {
        uint8_t chunk[CHUNK_SIZE];
        memset(chunk, 0, CHUNK_SIZE);
        
        size_t bytes_to_copy = (i * CHUNK_SIZE + CHUNK_SIZE <= input_size) ? 
                              CHUNK_SIZE : input_size - i * CHUNK_SIZE;
        
        if (bytes_to_copy > CHUNK_SIZE) return 0;
        
        memcpy(chunk, input + i * CHUNK_SIZE, bytes_to_copy);
        
        if (output->size >= MAX_OUTPUT_SIZE) return 0;
        
        if (!compress_chunk(chunk, &output->data[output->size])) return 0;
        output->size++;
    }
    
    return 1;
}

static int decompress_data(const Buffer* compressed, Buffer* output) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    if (compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = 0;
    
    for (size_t i = 0; i < compressed->size; i++) {
        if (output->size + CHUNK_SIZE > MAX_OUTPUT_SIZE) return 0;
        
        size_t decompressed = decompress_chunk(compressed->data[i], 
                                             output->data + output->size);
        if (decompressed != CHUNK_SIZE) return 0;
        
        output->size += decompressed;
    }
    
    return 1;
}

static void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            printf("Invalid data\n");
            return 1;
        }
        if (value > 0xFF) {
            printf("Invalid byte value\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    Buffer original = {0};
    memcpy(original.data, input_data, input_size);
    original.size = input_size;
    print_buffer_hex(&original);
    
    printf("\nCompressing...\n");