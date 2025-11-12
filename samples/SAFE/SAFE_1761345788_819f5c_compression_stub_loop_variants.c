//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    return 1;
}

Buffer compress_run_length(const char *input, size_t len) {
    Buffer result = {NULL, 0};
    if (len == 0) {
        return result;
    }
    
    uint8_t *output = malloc(len * 2);
    if (output == NULL) {
        return result;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < len) {
        char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_index + 2 > len * 2) {
            free(output);
            result.data = NULL;
            return result;
        }
        
        output[out_index++] = (uint8_t)count;
        output[out_index++] = current;
        
        i = j;
    }
    
    result.data = output;
    result.size = out_index;
    return result;
}

void print_compressed(const Buffer *compressed) {
    if (compressed->data == NULL || compressed->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        printf("[%u '%c'] ", compressed->data[i], compressed->data[i + 1]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Length: %zu\n", len);
    
    Buffer compressed = compress_run_length(input, len);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_compressed(&compressed);
    
    double ratio = (len > 0) ? (double)compressed.size / len : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    free(compressed.data);
    return 0;
}