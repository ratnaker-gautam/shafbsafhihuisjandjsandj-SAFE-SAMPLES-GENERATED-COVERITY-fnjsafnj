//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define MAGIC_HEADER 0xDEADBEEF
#define MAX_CHUNK_SIZE 128

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

#define WRITE_HEADER(output, offset) do { \
    if ((offset) + sizeof(uint32_t) > MAX_OUTPUT_SIZE) { \
        return -1; \
    } \
    *((uint32_t*)((output) + (offset))) = MAGIC_HEADER; \
    (offset) += sizeof(uint32_t); \
} while(0)

#define COMPRESS_CHUNK(input, output, in_len, out_offset) do { \
    size_t chunk_size = (in_len) < MAX_CHUNK_SIZE ? (in_len) : MAX_CHUNK_SIZE; \
    if ((out_offset) + chunk_size / COMPRESSION_FACTOR + 1 > MAX_OUTPUT_SIZE) { \
        return -1; \
    } \
    for (size_t i = 0; i < chunk_size; i += COMPRESSION_FACTOR) { \
        uint8_t compressed = 0; \
        for (int j = 0; j < COMPRESSION_FACTOR && i + j < chunk_size; j++) { \
            compressed |= (((input)[i + j] & 0x3F) << (6 - j * 2)); \
        } \
        (output)[(out_offset)++] = compressed; \
    } \
} while(0)

int compress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL || output_len == NULL) {
        return -1;
    }
    
    size_t offset = 0;
    WRITE_HEADER(output, offset);
    
    if (offset + sizeof(uint32_t) > MAX_OUTPUT_SIZE) {
        return -1;
    }
    *((uint32_t*)(output + offset)) = (uint32_t)input_len;
    offset += sizeof(uint32_t);
    
    size_t remaining = input_len;
    const uint8_t* current = input;
    
    while (remaining > 0) {
        COMPRESS_CHUNK(current, output, remaining, offset);
        size_t chunk_processed = remaining < MAX_CHUNK_SIZE ? remaining : MAX_CHUNK_SIZE;
        current += chunk_processed;
        remaining -= chunk_processed;
    }
    
    *output_len = offset;
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_len = 0;
    size_t output_len = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE - 1) {
        input_buffer[input_len++] = (uint8_t)c;
    }
    input_buffer[input_len] = '\0';
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (compress_data(input_buffer, input_len, output_buffer, &output_len) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", output_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)output_len / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_len && i < 32; i++) {
        printf("%02X ", output_buffer[i]);
    }
    if (output_len > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}