//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
        i++;
    }
    
    for (k = 0; k < j; k++) {
        processed[j + k] = processed[j - k - 1];
    }
    
    processed[j * 2] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char search;
    printf("Enter a character to count: ");
    if (scanf(" %c", &search) != 1) {
        fprintf(stderr, "Error reading character\n");
        return 1;
    }
    
    int count = 0;
    char *ptr = processed;
    while (*ptr) {
        if (*ptr == search) {
            count++;
        }
        ptr++;
    }
    
    printf("Character '%c' appears %d times\n", search, count);
    
    int word_count = 0;
    int in_word = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}