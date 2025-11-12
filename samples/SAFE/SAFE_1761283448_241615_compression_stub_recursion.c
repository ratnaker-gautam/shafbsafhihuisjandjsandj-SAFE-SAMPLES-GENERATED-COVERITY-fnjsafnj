//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_rle(const unsigned char *input, size_t len, size_t pos) {
    Buffer result = {NULL, 0};
    if (pos >= len) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }

    unsigned char current = input[pos];
    size_t count = 1;
    size_t i = pos + 1;
    while (i < len && input[i] == current && count < UCHAR_MAX) {
        count++;
        i++;
    }

    Buffer rest = compress_rle(input, len, i);
    if (rest.data == NULL && rest.size > 0) {
        return result;
    }

    size_t needed = rest.size + 2;
    if (needed < rest.size) {
        free(rest.data);
        return result;
    }

    result.data = malloc(needed);
    if (result.data == NULL) {
        free(rest.data);
        return result;
    }

    result.data[0] = current;
    result.data[1] = (unsigned char)count;
    memcpy(result.data + 2, rest.data, rest.size);
    result.size = rest.size + 2;
    free(rest.data);
    return result;
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }

    if (len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    Buffer compressed = compress_rle((unsigned char*)input, len, 0);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(len - compressed.size) / len * 100.0f);

    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i > 0) printf(" ");
        printf("%c%u", compressed.data[i], (unsigned int)compressed.data[i+1]);
    }
    printf("\n");

    free(compressed.data);
    return 0;
}