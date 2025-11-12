//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) exit(EXIT_FAILURE);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        buffer_append(output, (uint8_t)count);
        buffer_append(output, current);
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t count = *(input + i);
        uint8_t value = *(input + i + 1);
        
        if (count == 0) return -1;
        
        for (size_t j = 0; j < count; j++) {
            buffer_append(output, value);
        }
        
        i += 2;
    }
    
    return 0;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &data_size) != 1 || data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1 || value < 0 || value > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        *(original_data + i) = (uint8_t)value;
    }
    
    Buffer compressed;
    buffer_init(&compressed, data_size);
    
    if (compress_rle(original_data, data_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", data_size, compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / data_size) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", *(compressed.data + i));
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, data_size);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (decompressed.size != data_size) {
        printf("Decompression size mismatch\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    int match = 1;
    for (size_t i = 0; i < data_size; i++) {
        if (*(original_data + i) != *(decompressed.data + i)) {
            match = 0;
            break;
        }
    }
    
    printf("Decompression %s\n", match ? "successful" : "failed");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return match ? EXIT_SUCCESS : EXIT_FAILURE;
}