//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_HEADER 0xABCDEF00
#define CHUNK_SIZE 8
#define ROTATE_BYTE(b) (((b) << 4) | ((b) >> 4))

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint8_t data[MAX_INPUT_SIZE];
} compressed_data_t;

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_compress(const uint8_t* input, size_t input_size, compressed_data_t* output) {
    output->header = COMPRESSED_HEADER;
    output->original_size = (uint32_t)input_size;
    
    for (size_t i = 0; i < input_size; i++) {
        if (i < MAX_INPUT_SIZE) {
            output->data[i] = ROTATE_BYTE(input[i]);
        }
    }
    
    size_t padding = CHUNK_SIZE - (input_size % CHUNK_SIZE);
    if (padding < CHUNK_SIZE) {
        for (size_t i = input_size; i < input_size + padding; i++) {
            if (i < MAX_INPUT_SIZE) {
                output->data[i] = 0xFF;
            }
        }
    }
}

static int simple_decompress(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input->header != COMPRESSED_HEADER) return 0;
    if (input->original_size > output_size || input->original_size > MAX_INPUT_SIZE) return 0;
    
    for (size_t i = 0; i < input->original_size; i++) {
        if (i < MAX_INPUT_SIZE) {
            output[i] = ROTATE_BYTE(input->data[i]);
        }
    }
    return 1;
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
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (!validate_input(input_buffer, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_buffer, input_size);
    
    simple_compress(input_buffer, input_size, &compressed);
    
    size_t compressed_size = input_size + (CHUNK_SIZE - (input_size % CHUNK_SIZE));
    if (compressed_size > MAX_INPUT_SIZE) compressed_size = MAX_INPUT_SIZE;
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    memset(output_buffer, 0, sizeof(output_buffer));
    
    if (simple_decompress(&compressed, output_buffer, sizeof(output_buffer))) {
        printf("\nDecompressed data (%u bytes):\n", compressed.original_size);
        print_hex(output_buffer, compressed.original_size);
        
        printf("\nDecompressed text: %s\n", output_buffer);
        
        if (memcmp(input_buffer, output_buffer, input_size) == 0) {
            printf("Compression/Decompression successful!\n");
        } else {
            printf("Compression/Decompression failed!\n");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}