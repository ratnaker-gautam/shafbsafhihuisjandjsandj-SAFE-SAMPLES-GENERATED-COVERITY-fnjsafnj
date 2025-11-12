//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAGIC_HEADER 0xDEADBEEF
#define STATUS_OK 0
#define STATUS_ERROR -1

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_INPUT_SIZE];
} compressed_data_t;

#define VALIDATE_PTR(ptr) if (!(ptr)) return STATUS_ERROR
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) return STATUS_ERROR
#define IS_VALID_HEADER(hdr) ((hdr) == MAGIC_HEADER)

static int compress_data(const uint8_t* input, size_t input_size, compressed_data_t* output) {
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return STATUS_ERROR;
    }
    
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_size;
    
    size_t compressed_idx = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSED_CHUNK) {
        CHECK_BOUNDS(compressed_idx, MAX_INPUT_SIZE);
        
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSED_CHUNK; j++) {
            size_t pos = i + j;
            if (pos < input_size) {
                compressed_byte |= (input[pos] & 0x01) << j;
            }
        }
        output->data[compressed_idx++] = compressed_byte;
    }
    
    output->compressed_size = (uint32_t)compressed_idx;
    return STATUS_OK;
}

static int decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    
    if (!IS_VALID_HEADER(input->header)) {
        return STATUS_ERROR;
    }
    
    if (input->original_size > output_size || input->original_size > MAX_INPUT_SIZE) {
        return STATUS_ERROR;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input->compressed_size; i++) {
        uint8_t compressed_byte = input->data[i];
        for (int j = 0; j < COMPRESSED_CHUNK; j++) {
            if (output_idx < input->original_size) {
                output[output_idx++] = (compressed_byte >> j) & 0x01 ? 0xFF : 0x00;
            }
        }
    }
    
    return STATUS_OK;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    size_t input_size = 0;
    
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        char* pos = line;
        while (*pos && input_size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\n') {
                pos++;
                continue;
            }
            
            if (sscanf(pos, "%2hhx", &input_data[input_size]) == 1) {
                input_size++;
                pos += 2;
            } else {
                break;
            }
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    if (compress_data(input_data, input_size, &compressed) != STATUS_OK) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%u bytes):\n", compressed.compressed_size);
    print_hex(compressed.data, compressed.compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed.compressed_size / compressed.original_size * 100);
    
    memset(output_data, 0, sizeof(output_data));
    if (decompress_data(&compressed, output_data, sizeof(output_data)) != STATUS_OK) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("\nDecompressed data (%u bytes):\n", compressed.original_size);
    print_hex(output_data,