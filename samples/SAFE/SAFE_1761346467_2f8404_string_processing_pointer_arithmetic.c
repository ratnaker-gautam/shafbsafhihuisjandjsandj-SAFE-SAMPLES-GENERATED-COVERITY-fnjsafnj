//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + sizeof(input);
    
    while (*src != '\0' && src < end) {
        if (isalnum((unsigned char)*src)) {
            *dst = *src;
            dst++;
        } else if (isspace((unsigned char)*src)) {
            *dst = ' ';
            dst++;
        }
        src++;
        
        if ((size_t)(dst - output) >= sizeof(output) - 1) {
            break;
        }
    }
    
    *dst = '\0';
    
    char *start = output;
    char *current = output;
    
    while (*current != '\0') {
        if (*current == ' ') {
            if (current > start) {
                char *word_end = current - 1;
                char *word_start = start;
                
                while (word_start < word_end) {
                    char temp = *word_start;
                    *word_start = *word_end;
                    *word_end = temp;
                    word_start++;
                    word_end--;
                }
            }
            start = current + 1;
        }
        current++;
    }
    
    if (current > start) {
        char *word_end = current - 1;
        char *word_start = start;
        
        while (word_start < word_end) {
            char temp = *word_start;
            *word_start = *word_end;
            *word_end = temp;
            word_start++;
            word_end--;
        }
    }
    
    printf("Processed string: %s\n", output);
    
    return EXIT_SUCCESS;
}