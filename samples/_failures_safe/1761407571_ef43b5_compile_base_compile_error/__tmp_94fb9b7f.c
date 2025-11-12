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
#define MAX_FILENAME 256

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[];
} compressed_file_t;

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(val, max) if ((val) > (max)) { return EXIT_FAILURE; }
#define SAFE_READ(fptr, buf, size) if (fread((buf), 1, (size), (fptr)) != (size)) { return EXIT_FAILURE; }
#define SAFE_WRITE(fptr, buf, size) if (fwrite((buf), 1, (size), (fptr)) != (size)) { return EXIT_FAILURE; }

static int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    *output_size = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (*output_size >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 4) | (input[i + j] & 0x0F);
        }
        output[(*output_size)++] = compressed_byte;
    }
    return 0;
}

static int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    *output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (*output_size >= MAX_INPUT_SIZE) {
            return -1;
        }
        uint8_t compressed_byte = input[i];
        output[(*output_size)++] = (compressed_byte >> 4) & 0x0F;
        if (*output_size < MAX_INPUT_SIZE) {
            output[(*output_size)++] = compressed_byte & 0x0F;
        }
    }
    return 0;
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int operation;
    
    printf("Enter operation (1=compress, 2=decompress): ");
    if (scanf("%d", &operation) != 1) {
        return EXIT_FAILURE;
    }
    if (operation != 1 && operation != 2) {
        return EXIT_FAILURE;
    }
    
    printf("Enter input filename: ");
    if (scanf("%255s", input_filename) != 1) {
        return EXIT_FAILURE;
    }
    
    printf("Enter output filename: ");
    if (scanf("%255s", output_filename) != 1) {
        return EXIT_FAILURE;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return EXIT_FAILURE;
    }
    
    if (operation == 1) {
        uint8_t input_data[MAX_INPUT_SIZE];
        size_t bytes_read = fread(input_data, 1, MAX_INPUT_SIZE, input_file);
        if (bytes_read == 0 || ferror(input_file)) {
            fclose(input_file);
            return EXIT_FAILURE;
        }
        fclose(input_file);
        
        uint8_t compressed_data[MAX_OUTPUT_SIZE];
        size_t compressed_size;
        if (compress_data(input_data, bytes_read, compressed_data, &compressed_size) != 0) {
            return EXIT_FAILURE;
        }
        
        FILE* output_file = fopen(output_filename, "wb");
        if (output_file == NULL) {
            return EXIT_FAILURE;
        }
        
        compressed_file_t header = {
            .magic = MAGIC_HEADER,
            .original_size = (uint32_t)bytes_read,
            .compressed_size = (uint32_t)compressed_size
        };
        
        SAFE_WRITE(output_file, &header, sizeof(header));
        SAFE_WRITE(output_file, compressed_data, compressed_size);
        fclose(output_file);
        
        printf("Compression complete: %zu -> %zu bytes\n", bytes_read, compressed_size);
    } else {
        compressed_file_t header;
        SAFE_READ(input_file, &header, sizeof(header));
        if (header.magic != MAGIC_HEADER) {
            fclose(input_file);
            return EXIT_FAILURE;
        }
        CHECK_BOUNDS(header.original_size, MAX_INPUT_SIZE);