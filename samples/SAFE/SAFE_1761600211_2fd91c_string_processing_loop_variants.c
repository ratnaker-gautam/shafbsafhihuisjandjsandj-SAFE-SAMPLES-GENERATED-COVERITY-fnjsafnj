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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
    
    char reversed[MAX_INPUT];
    size_t rev_len = strlen(processed);
    if (rev_len >= sizeof(reversed)) {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    size_t k = 0;
    while (rev_len > 0) {
        reversed[k++] = processed[rev_len - 1];
        rev_len--;
    }
    reversed[k] = '\0';
    
    char words[MAX_INPUT];
    size_t word_count = 0;
    size_t pos = 0;
    
    char *ptr = processed;
    while (*ptr != '\0') {
        while (*ptr == ' ') {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr != '\0' && *ptr != ' ') {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (pos + word_len + 1 < sizeof(words)) {
            if (pos > 0) {
                words[pos++] = ' ';
            }
            for (size_t m = 0; m < word_len; m++) {
                words[pos++] = word_start[m];
            }
            word_count++;
        } else {
            break;
        }
    }
    words[pos] = '\0';
    
    printf("Original: %s\n", input);
    printf("Filtered: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Words: %s\n", words);
    printf("Word count: %zu\n", word_count);
    
    char upper[MAX_INPUT];
    size_t upper_len = strlen(processed);
    if (upper_len >= sizeof(upper)) {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    for (size_t n = 0; n < upper_len; n++) {
        upper[n] = toupper((unsigned char)processed[n]);
    }
    upper[upper_len] = '\0';
    
    printf("Uppercase: %s\n", upper);
    
    return 0;
}