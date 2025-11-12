//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i]) || isspace((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    size_t processed_len = j;
    char reversed[MAX_INPUT];
    
    size_t k = 0;
    while (k < processed_len) {
        reversed[k] = processed[processed_len - 1 - k];
        k++;
    }
    reversed[processed_len] = '\0';
    
    char vowels_removed[MAX_INPUT];
    size_t m = 0;
    size_t n = 0;
    
    do {
        char c = processed[n];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            vowels_removed[m++] = c;
        }
        n++;
    } while (n < processed_len);
    vowels_removed[m] = '\0';
    
    char words[MAX_INPUT][MAX_INPUT];
    size_t word_count = 0;
    size_t word_pos = 0;
    size_t p = 0;
    
    while (p < processed_len) {
        if (!isspace((unsigned char)processed[p])) {
            size_t start = p;
            while (p < processed_len && !isspace((unsigned char)processed[p])) {
                p++;
            }
            size_t word_len = p - start;
            if (word_len < MAX_INPUT && word_count < MAX_INPUT) {
                strncpy(words[word_count], processed + start, word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            }
        } else {
            p++;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Alphanumeric+spaces: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Word count: %zu\n", word_count);
    
    if (word_count > 0) {
        printf("Words: ");
        for (size_t q = 0; q < word_count; q++) {
            printf("[%s] ", words[q]);
        }
        printf("\n");
    }
    
    return 0;
}