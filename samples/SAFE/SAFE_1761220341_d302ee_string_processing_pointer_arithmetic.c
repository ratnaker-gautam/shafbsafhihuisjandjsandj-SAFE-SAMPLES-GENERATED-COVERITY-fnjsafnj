//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

int main(void) {
    char input[MAX_INPUT];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr != '\0' && !isspace((unsigned char)*ptr)) {
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
        
        if (*ptr != '\0') {
            ptr++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    printf("Reversed words:\n");
    
    for (int i = 0; i < word_count; i++) {
        char *word = words[i];
        size_t len = strlen(word);
        
        char *start = word;
        char *end = word + len - 1;
        
        while (start < end) {
            char temp = *start;
            *start = *end;
            *end = temp;
            start++;
            end--;
        }
        
        printf("%s\n", word);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}