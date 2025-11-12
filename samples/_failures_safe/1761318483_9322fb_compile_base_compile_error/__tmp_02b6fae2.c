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

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE);
    CHECK_BOUNDS(output, len / COMPRESSION_RATIO, CHUNK_SIZE / COMPRESSION_RATIO);
    
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            output[i / COMPRESSION_RATIO] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[i / COMPRESSION_RATIO] = input[i] & 0xF0;
        }
    }
    return len / COMPRESSION_RATIO + (len % COMPRESSION_RATIO ? 1 : 0);
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE / COMPRESSION_RATIO);
    CHECK_BOUNDS(output, len * COMPRESSION_RATIO, CHUNK_SIZE);
    
    for (size_t i = 0; i < len; i++) {
        output[i * 2] = input[i] & 0xF0;
        if (i * 2 + 1 < len * COMPRESSION_RATIO) {
            output[i * 2 + 1] = (input[i] & 0x0F) << 4;
        }
    }
    return len * COMPRESSION_RATIO;
}

static int compress_data(const uint8_t* input, size_t input_len, compressed_data_t* output) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL) return -1;
    
    output->header = MAGIC_HEADER;
    output->original_size = input_len;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) > CHUNK_SIZE ? CHUNK_SIZE : (input_len - i);
        int result = compress_chunk(input + i, chunk_len, output->data + compressed_size);
        if (result < 0) return -1;
        compressed_size += result;
        if (compressed_size > MAX_OUTPUT_SIZE) return -1;
    }
    
    output->compressed_size = compressed_size;
    return 0;
}

static int decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return -1;
    if (input->header != MAGIC_HEADER) return -1;
    if (input->original_size > output_size || input->original_size > MAX_INPUT_SIZE) return -1;
    
    size_t decompressed_size = 0;
    size_t compressed_pos = 0;
    
    while (decompressed_size < input->original_size && compressed_pos < input->compressed_size) {
        size_t remaining = input->original_size - decompressed_size;
        size_t chunk_len = remaining > CHUNK_SIZE ? CHUNK_SIZE : remaining;
        
        size_t compressed_chunk_len = chunk_len / COMPRESSION_RATIO + (chunk_len % COMPRESSION_RATIO ? 1 : 0);
        if (compressed_pos + compressed_chunk_len > input->compressed_size) return -1;
        
        int result = decompress_chunk(input->data + compressed_pos, compressed_chunk_len, output + decompressed_size);
        if (result < 0) return -1;
        
        decompressed_size += result;
        compressed_pos += compressed_chunk_len;
    }
    
    return decompressed_size == input->original_size ? 0 : -1;
}

static void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed