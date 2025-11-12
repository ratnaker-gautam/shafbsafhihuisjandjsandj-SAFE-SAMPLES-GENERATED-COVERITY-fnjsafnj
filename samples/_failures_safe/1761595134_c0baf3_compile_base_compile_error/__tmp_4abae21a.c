//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer* buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer* buf) {
    if (buf->data != NULL) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer* buf, const unsigned char* data, size_t len) {
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        unsigned char* new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

int compress_rle(const unsigned char* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 1;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        unsigned char chunk[2] = { (unsigned char)count, current };
        if (!buffer_append(output, chunk, 2)) return 0;
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 1;
    if (input_size % 2 != 0) return 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        unsigned char count = input[i];
        unsigned char value = input[i + 1];
        
        if (count == 0) return 0;
        
        for (int j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int read_input(Buffer* buf) {
    if (!buffer_init(buf, CHUNK_SIZE)) return 0;
    
    unsigned char chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        if (total_read > SIZE_MAX - bytes_read) {
            buffer_free(buf);
            return 0;
        }
        total_read += bytes_read;
        
        if (total_read > MAX_INPUT_SIZE) {
            buffer_free(buf);
            return 0;
        }
        
        if (!buffer_append(buf, chunk, bytes_read)) {
            buffer_free(buf);
            return 0;
        }
        
        if (bytes_read < CHUNK_SIZE) break;
    }
    
    return 1;
}

int main(void) {
    printf("Select operation (1=compress, 2=decompress): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    getchar();
    
    if (choice != 1 && choice != 2) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    Buffer input;
    if (!read_input(&input)) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    Buffer output;
    if (!buffer_init(&output, input.size)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    int success = 0;
    if (choice == 1) {
        success = compress_rle(input.data, input.size, &output);
    } else {
        success = decompress_rle(input.data, input.size, &output);
    }
    
    if (!success) {
        fprintf(stderr, "Operation failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Output write failed\n");