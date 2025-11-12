//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_data(const char* input, size_t input_len, char* output, size_t output_len) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_len < input_len * COMPRESSED_MULTIPLIER) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (out_idx + 2 > output_len) return 0;
        output[out_idx++] = input[i];
        output[out_idx++] = input[i];
    }
    return out_idx;
}

size_t decompress_data(const char* input, size_t input_len, char* output, size_t output_len) {
    if (input == NULL || input_len == 0 || input_len % 2 != 0) return 0;
    if (output == NULL || output_len < input_len / 2) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) return 0;
        if (out_idx >= output_len) return 0;
        if (input[i] != input[i + 1]) return 0;
        output[out_idx++] = input[i];
    }
    return out_idx;
}

int write_compressed_file(const char* filename, const char* data, size_t data_len) {
    if (filename == NULL || data == NULL || data_len == 0) return EXIT_FAILURE;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return EXIT_FAILURE;
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)data_len;
    
    if (fwrite(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fwrite(data, 1, data_len, file) != data_len) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fclose(file) != 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int read_compressed_file(const char* filename, char** data, size_t* data_len) {
    if (filename == NULL || data == NULL || data_len == NULL) return EXIT_FAILURE;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return EXIT_FAILURE;
    
    CompressionHeader header;
    if (fread(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (header.magic != MAGIC_NUMBER) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (header.original_size == 0 || header.original_size > MAX_INPUT_SIZE) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    *data_len = header.original_size;
    *data = malloc(*data_len);
    if (*data == NULL) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fread(*data, 1, *data_len, file) != *data_len) {
        free(*data);
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fclose(file) != 0) {
        free(*data);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_len * COMPRESSED_MULTIPLIER;
    char* compressed_data = malloc(compressed_size);
    if (compressed_data == NULL) {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    size_t actual_compressed =