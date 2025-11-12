//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') {
        if (!isalnum(*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void reverse_words(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    char *end = str;
    
    while (*end != '\0') {
        if (*end == ' ') {
            char *word_start = start;
            char *word_end = end - 1;
            while (word_start < word_end) {
                char temp = *word_start;
                *word_start = *word_end;
                *word_end = temp;
                word_start++;
                word_end--;
            }
            start = end + 1;
        }
        end++;
    }
    
    char *word_start = start;
    char *word_end = end - 1;
    while (word_start < word_end) {
        char temp = *word_start;
        *word_start = *word_end;
        *word_end = temp;
        word_start++;
        word_end--;
    }
    
    char *str_start = str;
    char *str_end = end - 1;
    while (str_start < str_end) {
        char temp = *str_start;
        *str_start = *str_end;
        *str_end = temp;
        str_start++;
        str_end--;
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (alphanumeric and spaces only, max %d chars): ", MAX_INPUT);
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input - only alphanumeric characters and spaces allowed\n");
        return 1;
    }
    
    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *src = input;
    char *dst = buffer;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    
    reverse_words(buffer);
    
    printf("Original: %s\n", input);
    printf("Reversed words: %s\n", buffer);
    
    free(buffer);
    return 0;
}