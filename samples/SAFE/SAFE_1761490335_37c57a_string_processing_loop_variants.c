//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
    printf("Enter text to process: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i]) || isspace((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    size_t processed_len = strlen(processed);
    char reversed[MAX_INPUT];
    
    size_t k = 0;
    size_t m = processed_len;
    while (m > 0) {
        m--;
        reversed[k++] = processed[m];
    }
    reversed[k] = '\0';
    
    char vowels_removed[MAX_INPUT];
    size_t v = 0;
    size_t n = 0;
    do {
        char c = processed[n];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            vowels_removed[v++] = c;
        }
        n++;
    } while (n < processed_len);
    vowels_removed[v] = '\0';
    
    int word_count = 0;
    int in_word = 0;
    for (size_t p = 0; p < processed_len; p++) {
        if (isspace((unsigned char)processed[p])) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Filtered: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("No vowels: %s\n", vowels_removed);
    printf("Word count: %d\n", word_count);
    
    return 0;
}