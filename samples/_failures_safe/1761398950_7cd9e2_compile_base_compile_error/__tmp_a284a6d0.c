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
    CHECK_BOUNDS(output, len * 2, MAX_OUTPUT_SIZE);
    
    for (size_t i = 0; i < len; i++) {
        output[i * 2] = input[i] >> 4;
        output[i * 2 + 1] = input[i] & 0x0F;
    }
    return len * 2;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE * 2);
    CHECK_BOUNDS(output, len / 2, MAX_INPUT_SIZE);
    
    if (len % 2 != 0) {
        return -1;
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        output[i] = (input[i * 2] << 4) | (input[i * 2 + 1] & 0x0F);
    }
    return len / 2;
}

int compress_data(const uint8_t* input, size_t input_len, compressed_data_t* output) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL) {
        return -1;
    }
    
    output->header = MAGIC_HEADER;
    output->original_size = input_len;
    output->compressed_size = 0;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, output->data + total_compressed);
        if (result < 0) {
            return -1;
        }
        total_compressed += result;
    }
    
    output->compressed_size = total_compressed;
    return 0;
}

int decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->header != MAGIC_HEADER) {
        return -1;
    }
    
    if (output_size < input->original_size) {
        return -1;
    }
    
    size_t total_decompressed = 0;
    for (size_t i = 0; i < input->compressed_size; i += CHUNK_SIZE * 2) {
        size_t chunk_len = (input->compressed_size - i) < (CHUNK_SIZE * 2) ? 
                          (input->compressed_size - i) : (CHUNK_SIZE * 2);
        int result = decompress_chunk(input->data + i, chunk_len, output + total_decompressed);
        if (result < 0) {
            return -1;
        }
        total_decompressed += result;
    }
    
    if (total_decompressed != input->original_size) {
        return -1;
    }
    
    return 0;
}

void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t test_data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22};
    size_t test_len = sizeof(test_data);
    
    compressed_data_t compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Original data (%zu bytes):\n", test_len);
    print_hex(test_data, test_len);
    
    if (compress_data(test_data, test_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%u bytes):\n", compressed.compressed_size);
    print