//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

int main(void) {
    char input[MAX_INPUT + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 0;
    }
    
    char *ptr = input;
    char *end = input + len;
    
    while (ptr < end && word_count < MAX_WORDS) {
        while (ptr < end && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (ptr >= end) {
            break;
        }
        
        char *word_start = ptr;
        
        while (ptr < end && !isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            
            char *dest = word;
            char *src = word_start;
            for (size_t i = 0; i < word_len; i++) {
                *dest++ = *src++;
            }
            *dest = '\0';
            
            words[word_count++] = word;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}