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

#define CHECK_BOUNDS(ptr, size, max) do { \
    if ((size) > (max) || (ptr) == NULL) { \
        return -1; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
    if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
        return -1; \
    } \
} while(0)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE || input == NULL || output == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 < len) {
            output[i/2] = (input[i] >> 4) | ((input[i+1] >> 4) << 4);
        } else {
            output[i/2] = input[i] >> 4;
        }
    }
    
    return (len + 1) / 2;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE / 2 || input == NULL || output == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        output[i*2] = (input[i] & 0x0F) << 4;
        if (i*2 + 1 < len * 2) {
            output[i*2 + 1] = (input[i] >> 4) << 4;
        }
    }
    
    return len * 2;
}

int compress_data(const uint8_t *input, size_t input_len, compressed_data_t *output) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL) {
        return -1;
    }
    
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_len;
    
    size_t compressed_pos = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i < CHUNK_SIZE) ? input_len - i : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, output->data + compressed_pos);
        if (result < 0) {
            return -1;
        }
        compressed_pos += result;
        if (compressed_pos >= MAX_OUTPUT_SIZE) {
            return -1;
        }
    }
    
    output->compressed_size = (uint32_t)compressed_pos;
    return 0;
}

int decompress_data(const compressed_data_t *input, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input->header != MAGIC_HEADER) {
        return -1;
    }
    
    if (output_size < input->original_size) {
        return -1;
    }
    
    size_t decompressed_pos = 0;
    size_t compressed_pos = 0;
    
    while (decompressed_pos < input->original_size && compressed_pos < input->compressed_size) {
        size_t remaining = input->original_size - decompressed_pos;
        size_t chunk_len = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed_chunk_len = (chunk_len + 1) / 2;
        
        if (compressed_pos + compressed_chunk_len > input->compressed_size) {
            return -1;
        }
        
        int result = decompress_chunk(input->data + compressed_pos, compressed_chunk_len, output + decompressed_pos);
        if (result < 0) {
            return -1;
        }
        
        decompressed_pos += chunk_len;
        compressed_pos += compressed_chunk_len;
    }
    
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    if (compress_data(input_data, input_len, &compressed) != 0) {
        fprintf(stderr, "