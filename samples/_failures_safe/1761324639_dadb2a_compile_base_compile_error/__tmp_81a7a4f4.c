//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->length == 0) {
        output->length = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input->length == 0 || input->length % 2 != 0) {
        output->length = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    for (i = 0; i < input->length; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->length = out_idx;
}

int buffers_equal(const Buffer* a, const Buffer* b) {
    if (a->length != b->length) {
        return 0;
    }
    
    for (size_t idx = 0; idx < a->length; idx++) {
        if (a->data[idx] != b->data[idx]) {
            return 0;
        }
    }
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf->length == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t pos = 0; pos < buf->length; pos++) {
        printf("%02X", buf->data[pos]);
        if ((pos + 1) % 16 == 0) {
            printf("\n");
        } else if (pos + 1 < buf->length) {
            printf(" ");
        }
    }
    if (buf->length % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer original, compressed, decompressed;
    initialize_buffer(&original);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    char input_text[MAX_INPUT_SIZE];
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!validate_input((uint8_t*)input_text, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, input_text, input_len);
    original.length = input_len;
    
    simple_compress(&original, &compressed);
    
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (!buffers_equal(&original, &decompressed)) {
        fprintf(stderr, "Compression/decompression mismatch\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    double ratio = (compressed.length > 0) ? (double)original.length / compressed.length : 0.0;
    printf("Compression ratio: %.2f:1\n", ratio);
    
    printf("\nOriginal data (hex):\n");
    print_buffer