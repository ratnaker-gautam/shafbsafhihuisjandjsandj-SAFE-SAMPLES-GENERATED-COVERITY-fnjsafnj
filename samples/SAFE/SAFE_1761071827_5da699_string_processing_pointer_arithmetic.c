//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT) return 0;
    const char *p = str;
    while (*p) {
        if (!isalnum((unsigned char)*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void reverse_words(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    start = str;
    while (*start) {
        while (*start && *start == ' ') start++;
        if (!*start) break;
        
        end = start;
        while (*end && *end != ' ') end++;
        
        char *word_start = start;
        char *word_end = end - 1;
        
        while (word_start < word_end) {
            char temp = *word_start;
            *word_start = *word_end;
            *word_end = temp;
            word_start++;
            word_end--;
        }
        
        start = end;
    }
}

void process_string(char *str) {
    if (str == NULL) return;
    
    char *p = str;
    char *q = str;
    
    while (*p) {
        if (isalpha((unsigned char)*p)) {
            *q = toupper((unsigned char)*p);
            q++;
        } else if (*p == ' ') {
            if (q > str && *(q-1) != ' ') {
                *q = ' ';
                q++;
            }
        }
        p++;
    }
    
    if (q > str && *(q-1) == ' ') q--;
    *q = '\0';
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and spaces allowed\n");
        return 1;
    }
    
    char processed[MAX_INPUT];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(processed, input);
    
    process_string(processed);
    reverse_words(processed);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    return 0;
}