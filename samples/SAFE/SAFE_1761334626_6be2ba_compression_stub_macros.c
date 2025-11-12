//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define RLE_THRESHOLD 3
#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_rle_compress(const uint8_t *input, size_t len, Buffer *output) {
    output->size = 0;
    if (len == 0) return;

    size_t i = 0;
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }

        if (count >= RLE_THRESHOLD) {
            if (output->size + 3 > MAX_OUTPUT_SIZE) break;
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
            i += count;
        } else {
            if (output->size + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output->data[output->size++] = input[i + j];
            }
            i += count;
        }
    }
}

static void hex_dump(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen((char*)input);
    if (len > 0 && input[len-1] == '\n') {
        input[--len] = '\0';
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    simple_rle_compress(input, len, &output);
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f%%\n", 
           (len > 0) ? (100.0 * output.size / len) : 0.0);
    
    printf("\nCompressed data (hex):\n");
    hex_dump(output.data, output.size);
    
    int printable_count = 0;
    for (size_t i = 0; i < output.size; i++) {
        if (IS_PRINTABLE(output.data[i])) printable_count++;
    }
    printf("\nPrintable characters in output: %d/%zu\n", 
           printable_count, output.size);
    
    return EXIT_SUCCESS;
}