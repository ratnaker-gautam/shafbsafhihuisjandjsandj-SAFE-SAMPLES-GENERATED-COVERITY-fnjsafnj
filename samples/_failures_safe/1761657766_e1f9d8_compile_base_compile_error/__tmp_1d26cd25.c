//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressHeader;

size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

int compress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t max_output_size = safe_multiply(input_size, COMPRESSED_MULTIPLIER);
    if (max_output_size == SIZE_MAX || max_output_size < HEADER_SIZE) {
        return 0;
    }

    unsigned char* compressed = malloc(max_output_size);
    if (compressed == NULL) {
        return 0;
    }

    CompressHeader* header = (CompressHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t compressed_pos = HEADER_SIZE;
    for (size_t i = 0; i < input_size && compressed_pos < max_output_size - 1; i++) {
        compressed[compressed_pos++] = input[i] ^ 0x55;
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            size_t run_length = 1;
            while (i + run_length < input_size && input[i] == input[i + run_length] && run_length < 255) {
                run_length++;
            }
            compressed[compressed_pos++] = (unsigned char)run_length;
            i += run_length - 1;
        } else {
            compressed[compressed_pos++] = 1;
        }
    }

    if (compressed_pos >= max_output_size) {
        free(compressed);
        return 0;
    }

    *output = compressed;
    *output_size = compressed_pos;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return 0;
    }

    CompressHeader* header = (CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    unsigned char* decompressed = malloc(header->original_size);
    if (decompressed == NULL) {
        return 0;
    }

    size_t input_pos = HEADER_SIZE;
    size_t output_pos = 0;
    
    while (input_pos < input_size && output_pos < header->original_size) {
        if (input_pos + 1 >= input_size) {
            free(decompressed);
            return 0;
        }
        
        unsigned char value = input[input_pos++] ^ 0x55;
        unsigned char count = input[input_pos++];
        
        if (count == 0 || output_pos + count > header->original_size) {
            free(decompressed);
            return 0;
        }
        
        for (unsigned char i = 0; i < count; i++) {
            decompressed[output_pos++] = value;
        }
    }

    if (output_pos != header->original_size) {
        free(decompressed);
        return 0;
    }

    *output = decompressed;
    *output_size = header->original_size;
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char buffer[MAX_INPUT_SIZE * 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data, buffer, len);
    input_size = len;
    
    unsigned char* compressed = NULL;
    size_t compressed_size = 0;
    
    if (!compress_data(input_data, input_size, &compressed, &compressed_size)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_size, compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / input_size * 100);
    
    unsigned char* decompressed = NULL;
    size_t decompressed_size = 0;
    
    if (!decompress_data(compressed, compressed_size, &decompressed, &de