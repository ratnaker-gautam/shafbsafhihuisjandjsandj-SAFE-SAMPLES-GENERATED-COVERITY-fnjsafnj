//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} FileHeader;

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(output_idx, input_size * COMPRESSED_MULTIPLIER);
        uint8_t current = input[i];
        output[output_idx++] = current;
        output[output_idx++] = current;
    }
    return output_idx;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return EXIT_FAILURE;
    }
    
    if (input_size % 2 != 0) {
        return EXIT_FAILURE;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        CHECK_BOUNDS(i + 1, input_size);
        CHECK_BOUNDS(output_idx, output_size);
        if (input[i] != input[i + 1]) {
            return EXIT_FAILURE;
        }
        output[output_idx++] = input[i];
    }
    return EXIT_SUCCESS;
}

static int write_compressed_file(const char *filename, const uint8_t *data, size_t data_size, size_t original_size) {
    if (filename == NULL || data == NULL || data_size == 0) {
        return EXIT_FAILURE;
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return EXIT_FAILURE;
    }
    
    FileHeader header = { MAGIC_NUMBER, (uint32_t)original_size };
    if (fwrite(&header, sizeof(FileHeader), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fwrite(data, 1, data_size, file) != data_size) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fclose(file) != 0) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

static int read_compressed_file(const char *filename, uint8_t *data, size_t *data_size, size_t *original_size) {
    if (filename == NULL || data == NULL || data_size == NULL || original_size == NULL) {
        return EXIT_FAILURE;
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return EXIT_FAILURE;
    }
    
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (header.magic != MAGIC_NUMBER) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    *original_size = header.original_size;
    *data_size = 0;
    
    while (!feof(file)) {
        CHECK_BOUNDS(*data_size, MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER);
        size_t read_count = fread(&data[*data_size], 1, 1, file);
        if (read_count != 1 && !feof(file)) {
            fclose(file);
            return EXIT_FAILURE;
        }
        if (read_count == 1) {
            (*data_size)++;
        }
    }
    
    if (fclose(file) != 0) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char *)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char *)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_data(input_buffer, input_size, compressed_buffer);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }