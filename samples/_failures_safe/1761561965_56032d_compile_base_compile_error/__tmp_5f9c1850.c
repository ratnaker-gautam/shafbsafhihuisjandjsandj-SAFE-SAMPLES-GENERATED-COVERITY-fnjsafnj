//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum << 5) + sum + data[i];
    }
    return sum;
}

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return EXIT_FAILURE;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t max_output = *output_len;
    *output_len = 0;
    
    if (max_output < HEADER_SIZE + input_len / COMPRESSION_RATIO) {
        return EXIT_FAILURE;
    }
    
    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t in_idx = 0; in_idx < input_len; in_idx += COMPRESSION_RATIO) {
        CHECK_BOUNDS(out_idx, max_output);
        output[out_idx] = input[in_idx];
        out_idx++;
    }
    
    *output_len = out_idx;
    uint32_t checksum = calculate_checksum(input, input_len);
    memcpy(output + out_idx, &checksum, sizeof(checksum));
    *output_len += sizeof(checksum);
    
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return EXIT_FAILURE;
    }
    if (input_len < HEADER_SIZE + sizeof(uint32_t)) {
        return EXIT_FAILURE;
    }
    
    const uint32_t *header = (const uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    uint32_t orig_len = header[1];
    if (orig_len == 0 || orig_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (*output_len < orig_len) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_len - HEADER_SIZE - sizeof(uint32_t);
    size_t expected_compressed = (orig_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    if (compressed_size != expected_compressed) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < orig_len; i++) {
        size_t comp_idx = i / COMPRESSION_RATIO;
        CHECK_BOUNDS(comp_idx, compressed_size);
        output[i] = input[HEADER_SIZE + comp_idx];
    }
    
    uint32_t expected_checksum;
    memcpy(&expected_checksum, input + HEADER_SIZE + compressed_size, sizeof(expected_checksum));
    uint32_t actual_checksum = calculate_checksum(output, orig_len);
    if (expected_checksum != actual_checksum) {
        return EXIT_FAILURE;
    }
    
    *output_len = orig_len;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, line, input_len);
    
    size_t compressed_len = sizeof(compressed_data);
    if (compress_data(input_data, input_len, compressed_data, &compressed_len) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_len);
    
    size_t decompressed_len = sizeof(decompressed_data);
    if (decompress_data(compressed_data, compressed_len, decompressed_data, &decompressed_len) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }