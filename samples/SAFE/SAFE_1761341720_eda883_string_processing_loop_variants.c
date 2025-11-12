//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    int i, j, len;
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    printf("Alphanumeric characters: ");
    i = 0;
    while (processed[i] != '\0') {
        putchar(processed[i++]);
    }
    putchar('\n');
    
    printf("Reversed: ");
    i = j - 1;
    do {
        putchar(processed[i--]);
    } while (i >= 0);
    putchar('\n');
    
    printf("Character counts:\n");
    int counts[256] = {0};
    for (i = 0; processed[i] != '\0'; i++) {
        unsigned char c = (unsigned char)processed[i];
        if (c < 256) {
            counts[c]++;
        }
    }
    
    for (i = 0; i < 256; i++) {
        if (counts[i] > 0) {
            printf("'%c': %d\n", (char)i, counts[i]);
        }
    }
    
    return 0;
}