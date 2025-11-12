//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_len, uint8_t* output) {
    if (chunk_len == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_len; i += 2) {
        if (i + 1 < chunk_len) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_size] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
            output_size++;
        } else {
            output[output_size] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
            output_size++;
        }
        if (output_size >= MAX_OUTPUT_SIZE) break;
    }
    return output_size;
}

static int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    const uint8_t* data = (const uint8_t*)input;
    size_t remaining = input_len;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        size_t chunk_len = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(data, chunk_len, 
                                              output->data + output->size);
        if (compressed_size == 0) break;
        
        output->size += compressed_size;
        data += chunk_len;
        remaining -= chunk_len;
        
        if (output->size > MAX_OUTPUT_SIZE - compressed_size) break;
    }
    
    return output->size > 0;
}

static void print_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    Buffer output;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (!compress_data(input, input_len, &output)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f:1\n", (float)input_len / output.size);
    printf("Compressed data (hex):\n");
    print_hex(&output);
    
    return EXIT_SUCCESS;
}