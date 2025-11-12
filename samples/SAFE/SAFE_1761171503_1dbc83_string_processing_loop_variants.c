//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
    int i, j, len;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
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
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    
    int word_count = 0;
    int in_word = 0;
    i = 0;
    while (input[i] != '\0') {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        i++;
    }
    
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    int k = 0;
    for (i = len - 1; i >= 0; i--) {
        reversed[k++] = input[i];
    }
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int char_count[256] = {0};
    for (i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (i = 0; i < 256; i++) {
        if (char_count[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", i, char_count[i]);
        }
    }
    
    return 0;
}